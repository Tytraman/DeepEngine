#include "image/bmp.hpp"
#include "image/png.hpp"
#include "io/stream.hpp"

#include <string.h>
#include <math.h>

namespace deep
{

    /*
    ========
    bmp::bmp
    ========
    */
    bmp::bmp()
        : m_ImageDataOffset(0),
          m_ColorTableOffset(0),
          m_ColorEntrySize(0),
          m_RowSize(0),
          m_Width(0),
          m_Height(0),
          m_ColorType(image_color_space::None),
          m_ColorDepth(0),
          m_InfoHeaderSize(0)
    { }

    /*
    ========
    bmp::bmp
    ========
    */
    bmp::bmp(const bmp &other)
        : m_ImageDataOffset(other.m_ImageDataOffset),
          m_ColorTableOffset(other.m_ColorTableOffset),
          m_ColorEntrySize(other.m_ColorEntrySize),
          m_RowSize(other.m_RowSize),
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
    bool bmp::create(int32_t width, int32_t height, uint16_t colorDepth, image_color_space colorType)
    {
        m_MemoryChunk.free();

        // Si le nombre de bits par pixel est inférieur ou égal à 8,
        // une table des couleurs doit être présente dans le fichier BMP.
        // Pour le moment ce n'est pas implémenté donc on retourne une erreur.
        if(colorDepth <= 8)
            return false;

        switch(colorType)
        {
            default:
                return false;
            case image_color_space::RGB:
            case image_color_space::BGR:
            {
                m_InfoHeaderSize = sizeof(bmp_info_header);
            } break;
            case image_color_space::RGBA:
            case image_color_space::BGRA:
            {
                m_InfoHeaderSize = sizeof(bmp_v4_header);
            } break;
        }

        m_ImageDataOffset = sizeof(bmp_file_header) + m_InfoHeaderSize;
        m_ColorType  = colorType;
        m_ColorDepth = colorDepth;

        if(!resize(width, height))
            return false;

        mem_ptr data = m_MemoryChunk.data();
        bmp_file_header *bmpFileHeader = (bmp_file_header *) data;

        // ===== Bitmap File Header ===== //

        bmpFileHeader->signature       = BMPSignature;    // BM
        bmpFileHeader->reserved1       = bmpFileHeader->reserved2 = 0;
        bmpFileHeader->imageDataOffset = m_ImageDataOffset;

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
            case image_color_space::RGB:
            case image_color_space::BGR:
            {
                bmpInfoHeader->compressionMethod = 0;
            } break;
            case image_color_space::RGBA:
            case image_color_space::BGRA:
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

    uint8_t __deep_color_table_4(uint8_t index, uint8_t *table, uint8_t *dest)
    {
        uint8_t index1 = index & 0xF;
        uint8_t index2 = (index >> 4) & 0xF;

        uint32_t entry1 = *((uint32_t *) ((uint32_t *) table + index1));
        uint32_t entry2 = *((uint32_t *) ((uint32_t *) table + index2));

        *((uint32_t *) dest)     = entry1;
        *((uint32_t *) dest + 1) = entry2;

        return 8;
    }

    uint8_t __deep_color_table_8(uint8_t index, uint8_t *table, uint8_t *dest)
    {
        uint32_t entry = *((uint32_t *) ((uint32_t *) table + index));

        *((uint32_t *) dest) = entry;

        return 4;
    }

    /*
    =======================
    bmp::create_from_stream
    =======================
    */
    bool bmp::create_from_stream(ref<stream> inputStream, image &dest)
    {
        if(!inputStream->can_read())
        {
            return false;
        }

        if(!inputStream->is_opened())
        {
            if(!inputStream->open())
            {
                return false;
            }
        }

        size_t fileSize = inputStream->get_length();
        uint8_t *rawFile = static_cast<uint8_t *>(mem::alloc(fileSize));

        size_t bytesRead = 0;

        // Lit l'en-tête du fichier.
        if(!inputStream->read(rawFile, 0, fileSize, &bytesRead))
        {
            mem::free(rawFile);

            return false;
        }

        bmp_file_header *bmpFileHeader = (bmp_file_header *) rawFile;
        bmp_info_header *bmpInfoHeader = (bmp_info_header *) (rawFile + sizeof(*bmpFileHeader));

        uint32_t infoHeaderSize = bmpInfoHeader->size;
        uint32_t imageDataOffset = bmpFileHeader->imageDataOffset;
        uint8_t bytes;

        uint64_t width  = bmpInfoHeader->width;
        uint64_t height = bmpInfoHeader->height;
        image::color_model model;

        uint32_t colorTableOffset = sizeof(bmp_file_header) + infoHeaderSize;

        bool colorTable = false;

        uint8_t (*callback)(uint8_t, uint8_t *, uint8_t *);

        switch(bmpInfoHeader->colorDepth)
        {
            default: return false;
            // La présence d'une table de couleurs est obligatoire quand le nombre de bits par pixel est <= 8.
            case 1:
            case 4:
            {
                callback = __deep_color_table_4;
            } goto case_8;
            case 8:
            {
                callback = __deep_color_table_8;
case_8:
                if(infoHeaderSize == sizeof(bmp_core_header))
                {
                    bytes = 3;

                    model = image::color_model::BGR;
                }
                else
                {
                    bytes = 4;

                    model = image::color_model::BGRA;
                }

                colorTable = true;
            } break;
            case 24:
            {
                model = image::color_model::BGR;

                bytes = 3;
            } break;
            case 32:
            {
                model = image::color_model::BGRA;

                bytes = 4;
            } break;
        }

        uint32_t rowSize = ((bmpInfoHeader->colorDepth * width + 31) / 32) * 4;

        size_t imageSize = width * height * bytes;
        mem_ptr destData = mem::alloc(imageSize);

        if(destData == nullptr)
        {
            mem::free(rawFile);

            return false;
        }

        if(dest.m_ColorData != nullptr)
        {
            mem::free(dest.m_ColorData);
        }

        uint32_t empty = rowSize % bytes;
        uint32_t trueRowSize = rowSize - empty;

        // Pointeurs vers le premier pixel, qui se trouve en bas à gauche.
        uint8_t *pixels = rawFile + bmpFileHeader->imageDataOffset;

        // La classe image doit stocker les pixels en partant de en haut à gauche.
        uint64_t h = 0;
        size_t pos;
        size_t destPos = 0;

        if(colorTable)
        {
            uint8_t *table = rawFile + colorTableOffset;
            uint64_t destIndex = 0;

            while(true)
            {
                pos = rowSize * h;

                uint32_t fromIndex;

                for(fromIndex = 0; fromIndex < trueRowSize; ++fromIndex)
                {
                    destIndex += callback(pixels[pos + fromIndex], table, static_cast<uint8_t *>(destData) + destIndex);
                }

                if(h >= height - 1)
                {
                    break;
                }

                h++;
            }
        }
        else
        {
            while(true)
            {
                pos = rowSize * h;

                memcpy(static_cast<uint8_t *>(destData) + destPos, pixels + pos, trueRowSize);

                destPos += width * bytes;

                if(h >= height - 1)
                {
                    break;
                }

                h++;
            }
        }

        dest.m_Width = width;
        dest.m_Height = height;
        dest.m_ColorModel = model;
        dest.m_ColorData = destData;

        mem::free(rawFile);

        return true;
    }

    /*
    =====================
    bmp::create_from_file
    =====================
    */
    bool bmp::create_from_file(const char *filename)
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

        m_MemoryChunk.set_data(data);
        m_MemoryChunk.set_size(size);

        bmp_file_header *bmpFileHeader = (bmp_file_header *) data;
        bmp_info_header *bmpInfoHeader = (bmp_info_header *) ((uint8_t *) data + sizeof(*bmpFileHeader));

        m_ImageDataOffset = bmpFileHeader->imageDataOffset;
        m_InfoHeaderSize = bmpInfoHeader->size;
        m_ColorDepth = bmpInfoHeader->colorDepth;

        switch(m_ColorDepth)
        {
            default:
            {
                m_ColorType = image_color_space::None;
            } break;
            case 1:
            case 4:
            case 8:
            {
                // La présence d'une table de couleurs est obligatoire quand le nombre de bits par pixel est <= 8.
                m_ColorType = image_color_space::Palette;
                m_ColorTableOffset = sizeof(bmp_file_header) + m_InfoHeaderSize;
                if(m_InfoHeaderSize == sizeof(bmp_core_header))
                    m_ColorEntrySize = 3;
                else
                    m_ColorEntrySize = 4;
            } break;
            case 24:
            {
                m_ColorType = image_color_space::BGR;
            } break;
            case 32:
            {
                m_ColorType = image_color_space::BGRA;
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
        uint32_t imageDataOffset = m_ImageDataOffset;

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

        if(memData != nullptr)
        {
            // Copie les en-têtes.
            memcpy(newData, memData, imageDataOffset);
        }
        
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

        if(memData != nullptr)
        {
            uint8_t bytes;
            switch(m_ColorType)
            {
                default:
                    return false;
                // TODO: gérer Palette
                case image_color_space::RGB:
                case image_color_space::BGR:
                {
                    bytes = 3;
                } break;
                case image_color_space::RGBA:
                case image_color_space::BGRA:
                {
                    bytes = 4;
                } break;
            }

            // Copie toutes les lignes.
            uint8_t *oldData = memData + imageDataOffset;
            int32_t row;
            for(row = 0; row < maxHeight; ++row)
                copy_row(row, row, maxWidth, m_RowSize, newRowSize, oldData, data);

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

            mem::free(memData);
        }

        // Met à jour les variables internes.
        m_Width   = width;
        m_Height  = height;
        m_RowSize = newRowSize;

        m_MemoryChunk.set_data(newData);
        m_MemoryChunk.set_size(newSize);

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

    /*
    ================
    bmp::convert_raw
    ================
    */
    void bmp::convert_raw(uint8_t *raw)
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
    =================
    bmp::convert_from
    =================
    */
    void bmp::convert_from(png &png)
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
    ================
    bmp::copy_column
    ================
    */
    bool bmp::copy_column(int32_t fromCol, int32_t destCol, int32_t startRow, int32_t endRow, int32_t destRow, uint32_t fromRowSize, uint32_t toRowSize, uint8_t *from, uint8_t *to)
    {
        uint8_t bytes;
        switch(m_ColorType)
        {
            default:
                return false;
            case image_color_space::RGB:
            case image_color_space::BGR:
            {
                bytes = 3;
            } break;
            case image_color_space::RGBA:
            case image_color_space::BGRA:
            {
                bytes = 4;
            } break;
        }

        size_t row;
        size_t destOffset;
        size_t indexOffset;

        for(row = startRow; row < endRow; ++row)
        {
            indexOffset = (fromCol * bytes) + (fromRowSize * row);
            destOffset = (destCol * bytes) + (toRowSize * (destRow + row));

            memcpy(to + destOffset, from + indexOffset, bytes);
        }

        return true;
    }

    /*
    ================
    bmp::copy_column
    ================
    */
    bool bmp::copy_column(int32_t index, int32_t dest, int32_t start, int32_t end)
    {
        uint8_t *memData = (uint8_t *) m_MemoryChunk.data();
        uint8_t *data = memData + ((bmp_file_header *) memData)->imageDataOffset;

        return copy_column(index, dest, start, end, 0, m_RowSize, m_RowSize, data, data);
    }

    /*
    =============
    bmp::copy_row
    =============
    */
    bool bmp::copy_row(int32_t index, int32_t dest, int32_t colEnd, uint32_t fromRowSize, uint32_t toRowSize, uint8_t *from, uint8_t *to)
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
            case image_color_space::RGB:
            case image_color_space::BGR:
            {
                bytes = 3;
            } break;
            case image_color_space::RGBA:
            case image_color_space::BGRA:
            {
                bytes = 4;
            } break;
        }

        uint32_t rowSize = fromRowSize - diff * bytes;

        memcpy(to + dest * toRowSize, from + index * fromRowSize, rowSize);

        return true;
    }

    /*
    =============
    bmp::copy_row
    =============
    */
    bool bmp::copy_row(int32_t index, int32_t dest, int32_t colEnd)
    {
        uint8_t *memData = (uint8_t *) m_MemoryChunk.data();
        uint8_t *data = memData + ((bmp_file_header *) memData)->imageDataOffset;

        return copy_row(index, dest, colEnd, m_RowSize, m_RowSize, data, data);
    }

    /*
    =============
    bmp::copy_row
    =============
    */
    bool bmp::copy_row(int32_t index, int32_t dest, uint8_t *from, uint8_t *to)
    {
        if(index > m_Height || dest > m_Height)
            return false;

        memcpy(to + dest * m_RowSize, from + index * m_RowSize, m_RowSize);

        return true;
    }

    /*
    ================
    bmp::cut_columns
    ================
    */
    bool bmp::cut_columns(int32_t start, int32_t end)
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
            case image_color_space::RGB:
            case image_color_space::BGR:
            {
                bytes = 3;
            } break;
            case image_color_space::RGBA:
            case image_color_space::BGRA:
            {
                bytes = 4;
            } break;
        }

