#include <DE/image/mybmp.hpp>
#include <DE/image/mypng.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

namespace de {

	/*
	============
	MyBMP::MyBMP
	============
	*/
	MyBMP::MyBMP()
		: m_Data(nullptr),
		  m_RowSize(0)
	{ }

	/*
	=============
	MyBMP::create
	=============
	*/
	bool MyBMP::create(int32_t width, int32_t height, uint16_t colorDepth)
	{
		// Cette formule permet de faire du padding augmenté pour que la taille d'une ligne soit un multiple de 4.
		m_RowSize = ((colorDepth * width + 31) / 32) * 4;
		// Récupère la valeur absolue car la hauteur d'une image peut être négative.
		uint32_t imageHeight = abs(height);
		uint32_t imageSize = m_RowSize * imageHeight;

		uint32_t size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + imageSize;

		if(size < 14)
			return false;

		// Si le nombre de bits par pixel est inférieur ou égal à 8,
		// une table des couleurs doit être présente dans le fichier BMP.
		// Pour le moment ce n'est pas implémenté donc on retourne une erreur.
		if(colorDepth <= 8)
			return false;

		m_Data = (uint8_t *) malloc(size);
		if(m_Data == NULL)
			return false;

		BMPFileHeader *bmpFileHeader = (BMPFileHeader *) m_Data;

		// ===== Bitmap File Header ===== //

		bmpFileHeader->signature       = BMPSignature;
		bmpFileHeader->size            = size;
		bmpFileHeader->reserved1       = bmpFileHeader->reserved2 = 0;
		bmpFileHeader->imageDataOffset = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

		// ===== BITMAPINFOHEADER ===== //

		BMPInfoHeader *bmpInfoHeader = (BMPInfoHeader *) (m_Data + sizeof(*bmpFileHeader));

		bmpInfoHeader->size                    = 40;          // Taille de l'en-tête
		bmpInfoHeader->width                   = width;
		bmpInfoHeader->height                  = height;
		bmpInfoHeader->colorPlanes             = 1;           // Doit être à 1
		bmpInfoHeader->colorDepth              = colorDepth;  // 1, 4, 8, 16, 24 ou 32
		bmpInfoHeader->compressionMethod       = 0;           // BI_ALPHABITFIELDS
		bmpInfoHeader->imageSize               = imageSize;   // Taille du tableau de pixels
		bmpInfoHeader->hResolution             = 0;           // Pixel par mètre horizontal, 0 = pas de préférence
		bmpInfoHeader->vResolution             = 0;           // Pixel par mètre vertical, 0 = pas de préférence
		bmpInfoHeader->numberOfColors          = 0;           // Nombre de couleurs dans la palette
		bmpInfoHeader->numberOfImportantColors = 0;           // Nombre de couleurs importantes, 0 = toutes importantes

		return true;
	}

	/*
	==============
	MyBMP::destroy
	==============
	*/
	void MyBMP::destroy()
	{

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

		uint8_t *data = m_Data + ((BMPFileHeader *) m_Data)->imageDataOffset;

		size_t row, column;
		uint32_t empty = m_RowSize % 3;

		// Parcourt toutes les lignes.
		for(row = 0; row < rows; ++row, --r) {
			// Parcourt toutes les colonnes de la ligne en cours.
			for(column = 0; column < columns; ++column) {
				*(data)     = (*r)[column * 3 + 2];
				*(data + 1) = (*r)[column * 3 + 1];
				*(data + 2) = (*r)[column * 3];
				data += 3;
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
	bool MyBMP::save(const char *filename) const
	{
		FILE *file;
		if(fopen_s(&file, filename, "wb") != 0)
			return false;

		fwrite(m_Data, sizeof(uint8_t), ((BMPFileHeader *) m_Data)->size, file);

		fclose(file);

		return true;
	}

}
