#include <DE/image/bmp.hpp>
#include <DE/image/png.hpp>
#include <DE/stream.hpp>

#include <string.h>
#include <math.h>

namespace deep {

	/*
	========
	bmp::bmp
	========
	*/
	bmp::bmp()
		: m_RowSize(0),
		  m_Width(0),
		  m_Height(0),
		  m_ColorType(image_color_type::None),
          m_ColorDepth(0),
          m_InfoHeaderSize(0)
	{ }

    /*
	========
	bmp::bmp
	========
	*/
    bmp::bmp(const bmp &other)
        : m_RowSize(other.m_RowSize),
          m_Width(other.m_Width),
          m_Height(other.m_Height),
          m_ColorType(other.m_ColorType),
          m_ColorDepth(other.m_ColorDepth),
          m_InfoHeaderSize(other.m_InfoHeaderSize),
          m_MemoryChunk(other.m_MemoryChunk)
    { }

	/*
	===========
	bmp::create
	===========
	*/
	bool bmp::create(int32_t width, int32_t height, uint16_t colorDepth, image_color_type colorType)
	{
		m_MemoryChunk.free();

		// Si le nombre de bits par pixel est inférieur ou égal à 8,
		// une table des couleurs doit être présente dans le fichier BMP.
		// Pour le moment ce n'est pas implémenté donc on retourne une erreur.
		if(colorDepth <= 8)
			return false;

		// Seuls les types de couleurs RGB et RGBA sont supportés.
		// Si le non support des autres types devient trop contraignant, alors ils seront ajoutés.
		switch(colorType)
        {
			default:
				return false;
			case image_color_type::RGB:
            {
				m_InfoHeaderSize = sizeof(bmp_info_header);
			} break;
			case image_color_type::RGBA:
            {
				m_InfoHeaderSize = sizeof(bmp_v4_header);
			} break;
		}

        m_ColorType  = colorType;
        m_ColorDepth = colorDepth;

        if(!resize(width, height))
            return false;

        mem_ptr data = m_MemoryChunk.data();
		bmp_file_header *bmpFileHeader = (bmp_file_header *) data;

		// ===== Bitmap File Header ===== //

		bmpFileHeader->signature       = BMPSignature;    // BM
		bmpFileHeader->reserved1       = bmpFileHeader->reserved2 = 0;
		bmpFileHeader->imageDataOffset = sizeof(bmp_file_header) + m_InfoHeaderSize;

		// ===== BITMAPINFOHEADER ===== //

		bmp_info_header *bmpInfoHeader = (bmp_info_header *) ((uint8_t *) data + sizeof(*bmpFileHeader));

		bmpInfoHeader->size                    = m_InfoHeaderSize;  // Taille de l'en-tête
		bmpInfoHeader->colorPlanes             = 1;                 // Doit être à 1.
		bmpInfoHeader->colorDepth              = colorDepth;        // 1, 4, 8, 16, 24 ou 32.
		bmpInfoHeader->hResolution             = 0;                 // Pixel par mètre horizontal, 0 = pas de préférence.
		bmpInfoHeader->vResolution             = 0;                 // Pixel par mètre vertical, 0 = pas de préférence.
		bmpInfoHeader->numberOfColors          = 0;                 // Nombre de couleurs dans la palette.
		bmpInfoHeader->numberOfImportantColors = 0;                 // Nombre de couleurs importantes, 0 = toutes importantes.

		switch(colorType)
        {
			case image_color_type::RGB:
            {
				bmpInfoHeader->compressionMethod = 0;
			} break;
			case image_color_type::RGBA:
            {
				// La structure BITMAPV4HEADER permet de gérer la transparence dans un BMP.
				bmp_v4_header *v4Header = (bmp_v4_header *) bmpInfoHeader;
				
				bmpInfoHeader->compressionMethod = 3;    // BI_BITFIELD; Permet d'indiquer un masque de couleur pour chaque channel.

				v4Header->redChannelBitmask   = 0x00FF0000;
				v4Header->greenChannelBitmask = 0x0000FF00;
				v4Header->blueChannelBitmask  = 0x000000FF;
				v4Header->alphaChannelBitmask = 0xFF000000;
				v4Header->colorSpaceType = ImageColorSpaceSRGB;
				memset(v4Header->endpoints, 0, sizeof(v4Header->endpoints));            // Les endpoints ne sont pas utilisés lorsque l'espace de couleurs est sRGB.
				v4Header->gammaRed = v4Header->gammaGreen = v4Header->gammaBlue = 0;    // Les gammas ne sont pas utilisés lorsque l'espace de couleurs est sRGB.
			} break;
		}

		return true;
	}

