#ifndef __DEEP_ENGINE_MYBMP_HPP__
#define __DEEP_ENGINE_MYBMP_HPP__

#include <DE/image/image.hpp>

namespace de {

	constexpr uint16_t BMPSignature = (('M' << 8) | ('B'));

	class MyPNG;

	DE_PACK(
		struct DE_API BMPFileHeader {
			uint16_t signature;          // 2
			uint32_t size;               // 4
			uint16_t reserved1;          // 2
			uint16_t reserved2;          // 2
			uint32_t imageDataOffset;    // 4
		}
	);

	DE_PACK(
		struct DE_API BMPInfoHeader {
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
		struct DE_API BMPV4Header {
			BMPInfoHeader infoHeader;
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

	class DE_API MyBMP {

		public:
			MyBMP();

			bool create(int32_t width, int32_t height, uint16_t colorDepth, ImageColorType::t colorType);
			void destroy();

			void setPNGImage(MyPNG &png);
			bool save(const char *filename);

		private:
			MemoryChunk m_MemoryChunk;
			uint32_t m_RowSize;

	};

}

#endif