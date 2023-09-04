#include <DE/image/mypng.hpp>
#include <DE/stream.hpp>

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
		: m_Image(nullptr),
		  m_Position(0),
		  m_PNG(nullptr),
		  m_PNGInfo(nullptr),
		  m_Width(0),
		  m_Height(0),
		  m_BitDepth(0),
		  m_ColorType(ImageColorType::None),
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
		InputFileStream ifs(filename);

		// Libère la mémoire du MemoryChunk pour éviter de faire un memory leak si il contenait déjà des données.
		m_MemoryChunk.free();

		if(!ifs.open())
			return false;

		size_t size = ifs.getFileSize();

		// Alloue de la mémoire pour le MemoryChunk.
		mem_ptr data = mem::alloc(size);
		if(data == nullptr) {
			ifs.close();
			return false;
		}

		m_MemoryChunk.setSize(size);
		m_MemoryChunk.setData(data);

		size_t bytesRead;

		// Lit toutes les données du fichiers et les stock dans le MemoryChunk.
		if(!ifs.readAll(data, &bytesRead)) {
			ifs.close();
			return false;
		}

		ifs.close();

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
				mem::free(*(rowPointers + row));

			mem::free(rowPointers);
		}

		png_destroy_read_struct(&m_PNG, &m_PNGInfo, NULL);
		m_MemoryChunk.free();
		m_Position = 0;
		m_PNG = nullptr;
		m_PNGInfo = nullptr;
	}

	/*
	============
	MyPNG::check
	============
	*/
	bool MyPNG::check()
	{
		// Cette fonction vérifie si le buffer vaut bien bien la valeur de la signature du format PNG.
		return !png_sig_cmp((png_const_bytep) m_MemoryChunk.data(), 0, 4);
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
		png_bytep *rowPointers = (png_bytep *) mem::alloc(sizeof(png_bytep) * m_Height);
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
		for(row = 0; row < m_Height; ++row) {
			size_t rowSize = png_get_rowbytes(m_PNG, m_PNGInfo);
			*(rowPointers + row) = (png_bytep) mem::alloc(rowSize);
		}

		for(pass = 0; pass < numberPassed; ++pass) {
			for(row = 0; row < m_Height; ++row) {
				png_read_rows(m_PNG, rowPointers + row, NULL, 1);
			}
		}

		png_read_end(m_PNG, m_PNGInfo);

		m_Image = (uint8_t *) rowPointers;

		return true;
	}

	mem_ptr MyPNG::rawImage()
	{
		png_bytep *rowPointers = (png_bytep *) m_Image;
		size_t rows = m_Height;
		size_t columns  = m_Width;
		png_bytep *r = rowPointers + rows - 1;

		uint8_t *memData = (uint8_t *) m_MemoryChunk.data();

		uint8_t b;

		size_t row, column;

		mem_ptr buffer = mem::alloc(m_Width * m_Height * m_Channels);
		uint8_t *data = (uint8_t *) buffer;

		// Parcourt toutes les lignes.
		for(row = 0; row < rows; ++row, --r) {
			// Parcourt toutes les colonnes de la ligne en cours.
			for(column = 0; column < columns; ++column) {
				for(b = 0; b < m_Channels; ++b)
					*(data + b) = (*r)[column * m_Channels + b];
					
				data += m_Channels;
			}
		}

		return buffer;
	}

	void MyPNG::applyHorizontalMirrorEffect()
	{
		png_bytep *rowPointers = (png_bytep *) m_Image;
		png_byte temp[4];
		uint32_t row;
		uint32_t column;
		uint32_t columns = m_Width / 2;

		// Pour chaque ligne de l'image.
		for(row = 0; row < m_Height; ++row) {
			for(column = 0; column < columns; ++column) {
				memcpy(temp, *(rowPointers + row) + column * m_Channels, m_Channels);
				memcpy(*(rowPointers + row) + column * m_Channels, *(rowPointers + row) + (m_Width - 1 - column) * m_Channels, m_Channels);
				memcpy(*(rowPointers + row) + (m_Width - 1 - column) * m_Channels, temp, m_Channels);
			}
		}
	}

	void MyPNG::applyVerticalMirrorEffect()
	{
		png_bytep *rowPointers = (png_bytep *) m_Image;
		png_byte temp[4];
		uint32_t row;
		uint32_t column;
		uint32_t rows = m_Height / 2;

		// Pour chaque colonne de l'image.
		for(column = 0; column < m_Width; ++column) {
			for(row = 0; row < rows; ++row) {
				// Copie le pixel actuel dans une variable temporaire.
				memcpy(temp, *(rowPointers + row) + column * m_Channels, m_Channels);

				// Copie le pixel de l'autre côté à la place du pixel actuel.
				memcpy(*(rowPointers + row) + column * m_Channels, *(rowPointers + m_Height - 1 - row) + column * m_Channels, m_Channels);

				// Copie le pixel de la variable temporaire à la place du pixel de l'autre côte.
				memcpy(*(rowPointers + m_Height - 1 - row) + column * m_Channels, temp, m_Channels);
			}
		}
	}

	void MyPNG::copyChannelColors(ImageChannel::t from, ImageChannel::t to)
	{
		png_bytep *rowPointers = (png_bytep *) m_Image;
		png_bytep *currentRow;
		uint32_t row;
		uint32_t column;

		// Inutile copier un channel vers le même channel.
		if(from == to)
			return;

		uint8_t channelFrom;
		uint8_t channelTo;

		switch(from) {
			default: return;
			case ImageChannel::Red: {
				channelFrom = 0;
			} break;
			case ImageChannel::Green: {
				channelFrom = 1;
			} break;
			case ImageChannel::Blue: {
				channelFrom = 2;
			} break;
			case ImageChannel::Alpha: {
				channelFrom = 3;
			} break;
		}

		switch(to) {
			default: return;
			case ImageChannel::Red: {
				channelTo = 0;
			} break;
			case ImageChannel::Green: {
				channelTo = 1;
			} break;
			case ImageChannel::Blue: {
				channelTo = 2;
			} break;
			case ImageChannel::Alpha: {
				channelTo = 3;
			} break;
		}

		// Pour chaque ligne de l'image
		for(row = 0; row < m_Height; ++row) {
			currentRow = rowPointers + row;
			// Pour chaque colonne de la ligne
			for(column = 0; column < m_Width; ++column) {
				*(*(currentRow) + column * m_Channels + channelTo) = *(*(currentRow) +column * m_Channels + channelFrom);
			}
		}
	}

	void MyPNG::swapChannelColors(ImageChannel::t channel1, ImageChannel::t channel2)
	{
		png_bytep *rowPointers = (png_bytep *) m_Image;
		png_bytep *currentRow;
		uint32_t row;
		uint32_t column;
		png_byte temp;

		// Inutile copier un channel vers le même channel.
		if(channel1 == channel2)
			return;

		uint8_t channelFrom;
		uint8_t channelTo;

		switch(channel1) {
			default: return;
			case ImageChannel::Red: {
				channelFrom = 0;
			} break;
			case ImageChannel::Green: {
				channelFrom = 1;
			} break;
			case ImageChannel::Blue: {
				channelFrom = 2;
			} break;
			case ImageChannel::Alpha: {
				channelFrom = 3;
			} break;
		}

		switch(channel2) {
			default: return;
			case ImageChannel::Red: {
				channelTo = 0;
			} break;
			case ImageChannel::Green: {
				channelTo = 1;
			} break;
			case ImageChannel::Blue: {
				channelTo = 2;
			} break;
			case ImageChannel::Alpha: {
				channelTo = 3;
			} break;
		}

		// Pour chaque ligne de l'image
		for(row = 0; row < m_Height; ++row) {
			currentRow = rowPointers + row;
			// Pour chaque colonne de la ligne
			for(column = 0; column < m_Width; ++column) {
				temp = *(*(currentRow) +column * m_Channels + channelFrom);
				*(*(currentRow) +column * m_Channels + channelFrom) = *(*(currentRow) + column * m_Channels + channelTo);
				*(*(currentRow) + column * m_Channels + channelTo) = temp;
			}
		}
	}

	void MyPNG::setChannelColor(ImageChannel::t channel, uint8_t value)
	{
		uint8_t chan;
		png_bytep *rowPointers = (png_bytep *) m_Image;
		png_bytep *currentRow;
		uint32_t row;
		uint32_t column;

		switch(channel) {
			default: return;
			case ImageChannel::Red: {
				chan = 0;
			} break;
			case ImageChannel::Green: {
				chan = 1;
			} break;
			case ImageChannel::Blue: {
				chan = 2;
			} break;
			case ImageChannel::Alpha: {
				chan = 3;
			} break;
		}

		// Pour chaque ligne de l'image
		for(row = 0; row < m_Height; ++row) {
			currentRow = rowPointers + row;
			// Pour chaque colonne de la ligne
			for(column = 0; column < m_Width; ++column) {
				*(*(currentRow) + column * m_Channels + chan) = value;
			}
		}
	}

	/*
	====================
	MyPNG::colorTypeName
	====================
	*/
	const char *MyPNG::colorTypeName(ImageColorType::t colorType)
	{
		switch(colorType) {
			default:
				return "Unknown";
			case ImageColorType::None:
				return "None";
			case ImageColorType::Palette:
				return "Palette";
			case ImageColorType::Gray:
				return "Grayscale";
			case ImageColorType::RGB:
				return "RGB";
			case ImageColorType::RGBA:
				return "RGBA";
			case ImageColorType::GA:
				return "GA";
		}
	}

}