        int32_t newWidth = width - diff;

        uint8_t *memData = (uint8_t *) m_MemoryChunk.data();
        uint8_t *data = memData + ((bmp_file_header *) memData)->imageDataOffset;

        int32_t index, dest = start;

        // Copie les colonnes de droite vers la gauche.
        for(index = end; index < width; ++index)
        {
            copy_column(index, dest, 0, m_Height, 0, m_RowSize, m_RowSize, data, data);
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
    =================
    bmp::swap_columns
    =================
    */
    void bmp::swap_columns(int32_t first, int32_t second, mem_ptr buffer, uint8_t bytes)
    {
        int32_t row;
        size_t destOffset;
        size_t indexOffset;
        size_t rowSize = m_RowSize;

        uint8_t *memData = (uint8_t *) m_MemoryChunk.data();
        uint8_t *data = memData + ((bmp_file_header *) memData)->imageDataOffset;

        int32_t height = m_Height;

        for(row = 0; row < height; ++row)
        {
            indexOffset = (first * bytes) + (rowSize * row);
            destOffset = (second * bytes) + (rowSize * row);

            memcpy(buffer, data + destOffset, bytes);
            memcpy(data + destOffset, data + indexOffset, bytes);
            memcpy(data + indexOffset, buffer, bytes);
        }
    }

    /*
    ==============
    bmp::swap_rows
    ==============
    */
    void bmp::swap_rows(int32_t first, int32_t second)
    {
        uint8_t *memData = (uint8_t *) m_MemoryChunk.data();
        uint8_t *data = memData + ((bmp_file_header *) memData)->imageDataOffset;

        size_t rowSize = m_RowSize;
        size_t indexOffset = first * rowSize;
        size_t destOffset = second * rowSize;

        // TODO: optimiser ça
        mem_ptr buffer = mem::alloc_no_track(rowSize);

        memcpy(buffer, data + destOffset, rowSize);
        memcpy(data + destOffset, data + indexOffset, rowSize);
        memcpy(data + indexOffset, buffer, rowSize);

        mem::free_no_track(buffer);
    }

    /*
    ==================
    bmp::vertical_flip
    ==================
    */
    void bmp::vertical_flip()
    {
        int32_t i, j;

        uint8_t bytes = 0;
        switch(m_ColorType)
        {
            default:
                return;
            case image_color_space::Palette:
            {
                if(m_ColorDepth != 8)
                    return;

                bytes = 1;
            } break;
            case image_color_space::RGB:
            case image_color_space::BGR:
            {
                bytes = 3;
            } break;
            case image_color_space::RGBA:
            case image_color_space::BGRA:
            {
                bytes = 4;
            } break;
        }

        uint8_t buffer[4];

        for(i = 0, j = m_Width - 1; i < j; ++i, --j)
            swap_columns(i, j, buffer, bytes);
    }

    /*
    ====================
    bmp::horizontal_flip
    ====================
    */
    void bmp::horizontal_flip()
    {
        int32_t i, j;

        for(i = 0, j = m_Height - 1; i < j; ++i, --j)
            swap_rows(i, j);
    }

    void __convert_to_BGR_depth_4(uint8_t index, uint32_t *currentRowSize, uint8_t *palette, uint8_t *newImage)
    {
        uint8_t index1 = index & 0xF;
        uint8_t index2 = (index >> 4) & 0xF;

        uint32_t entry1 = *((uint32_t *) ((uint32_t *) palette + index1));
        uint32_t entry2 = *((uint32_t *) ((uint32_t *) palette + index2));

        newImage[*currentRowSize]     =  entry1        & 0xFF;
        newImage[*currentRowSize + 1] = (entry1 >> 8)  & 0xFF;
        newImage[*currentRowSize + 2] = (entry1 >> 16) & 0xFF;

        newImage[*currentRowSize + 3] =  entry2        & 0xFF;
        newImage[*currentRowSize + 4] = (entry2 >> 8)  & 0xFF;
        newImage[*currentRowSize + 5] = (entry2 >> 16) & 0xFF;

        *currentRowSize += 6;
    }

    void __convert_to_BGR_depth_8(uint8_t index, uint32_t *currentRowSize, uint8_t *palette, uint8_t *newImage)
    {
        uint32_t entry = *((uint32_t *) ((uint32_t *) palette + index));

        newImage[*currentRowSize]     =  entry        & 0xFF;
        newImage[*currentRowSize + 1] = (entry >> 8)  & 0xFF;
        newImage[*currentRowSize + 2] = (entry >> 16) & 0xFF;

        *currentRowSize += 3;
    }

    /*
    ===================
    bmp::convert_to_BGR
    ===================
    */
    bool bmp::convert_to_BGR()
    {
        if(m_ColorType == image_color_space::BGR)
            return true;

        int32_t width = m_Width;
        int32_t height = m_Height;

        uint16_t colorDepth = 24;
        uint8_t *oldImage = (uint8_t *) get_image();

        // Nouvelle taille d'une ligne.
        uint32_t newRowSize = ((colorDepth * width + 31) / 32) * 4;
        uint32_t imageSize = newRowSize * height;

        uint32_t imageDataOffset = sizeof(bmp_file_header) + m_InfoHeaderSize;
        uint32_t newSize = imageDataOffset + imageSize;

        if(newSize < 14)
            return false;

        if(m_ColorType == image_color_space::Palette)
        {
            void (*callback)(uint8_t, uint32_t *, uint8_t *, uint8_t *);
            switch(m_ColorDepth)
            {
                default:
                    return false;
                case 4:
                {
                    callback = __convert_to_BGR_depth_4;
                } break;
                case 8:
                {
                    callback = __convert_to_BGR_depth_8;
                } break;
            }

            uint8_t *palette = (uint8_t *) get_color_table();
            uint8_t *memData = (uint8_t *) m_MemoryChunk.data();

            uint8_t *newData = (uint8_t *) mem::alloc(newSize);
            if(newData == nullptr)
                return false;

            if(memData != nullptr)
            {
                // Copie les en-têtes.
                memcpy(newData, memData, imageDataOffset);
            }

            bmp_file_header *fileHeader = (bmp_file_header *) newData;
            bmp_info_header *infoHeader = (bmp_info_header *) (newData + sizeof(bmp_file_header));
            
            m_RowSize = newRowSize;
            m_ColorDepth = colorDepth;
            m_ColorEntrySize = 0;
            m_ColorTableOffset = 0;
            m_ImageDataOffset = imageDataOffset;

            fileHeader->size = newSize;
            fileHeader->imageDataOffset = imageDataOffset;

            uint16_t currentColorDepth = infoHeader->colorDepth;
            infoHeader->colorDepth = colorDepth;
            infoHeader->colorPlanes = 1;
            infoHeader->imageSize = imageSize;
            infoHeader->numberOfColors = 0;
            infoHeader->numberOfImportantColors = 0;
            infoHeader->hResolution = 0;
            infoHeader->vResolution = 0;

            uint8_t *newImage = newData + imageDataOffset;

            uint32_t currentRowSize = 0;
            uint8_t empty = newRowSize % 3;
            size_t len = width * height;
            if(currentColorDepth == 4)
                len /= 2;

            size_t i;
            size_t current = 0;

            uint8_t index;

            // Pour chaque pixel.
            for(i = 0; i < len; ++i)
            {
                if(current == width)
                {
                    current = 0;
                    currentRowSize += empty;
                }

                index = oldImage[i];

                callback(index, &currentRowSize, palette, newImage);
            }
            if(memData != nullptr)
                mem::free(memData);

            m_MemoryChunk.set_data(newData);
            m_MemoryChunk.set_size(newSize);
        }
        else
            return false;

        m_ColorType = image_color_space::BGR;

        return true;
    }

    /*
    =========
    bmp::save
    =========
    */
    bool bmp::add(bmp &other, const vec2<int32_t> &position)
    {
        convert_to_BGR();
        other.convert_to_BGR();

        int32_t width = m_Width;            // Largeur de l'image actuelle.
        int32_t height = m_Height;          // Hauteur de l'image actuelle.
        int32_t oWidth = other.m_Width;     // Largeur de l'image à ajouter.
        int32_t oHeight = other.m_Height;   // Hauteur de l'image à ajouter.

        int32_t targetWidth = position.x + oWidth;
        int32_t widthDiff = targetWidth - width;

        int32_t targetHeight = position.y + oHeight;
        int32_t heightDiff = targetHeight - height;

        if(!resize((widthDiff > 0 ? targetWidth : width), (heightDiff > 0 ? targetHeight : height)))
            return false;

        int32_t i;
        uint8_t *to = (uint8_t *) get_image();
        uint8_t *from = (uint8_t *) other.get_image();

        int32_t x = position.x;

        for(i = 0; i < oWidth; ++i, ++x)
            copy_column(i, x, 0, oHeight, position.y, other.m_RowSize, m_RowSize, from, to);

        return true;
    }

}