    /*
	===================
	bmp::createFromFile
	===================
	*/
    bool bmp::createFromFile(const char *filename)
    {
        input_file_stream ifs(filename);

        if(!ifs.open())
            return false;

        size_t size = ifs.getFileSize();
        mem_ptr data = mem::alloc(size);
        if(data == nullptr)
            return false;

        size_t bytesRead;
        if(!ifs.readAll(data, &bytesRead))
        {
            mem::free(data);
            ifs.close();
            return false;
        }

        m_MemoryChunk.setData(data);
        m_MemoryChunk.setSize(size);

        bmp_file_header *bmpFileHeader = (bmp_file_header *) data;
        bmp_info_header *bmpInfoHeader = (bmp_info_header *) ((uint8_t *) data + sizeof(*bmpFileHeader));

        m_InfoHeaderSize = bmpInfoHeader->size;
        m_ColorDepth = bmpInfoHeader->colorDepth;

        switch(m_ColorDepth)
        {
            default:
            {
                m_ColorType = image_color_type::None;
            } break;
            case 24:
            {
                m_ColorType = image_color_type::RGB;
            } break;
            case 32:
            {
                m_ColorType = image_color_type::RGBA;
            } break;
        }

        m_Width   = bmpInfoHeader->width;
        m_Height  = bmpInfoHeader->height;

        m_RowSize = ((m_ColorDepth * m_Width + 31) / 32) * 4;

        ifs.close();

        return true;
    }

    /*
	===========
	bmp::resize
	===========
	*/
    bool bmp::resize(int32_t width, int32_t height)
    {
        uint8_t *memData = (uint8_t *) m_MemoryChunk.data();
        uint32_t imageDataOffset = ((bmp_file_header *) memData)->imageDataOffset;
        uint8_t *oldData = memData + ((bmp_file_header *) memData)->imageDataOffset;

        // Cette formule permet de faire du padding augmenté pour que la taille d'une ligne soit un multiple de 4.
        uint32_t newRowSize = ((m_ColorDepth * width + 31) / 32) * 4;

        // Récupère la valeur absolue car la hauteur d'une image peut être négative.
        uint32_t imageHeight = abs(height);
		uint32_t imageSize = newRowSize * imageHeight;

        uint32_t newSize = sizeof(bmp_file_header) + m_InfoHeaderSize + imageSize;
        
        if(newSize < 14)
			return false;

        // Alloue la mémoire pour pouvoir copier l'image dedans.
        mem_ptr newData = mem::alloc(newSize);
        if(newData == nullptr)
            return false;

        // Copie les en-têtes.
        memcpy(newData, memData, imageDataOffset);
        
        // Pointeur vers la zone qui stock les pixels.
        uint8_t *data = (uint8_t *) newData + imageDataOffset;

        bmp_file_header *bmpFileHeader = (bmp_file_header *) newData;
        bmp_info_header *bmpInfoHeader = (bmp_info_header *) ((uint8_t *) newData + sizeof(*bmpFileHeader));

        bmpFileHeader->size      = newSize;     // Taille totale du fichier.

        bmpInfoHeader->width     = width;       // Nombre de pixels de largeur.
        bmpInfoHeader->height    = height;      // Nombre de pixels de hauteur.
        bmpInfoHeader->imageSize = imageSize;   // Taille du tableau de pixels.

        int32_t maxWidth;
        int32_t maxHeight;
        
        if(width > m_Width)
            maxWidth = m_Width;
        else
            maxWidth = width;

        if(height > m_Height)
            maxHeight = m_Height;
        else
            maxHeight = height;

        // Copie toutes les lignes.
        int32_t row;
        for(row = 0; row < maxHeight; ++row)
            copyRow(row, row, maxWidth, m_RowSize, newRowSize, oldData, data);

        // Met à jour les variables internes.
        m_Width   = width;
        m_Height  = height;
        m_RowSize = newRowSize;

        uint8_t bytes;
        switch(m_ColorType)
        {
            default:
                return false;
            case image_color_type::RGB:
            {
                bytes = 3;
            } break;
            case image_color_type::RGBA:
            {
                bytes = 4;
            } break;
        }

        size_t rows = height;
        size_t indexOffset;
        uint32_t empty = newRowSize % bytes;

        uint32_t em;

        // Ajoute le padding à la fin de chaque ligne.
        for(row = 0; row < rows; ++row)
        {
            indexOffset = (width * bytes) + (newRowSize * row);

            for(em = 0; em < empty; ++em)
            {
                *(data + indexOffset + em) = 0;
            }
        }

        m_MemoryChunk.setData(newData);
        m_MemoryChunk.setSize(newSize);

        mem::free(memData);

        return true;
    }

	/*
	============
	bmp::destroy
	============
	*/
	void bmp::destroy()
	{
		m_MemoryChunk.free();
		m_RowSize = 0;
	}

