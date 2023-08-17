#ifndef __DEEP_ENGINE_MYPNG_HPP__
#define __DEEP_ENGINE_MYPNG_HPP__

#include <DE/def.h>
#include <DE/types.hpp>

#include <png.h>

namespace de {


	namespace MyPNGColorType {
		enum ColorType : int {
			None    = -1,
			Gray    = PNG_COLOR_TYPE_GRAY,
			Palette = PNG_COLOR_TYPE_PALETTE,
			RGB     = PNG_COLOR_TYPE_RGB,
			RGBA    = PNG_COLOR_TYPE_RGBA,
			GA      = PNG_COLOR_TYPE_GA
		};
	}
	

	class DE_API MyPNG {

		public:
			MyPNG();

			/// @brief          Charge le contenu d'un fichier dans la mémoire.
			/// @param filename Nom du fichier à charger.
			/// @return         \c true si le fichier à été chargé en mémoire, sinon \c false.
			bool loadFile(const char *filename);

			/// @brief Libère la mémoire utilisée.
			void destroy();

			/// @brief  Vérifie si les données chargées correspondent au format PNG.
			/// @return  \c true si le format est PNG, sinon \c false.
			bool check() const;

			bool readPNGInfo();
			bool readPNGImage();

			uint8_t *data();
			size_t size() const;
			size_t position() const;
			uint32_t width() const;
			uint32_t height() const;
			int bitDepth() const;
			MyPNGColorType::ColorType colorType() const;
			int interlaceType() const;
			uint8_t channels() const;
			uint32_t colorDepth() const;
			uint8_t *image();

			void setPosition(size_t pos);

			static const char *colorTypeName(MyPNGColorType::ColorType colorType);

		private:
			uint8_t *m_Data;
			uint8_t *m_Image;
			size_t   m_Size;
			size_t   m_Position;

			png_structp m_PNG;
			png_infop   m_PNGInfo;

			uint32_t m_Width;
			uint32_t m_Height;
			int m_BitDepth;
			MyPNGColorType::ColorType m_ColorType;
			int m_InterlaceType;
			uint8_t m_Channels;
	};

	/*
	===========
	MyPNG::data
	===========
	*/
	inline uint8_t *MyPNG::data()
	{
		return m_Data;
	}

	/*
	===========
	MyPNG::size
	===========
	*/
	inline size_t MyPNG::size() const
	{
		return m_Size;
	}

	/*
	===============
	MyPNG::position
	===============
	*/
	inline size_t MyPNG::position() const
	{
		return m_Position;
	}

	/*
	============
	MyPNG::width
	============
	*/
	inline uint32_t MyPNG::width() const
	{
		return m_Width;
	}

	/*
	=============
	MyPNG::height
	=============
	*/
	inline uint32_t MyPNG::height() const
	{
		return m_Height;
	}

	/*
	===============
	MyPNG::bitDepth
	===============
	*/
	inline int MyPNG::bitDepth() const
	{
		return m_BitDepth;
	}

	/*
	================
	MyPNG::colorType
	================
	*/
	inline MyPNGColorType::ColorType MyPNG::colorType() const
	{
		return m_ColorType;
	}

	/*
	====================
	MyPNG::interlaceType
	====================
	*/
	inline int MyPNG::interlaceType() const
	{
		return m_InterlaceType;
	}

	/*
	===============
	MyPNG::channels
	===============
	*/
	inline uint8_t MyPNG::channels() const
	{
		return m_Channels;
	}

	/*
	=================
	MyPNG::colorDepth
	=================
	*/
	inline uint32_t MyPNG::colorDepth() const
	{
		return m_BitDepth * m_Channels;
	}

	/*
	============
	MyPNG::image
	============
	*/
	inline uint8_t *MyPNG::image()
	{
		return m_Image;
	}

	/*
	==================
	MyPNG::setPosition
	==================
	*/
	inline void MyPNG::setPosition(size_t pos)
	{
		m_Position = pos;
	}

}

#endif