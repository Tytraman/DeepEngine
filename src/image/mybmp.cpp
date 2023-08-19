#include <DE/image/mybmp.hpp>
#include <DE/image/mypng.hpp>
#include <DE/stream.hpp>

#include <string.h>
#include <math.h>

namespace de {

	/*
	============
	MyBMP::MyBMP
	============
	*/
	MyBMP::MyBMP()
		: m_RowSize(0)
	{ }

	/*
	=============
	MyBMP::create
	=============
	*/
	bool MyBMP::create(int32_t width, int32_t height, uint16_t colorDepth, ImageColorType::t colorType)
	{
		uint32_t infoHeaderSize;

		m_MemoryChunk.free();

		// Si le nombre de bits par pixel est inférieur ou égal à 8,
		// une table des couleurs doit être présente dans le fichier BMP.
		// Pour le moment ce n'est pas implémenté donc on retourne une erreur.
		if(colorDepth <= 8)
			return false;

		// Seuls les types de couleurs RGB et RGBA sont supportés.
		// Si le non support des autres types devient trop contraignant, alors ils seront ajoutés.
		switch(colorType) {
			default:
				return false;
			case ImageColorType::RGB: {
				infoHeaderSize = sizeof(BMPInfoHeader);
			} break;
			case ImageColorType::RGBA: {
				infoHeaderSize = sizeof(BMPV4Header);
			} break;
		}

		// Cette formule permet de faire du padding augmenté pour que la taille d'une ligne soit un multiple de 4.
		m_RowSize = ((colorDepth * width + 31) / 32) * 4;
		// Récupère la valeur absolue car la hauteur d'une image peut être négative.
		uint32_t imageHeight = abs(height);
		uint32_t imageSize = m_RowSize * imageHeight;

		uint32_t size = sizeof(BMPFileHeader) + infoHeaderSize + imageSize;

		if(size < 14)
			return false;

		mem_ptr data = mem::alloc(size);
		if(data == nullptr)
			return false;

		m_MemoryChunk.setData(data);
		m_MemoryChunk.setSize(size);

		BMPFileHeader *bmpFileHeader = (BMPFileHeader *) data;

		// ===== Bitmap File Header ===== //

		bmpFileHeader->signature       = BMPSignature;    // BM
		bmpFileHeader->size            = size;            // Taille totale du fichier.
		bmpFileHeader->reserved1       = bmpFileHeader->reserved2 = 0;
		bmpFileHeader->imageDataOffset = sizeof(BMPFileHeader) + infoHeaderSize;

		// ===== BITMAPINFOHEADER ===== //

		BMPInfoHeader *bmpInfoHeader = (BMPInfoHeader *) ((uint8_t *) data + sizeof(*bmpFileHeader));

		bmpInfoHeader->size                    = infoHeaderSize;    // Taille de l'en-tête
		bmpInfoHeader->width                   = width;             // Nombre de pixels de largeur.
		bmpInfoHeader->height                  = height;            // Nombre de pixels de hauteur.
		bmpInfoHeader->colorPlanes             = 1;                 // Doit être à 1.
		bmpInfoHeader->colorDepth              = colorDepth;        // 1, 4, 8, 16, 24 ou 32.
		bmpInfoHeader->imageSize               = imageSize;         // Taille du tableau de pixels.
		bmpInfoHeader->hResolution             = 0;                 // Pixel par mètre horizontal, 0 = pas de préférence.
		bmpInfoHeader->vResolution             = 0;                 // Pixel par mètre vertical, 0 = pas de préférence.
		bmpInfoHeader->numberOfColors          = 0;                 // Nombre de couleurs dans la palette.
		bmpInfoHeader->numberOfImportantColors = 0;                 // Nombre de couleurs importantes, 0 = toutes importantes.

		switch(colorType) {
			case ImageColorType::RGB: {
				bmpInfoHeader->compressionMethod = 0;
			} break;
			case ImageColorType::RGBA: {
				// La structure BITMAPV4HEADER permet de gérer la transparence dans un BMP.
				BMPV4Header *v4Header = (BMPV4Header *) bmpInfoHeader;
				
				bmpInfoHeader->compressionMethod = 3;    // BI_BITFIELD; Permet d'indiquer un masque de couleur pour chaque channel.

				v4Header->redChannelBitmask   = 0xFF000000;
				v4Header->greenChannelBitmask = 0x00FF0000;
				v4Header->blueChannelBitmask  = 0x0000FF00;
				v4Header->alphaChannelBitmask = 0x000000FF;
				v4Header->colorSpaceType = ImageColorSpaceSRGB;
				memset(v4Header->endpoints, 0, sizeof(v4Header->endpoints));            // Les endpoints ne sont pas utilisés lorsque l'espace de couleurs est sRGB.
				v4Header->gammaRed = v4Header->gammaGreen = v4Header->gammaBlue = 0;    // Les gammas ne sont pas utilisés lorsque l'espace de couleurs est sRGB.
			} break;
		}

		return true;
	}

	/*
	==============
	MyBMP::destroy
	==============
	*/
	void MyBMP::destroy()
	{
		m_MemoryChunk.free();
		m_RowSize = 0;
	}

	/*
	==================
	MyBMP::setPNGImage
	==================
	*/
	void MyBMP::setPNGImage(MyPNG &png)
	{
		png_bytep *rowPointers = (png_bytep *) png.image();
		size_t rows = png.height();
		size_t columns  = png.width();
		png_bytep *r = rowPointers + rows - 1;

		uint8_t * memData = (uint8_t *) m_MemoryChunk.data();

		uint8_t *data = memData + ((BMPFileHeader *) memData)->imageDataOffset;
		BMPInfoHeader *infoHeader = (BMPInfoHeader *) (memData + sizeof(BMPFileHeader));

		uint8_t b;
		uint8_t bytes;

		if(infoHeader->size == sizeof(BMPInfoHeader))
			bytes = 3;
		else if(infoHeader->size == sizeof(BMPV4Header))
			bytes = 4;
		else
			return;

		size_t row, column;
		uint32_t empty = m_RowSize % bytes;

		// Parcourt toutes les lignes.
		for(row = 0; row < rows; ++row, --r) {
			// Parcourt toutes les colonnes de la ligne en cours.
			for(column = 0; column < columns; ++column) {
				for(b = 0; b < bytes; ++b) {
					uint8_t gg = *(data + b) = (*r)[column * bytes + bytes - 1 - b];
					int o =0;
				}
					
				data += bytes;
			}
			// Rempli le padding de 0s.
			memset(data, 0, empty);
			data += empty;
		}
	}

	/*
	===========
	MyBMP::save
	===========
	*/
	bool MyBMP::save(const char *filename)
	{
		OutputFileStream ofs(filename);

		if(!ofs.open())
			return false;

		size_t bytesWritten;
		if(!ofs.write(m_MemoryChunk.data(), m_MemoryChunk.size(), &bytesWritten)) {
			ofs.close();
			return false;
		}

		ofs.close();

		return true;
	}

}