    void bmp::convertRaw(uint8_t *raw)
    {
        int32_t cols = m_Width;
        int32_t rows = m_Height;

        uint8_t * memData = (uint8_t *) m_MemoryChunk.data();
        uint8_t *data = memData + ((bmp_file_header *) memData)->imageDataOffset;

        bmp_info_header *infoHeader = (bmp_info_header *) (memData + sizeof(bmp_file_header));

        uint8_t b;

        // Correspond aux nombres d'octets par pixels, dans le cas de RGB, 3 pixels sont nécessaires.
		uint8_t bytes;

		if(infoHeader->size == sizeof(bmp_info_header))
			bytes = 3;
		else if(infoHeader->size == sizeof(bmp_v4_header))
			bytes = 4;
		else
			return;

        uint8_t *r = raw;

        int32_t row, col;
		uint32_t empty = m_RowSize % bytes;

        // Parcourt toutes les lignes.
		for(row = 0; row < rows; ++row)
        {
			// Parcourt toutes les colonnes de la ligne en cours.
			for(col = 0; col < cols; ++col)
            {
				for(b = 0; b < bytes; ++b, ++data, ++r)
                {
					*data = *r;
				}
			}

			// Rempli le padding de 0s.
			memset(data, 0, empty);
			data += empty;
		}
    }

	/*
	================
	bmp::convertFrom
	================
	*/
	void bmp::convertFrom(png &png)
	{
		png_bytep *rowPointers = (png_bytep *) png.image();
		size_t rows = png.height();
		size_t columns  = png.width();
		png_bytep *r = rowPointers + rows - 1;

		uint8_t *memData = (uint8_t *) m_MemoryChunk.data();
		uint8_t *data = memData + ((bmp_file_header *) memData)->imageDataOffset;

		bmp_info_header *infoHeader = (bmp_info_header *) (memData + sizeof(bmp_file_header));

		uint8_t b;
		uint8_t bytes;

		if(infoHeader->size == sizeof(bmp_info_header))
			bytes = 3;
		else if(infoHeader->size == sizeof(bmp_v4_header))
			bytes = 4;
		else
			return;

		size_t row, column;
		uint32_t empty = m_RowSize % bytes;

		// Parcourt toutes les lignes.
		for(row = 0; row < rows; ++row, --r)
        {
			// Parcourt toutes les colonnes de la ligne en cours.
			for(column = 0; column < columns; ++column)
            {
				for(b = 0; b < bytes; ++b)
                {
					*(data + b) = (*r)[column * bytes + bytes - 1 - b];
				}
					
				data += bytes;
			}

			// Rempli le padding de 0s.
			memset(data, 0, empty);
			data += empty;
		}
	}

	/*
	=========
	bmp::save
	=========
	*/
	bool bmp::save(const char *filename)
	{
		output_file_stream ofs(filename);

		if(!ofs.open(false, true))
			return false;

		size_t bytesWritten;
		if(!ofs.write(m_MemoryChunk.data(), m_MemoryChunk.size(), &bytesWritten))
        {
			ofs.close();
			return false;
		}

		ofs.close();

		return true;
	}

    /*
	===============
	bmp::copyColumn
	===============
	*/
    bool bmp::copyColumn(int32_t index, int32_t dest, int32_t start, int32_t end, uint8_t *from, uint8_t *to)
    {
        if(index > m_Width || dest > m_Width)
            return false;

        uint8_t bytes;
        switch(m_ColorType)
        {
            default:
                return false;
            case image_color_type::RGB:
            {
                bytes = 3;
            } break;
            case image_color_type::RGBA:
            {
                bytes = 4;
            } break;
        }

        size_t row;
        size_t destOffset;
        size_t indexOffset;
        size_t rowSize = m_RowSize;

        for(row = start; row < end; ++row)
        {
            indexOffset = (index * bytes) + (rowSize * row);
            destOffset = (dest * bytes) + (rowSize * row);

            memcpy(to + destOffset, from + indexOffset, bytes);
        }

        return true;
    }

    bool bmp::copyColumn(int32_t index, int32_t dest, int32_t start, int32_t end)
    {
        uint8_t *memData = (uint8_t *) m_MemoryChunk.data();
        uint8_t *data = memData + ((bmp_file_header *) memData)->imageDataOffset;

        return copyColumn(index, dest, start, end, data, data);
    }

