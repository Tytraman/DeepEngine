#ifndef __DEEP_ENGINE_MYBMP_HPP__
#define __DEEP_ENGINE_MYBMP_HPP__

#include <DE/image/image.hpp>

namespace deep
{

	constexpr uint16_t BMPSignature = (('M' << 8) | ('B'));

	class png;

	DE_PACK(
		struct DE_API bmp_file_header
        {
			uint16_t signature;          // 2
			uint32_t size;               // 4
			uint16_t reserved1;          // 2
			uint16_t reserved2;          // 2
			uint32_t imageDataOffset;    // 4
		}
	);

    

	DE_PACK(
		struct DE_API bmp_info_header
        {
			uint32_t size;
			int32_t width;
			int32_t height;
			uint16_t colorPlanes;
			uint16_t colorDepth;                 // Ou bit depth.
			uint32_t compressionMethod;
			uint32_t imageSize;
			uint32_t hResolution;                // Mettre à 0 pour aucune préférence.
			uint32_t vResolution;                // Mettre à 0 pour aucune préférence.
			uint32_t numberOfColors;
			uint32_t numberOfImportantColors;
		}
	);

	DE_PACK(
		struct DE_API bmp_v4_header
        {
			bmp_info_header infoHeader;
			uint32_t redChannelBitmask;
			uint32_t greenChannelBitmask;
			uint32_t blueChannelBitmask;
			uint32_t alphaChannelBitmask;
			uint32_t colorSpaceType;
			uint8_t  endpoints[0x24];
			uint32_t gammaRed;
			uint32_t gammaGreen;
			uint32_t gammaBlue;
		}
	);
    
	/// @brief Classe permettant l'ouverture, la modification et la conversion d'images bitmap.
	class DE_API bmp
    {

		public:
			/// @brief Constructeur qui initialise les variables internes à leurs valeurs par défaut.
			bmp();

            /// @brief Construit une nouvelle image bmp en faisant une copie de la mémoire d'une autre image bmp.
            /// @param other L'image bmp à copier.
            bmp(const bmp &other);

			bool create(int32_t width, int32_t height, uint16_t colorDepth, image_color_type colorType);

            /// @brief Crée une image bmp en l'ouvrant depuis un fichier.
            /// @param filename Le chemin du fichier à ouvrir.
            /// @return \c true si l'opération réussie.
            bool createFromFile(const char *filename);

			/// @brief Libère la mémoire utilisée par l'image.
			void destroy();

            /// @brief Convertie une zone mémoire brute en image bmp.
            /// @param raw Zone mémoire à convertir.
            void convertRaw(uint8_t *raw);

			/// @brief Convertie une image png en image bmp.
			/// @param png L'image bmp à convertir.
			void convertFrom(png &png);

			/// @brief Sauvegarde l'image bmp.
			/// @param filename Le chemin du fichier dans lequel sauvegarder l'image.
			/// @return \c true si l'opération réussie.
			bool save(const char *filename);

            bool copyColumn(int32_t index, int32_t dest, int32_t start, int32_t end, uint8_t *from, uint8_t *to);
            bool copyColumn(int32_t index, int32_t dest, int32_t start, int32_t end);

            bool copyRow(int32_t index, int32_t dest, int32_t colEnd, uint32_t fromRowSize, uint32_t toRowSize, uint8_t *from, uint8_t *to);
            bool copyRow(int32_t index, int32_t dest, int32_t colEnd);

            bool copyRow(int32_t index, int32_t dest, uint8_t *from, uint8_t *to);

            bool cutColumns(int32_t start, int32_t end);

            void swapColumns(int32_t first, int32_t second, mem_ptr buffer, uint8_t bytes);

            void verticalFlip();
            void horizontalFlip();

            bool add(const bmp &other);

            bool resize(int32_t width, int32_t height);

			int32_t getWidth() const;
			int32_t getHeight() const;
			image_color_type getColorType() const;
			mem_ptr data();
			mem_ptr image();

		private:
			memory_chunk m_MemoryChunk;
			uint32_t m_RowSize;
			int32_t m_Width;
			int32_t m_Height;
			image_color_type m_ColorType;
            uint16_t m_ColorDepth;
            uint32_t m_InfoHeaderSize;

	};

	inline int32_t bmp::getWidth() const
	{
		return m_Width;
	}

	inline int32_t bmp::getHeight() const
	{
		return m_Height;
	}

	inline image_color_type bmp::getColorType() const
	{
		return m_ColorType;
	}

	inline mem_ptr bmp::data()
	{
		return m_MemoryChunk.data();
	}

	inline mem_ptr bmp::image()
	{
		return (uint8_t *) m_MemoryChunk.data() + ((bmp_file_header *) m_MemoryChunk.data())->imageDataOffset;
	}

}

#endif