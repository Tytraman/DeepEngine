#ifndef __DEEP_ENGINE_MYBMP_HPP__
#define __DEEP_ENGINE_MYBMP_HPP__

#include <DE/image/image.hpp>

namespace de
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
			uint32_t hResolution;                // Mettre � 0 pour aucune pr�f�rence.
			uint32_t vResolution;                // Mettre � 0 pour aucune pr�f�rence.
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
    

	class DE_API bmp
    {

		public:
			bmp();

			bool create(int32_t width, int32_t height, uint16_t colorDepth, image_color_type colorType);
			void destroy();

            void convertRaw(uint8_t *raw);

			void convertFrom(png &png);
			bool save(const char *filename);

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