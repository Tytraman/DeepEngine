#ifndef __DEEP_ENGINE_MYBMP_HPP__
#define __MYBMP_HPP__

#include <DE/def.h>
#include <DE/types.hpp>

namespace de {

	class MyPNG;

	constexpr uint16_t BMPSignature = (('M' << 8) | ('B'));

	DE_PACK(
		struct DE_API BMPFileHeader {
			uint16_t signature;
			uint32_t size;
			uint16_t reserved1;
			uint16_t reserved2;
			uint32_t imageDataOffset;
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

	class DE_API MyBMP {

		public:
			MyBMP();

			bool create(int32_t width, int32_t height, uint16_t colorDepth);
			void destroy();

			void setPNGImage(MyPNG &png);
			bool save(const char *filename) const;

		private:
			uint8_t *m_Data;
			uint32_t m_RowSize;

	};

}

#endif