    /*
	============
	bmp::copyRow
	============
	*/
    bool bmp::copyRow(int32_t index, int32_t dest, int32_t colEnd, uint32_t fromRowSize, uint32_t toRowSize, uint8_t *from, uint8_t *to)
    {
        if(index > m_Height || dest > m_Height)
            return false;

        // RowSize contient le nombre d'octets que fait une ligne.
        // Il faut donc retirer à RowSize la différence entre la largeur totale et la colonne de fin.

        int32_t diff = m_Width - colEnd;
        if(diff < 0)
            return false;

        uint8_t bytes;
        switch(m_ColorType)
        {
            default:
                return false;
            case image_color_type::RGB:
            {
                bytes = 3;
            } break;
            case image_color_type::RGBA:
            {
                bytes = 4;
            } break;
        }

        uint32_t rowSize = fromRowSize - diff * bytes;

        memcpy(to + dest * toRowSize, from + index * fromRowSize, rowSize);

        return true;
    }

    /*
	============
	bmp::copyRow
	============
	*/
    bool bmp::copyRow(int32_t index, int32_t dest, int32_t colEnd)
    {
        uint8_t *memData = (uint8_t *) m_MemoryChunk.data();
        uint8_t *data = memData + ((bmp_file_header *) memData)->imageDataOffset;

        return copyRow(index, dest, colEnd, m_RowSize, m_RowSize, data, data);
    }

    /*
	============
	bmp::copyRow
	============
	*/
    bool bmp::copyRow(int32_t index, int32_t dest, uint8_t *from, uint8_t *to)
    {
        if(index > m_Height || dest > m_Height)
            return false;

        memcpy(to + dest * m_RowSize, from + index * m_RowSize, m_RowSize);

        return true;
    }

    /*
	===============
	bmp::cutColumns
	===============
	*/
    bool bmp::cutColumns(int32_t start, int32_t end)
    {
        int32_t width = m_Width;
        int32_t diff = end - start;

        if(diff < 0 || end > width || start < 0)
            return false;

        uint8_t bytes = 0;
        switch(m_ColorType)
        {
            default:
                return false;
            case image_color_type::RGB:
            {
                bytes = 3;
            } break;
            case image_color_type::RGBA:
            {
                bytes = 4;
            } break;
        }

        int32_t newWidth = width - diff;

        uint8_t *memData = (uint8_t *) m_MemoryChunk.data();
        uint32_t imageDataOffset = ((bmp_file_header *) memData)->imageDataOffset;
        uint8_t *data = memData + ((bmp_file_header *) memData)->imageDataOffset;

        int32_t index, dest = start;

        // Copie les colonnes de droite vers la gauche.
        for(index = end; index < width; ++index)
        {
            copyColumn(index, dest, 0, m_Height, data, data);
            dest++;
        }

        if(!resize(newWidth, m_Height))
            return false;

        uint32_t rowSize = m_RowSize;
        uint32_t empty = rowSize % bytes;

        memData = (uint8_t *) m_MemoryChunk.data();
        data = memData + ((bmp_file_header *) memData)->imageDataOffset;

        size_t row;
        size_t rows = m_Height;
        size_t indexOffset;

        uint32_t em;

        if(empty > 0)
        {
            // Ajoute le padding à la fin de chaque ligne.
            for(row = 0; row < rows; ++row)
            {
                indexOffset = rowSize * row;

                for(em = 0; em < empty; ++em)
                {
                    *(data + indexOffset - em) = 0;
                }
            }
        }

        return true;
    }

    /*
	================
	bmp::swapColumns
	================
	*/
    void bmp::swapColumns(int32_t first, int32_t second, mem_ptr buffer, uint8_t bytes)
    {
        size_t row;
        size_t destOffset;
        size_t indexOffset;
        size_t rowSize = m_RowSize;

        uint8_t *memData = (uint8_t *) m_MemoryChunk.data();
        uint8_t *data = memData + ((bmp_file_header *) memData)->imageDataOffset;

        for(row = 0; row < m_Height; ++row)
        {
            indexOffset = (first * bytes) + (rowSize * row);
            destOffset = (second * bytes) + (rowSize * row);

            memcpy(buffer, data + destOffset, bytes);
            memcpy(data + destOffset, data + indexOffset, bytes);
            memcpy(data + indexOffset, buffer, bytes);
        }
    }

    /*
	=================
	bmp::verticalFlip
	=================
	*/
    void bmp::verticalFlip()
    {
        int32_t i, j;

        uint8_t bytes = 0;
        switch(m_ColorType)
        {
            default:
                return;
            case image_color_type::RGB:
            {
                bytes = 3;
            } break;
            case image_color_type::RGBA:
            {
                bytes = 4;
            } break;
        }

        uint8_t buffer[4];

        for(i = 0, j = m_Width - 1; i < j; ++i, --j)
            swapColumns(i, j, buffer, bytes);
    }

    /*
	===================
	bmp::horizontalFlip
	===================
	*/
    void bmp::horizontalFlip()
    {
        // TODO: faire swapRows
        // TODO: faire cette fonction
    }

    /*
	=========
	bmp::save
	=========
	*/
    bool bmp::add(const bmp &other)
    {
        return true;
    }

}