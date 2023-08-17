#include <DE/image/mypng.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace de {

	void mypng_user_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		// Permet de récupérer le pointeur de données lié à la structure PNG.
		MyPNG *mypng = (MyPNG *) png_get_io_ptr(png_ptr);
		size_t position = mypng->position();

		// Vérifie qu'il y a suffisamment d'octets à lire.
		if(position + length > mypng->size()) {
			png_error(png_ptr, "mypng_user_read_data: cannot read more data");
			return;
		}

		// Copie les données dans le buffer libpng.
		memcpy(data, mypng->data() + position, length);

		mypng->setPosition(position + length);
	}

	/*
	============
	MyPNG::MyPNG
	============
	*/
	MyPNG::MyPNG()
		: m_Data(nullptr),
		  m_Image(nullptr),
		  m_Size(0),
		  m_Position(0),
		  m_PNG(nullptr),
		  m_PNGInfo(nullptr),
		  m_Width(0),
		  m_Height(0),
		  m_BitDepth(0),
		  m_ColorType(MyPNGColorType::None),
		  m_InterlaceType(0),
		  m_Channels(0)
	{ }
	
	/*
	===============
	MyPNG::loadFile
	===============
	*/
	bool MyPNG::loadFile(const char *filename)
	{
		FILE *file = NULL;
		if(fopen_s(&file, filename, "rb") != 0)
			return false;

		fseek(file, 0L, SEEK_END);
		m_Size = ftell(file);

		fseek(file, 0L, SEEK_SET);

		m_Data = (uint8_t *) malloc(m_Size);
		if(m_Data == NULL)
			return false;

		fread(m_Data, sizeof(uint8_t), m_Size, file);
		fclose(file);

		return true;
	}

	/*
	==============
	MyPNG::destroy
	==============
	*/
	void MyPNG::destroy()
	{
		if(m_Image != nullptr) {
			png_bytep *rowPointers = (png_bytep *) m_Image;
			size_t row;

			for(row = 0; row < m_Height; ++row)
				free(*(rowPointers + row));

			free(rowPointers);
		}

		png_destroy_read_struct(&m_PNG, &m_PNGInfo, NULL);
		free(m_Data);
		m_Data = nullptr;
		m_Size = 0;
		m_Position = 0;
		m_PNG = nullptr;
		m_PNGInfo = nullptr;
	}

	/*
	============
	MyPNG::check
	============
	*/
	bool MyPNG::check() const
	{
		// Cette fonction vérifie si le buffer vaut bien bien la valeur de la signature du format PNG.
		return !png_sig_cmp(m_Data, 0, 4);
	}

	/*
	==================
	MyPNG::readPNGInfo
	==================
	*/
	bool MyPNG::readPNGInfo()
	{
		// Initialise et alloue de la mémoire pour la structure PNG que l'on utilisera par la suite.
		m_PNG = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if(m_PNG == NULL)
			return false;

		// Initialise et alloue de la mémoire pour la structure qui contiendra les informations concernant l'image.
		m_PNGInfo = png_create_info_struct(m_PNG);
		if(m_PNGInfo == NULL) {
			png_destroy_read_struct(&m_PNG, NULL, NULL);
			return false;
		}

		// Indique la fonction à exécuter lorsque libpng lit des données de l'image.
		png_set_read_fn(m_PNG, this, mypng_user_read_data);

		// Saute les 4 premiers octets correspondant à la signature.
		png_set_sig_bytes(m_PNG, 4);
		m_Position = 4;

		// Lit les informations de l'image.
		png_read_info(m_PNG, m_PNGInfo);

		// Récupère la largeur et la hauteur de l'image.
		png_get_IHDR(m_PNG, m_PNGInfo, &m_Width, &m_Height, &m_BitDepth, (int *) & m_ColorType, &m_InterlaceType, NULL, NULL);

		m_Channels = png_get_channels(m_PNG, m_PNGInfo);

		return true;
	}

	/*
	===================
	MyPNG::readPNGImage
	===================
	*/
	bool MyPNG::readPNGImage()
	{
		png_bytep *rowPointers = (png_bytep *) malloc(sizeof(png_bytep) * m_Height);
		uint32_t row;

		if(rowPointers == NULL)
			return false;

		int numberPassed;
		int pass;

#ifdef PNG_READ_INTERLACING_SUPPORTED
		numberPassed = png_set_interlace_handling(m_PNG);
#else
		numberPassed = 1;
#endif

		// Pour chaque ligne de l'image.
		for(row = 0; row < m_Height; ++row)
			*(rowPointers + row) = (png_bytep) malloc(png_get_rowbytes(m_PNG, m_PNGInfo));

		for(pass = 0; pass < numberPassed; ++pass) {
			for(row = 0; row < m_Height; ++row) {
				png_read_rows(m_PNG, rowPointers + row, NULL, 1);
			}
		}

		png_read_end(m_PNG, m_PNGInfo);

		m_Image = (uint8_t *) rowPointers;

		return true;
	}

	/*
	====================
	MyPNG::colorTypeName
	====================
	*/
	const char *MyPNG::colorTypeName(MyPNGColorType::ColorType colorType)
	{
		switch(colorType) {
			default:
				return "Unknown";
			case MyPNGColorType::None:
				return "None";
			case MyPNGColorType::Palette:
				return "Palette";
			case MyPNGColorType::Gray:
				return "Grayscale";
			case MyPNGColorType::RGB:
				return "RGB";
			case MyPNGColorType::RGBA:
				return "RGBA";
			case MyPNGColorType::GA:
				return "GA";
		}
	}

}
