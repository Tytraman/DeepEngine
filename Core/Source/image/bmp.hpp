#ifndef __DEEP_ENGINE_MYBMP_HPP__
#define __DEEP_ENGINE_MYBMP_HPP__

#include "image/image.hpp"
#include "maths/vec.hpp"

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
        struct DE_API bmp_core_header
        {
            uint32_t size;
            int16_t width;
            int16_t height;
            uint16_t colorPlanes;
            uint16_t colorDepth;
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
    class bmp
    {

        public:
            /// @brief Constructeur qui initialise les variables internes à leurs valeurs par défaut.
            DE_API bmp();

            /// @brief Construit une nouvelle image bmp en faisant une copie de la mémoire d'une autre image bmp.
            /// @param other L'image bmp à copier.
            DE_API bmp(const bmp &other);

            DE_API bool create(int32_t width, int32_t height, uint16_t colorDepth, image_color_space colorType);

            /// @brief Crée une image bmp en l'ouvrant depuis un fichier.
            /// @param filename Le chemin du fichier à ouvrir.
            /// @return \c true si l'opération réussie.
            DE_API bool create_from_file(const char *filename);

            /// @brief Libère la mémoire utilisée par l'image.
            DE_API void destroy();

            /// @brief Convertie une zone mémoire brute en image bmp.
            /// @param raw Zone mémoire à convertir.
            DE_API void convert_raw(uint8_t *raw);

            /// @brief Convertie une image png en image bmp.
            /// @param png L'image bmp à convertir.
            DE_API void convert_from(png &png);

            /// @brief Sauvegarde l'image bmp.
            /// @param filename Le chemin du fichier dans lequel sauvegarder l'image.
            /// @return \c true si l'opération réussie.
            DE_API bool save(const char *filename);

            DE_API bool copy_column(int32_t fromCol, int32_t destCol, int32_t startRow, int32_t endRow, int32_t destRow, uint32_t fromRowSize, uint32_t toRowSize, uint8_t *from, uint8_t *to);
            DE_API bool copy_column(int32_t index, int32_t dest, int32_t start, int32_t end);

            DE_API bool copy_row(int32_t index, int32_t dest, int32_t colEnd, uint32_t fromRowSize, uint32_t toRowSize, uint8_t *from, uint8_t *to);
            DE_API bool copy_row(int32_t index, int32_t dest, int32_t colEnd);
            DE_API bool copy_row(int32_t index, int32_t dest, uint8_t *from, uint8_t *to);

            DE_API bool cut_columns(int32_t start, int32_t end);

            DE_API void swap_columns(int32_t first, int32_t second, mem_ptr buffer, uint8_t bytes);
            DE_API void swap_rows(int32_t first, int32_t second);

            DE_API void vertical_flip();
            DE_API void horizontal_flip();

            DE_API bool convert_to_BGR();

            DE_API bool add(bmp &other, const vec2<int32_t> &position);
            DE_API bool add(bmp &other);

            DE_API bool resize(int32_t width, int32_t height);

            DE_API int32_t get_width() const;
            DE_API int32_t get_height() const;
            DE_API image_color_space get_color_type() const;
            DE_API mem_ptr get_data();
            DE_API mem_ptr get_image();
            DE_API mem_ptr get_color_table();

        private:
            memory_chunk m_MemoryChunk;
            uint32_t m_ImageDataOffset;
            uint32_t m_ColorTableOffset;
            uint8_t m_ColorEntrySize;
            uint32_t m_RowSize;
            int32_t m_Width;
            int32_t m_Height;
            image_color_space m_ColorType;
            uint16_t m_ColorDepth;
            uint32_t m_InfoHeaderSize;

    };

    /*
    ========
    bmp::add
    ========
    */
    inline bool bmp::add(bmp &other)
    {
        return add(other, vec2<int32_t>(m_Width, 0));
    }

    /*
    =============
    bmp::get_width
    =============
    */
    inline int32_t bmp::get_width() const
    {
        return m_Width;
    }

    /*
    ==============
    bmp::get_height
    ==============
    */
    inline int32_t bmp::get_height() const
    {
        return m_Height;
    }

    /*
    =================
    bmp::get_color_type
    =================
    */
    inline image_color_space bmp::get_color_type() const
    {
        return m_ColorType;
    }

    /*
    =========
    bmp::data
    =========
    */
    inline mem_ptr bmp::get_data()
    {
        return m_MemoryChunk.data();
    }

    /*
    ==========
    bmp::image
    ==========
    */
    inline mem_ptr bmp::get_image()
    {
        uint8_t *memData = (uint8_t *) m_MemoryChunk.data();
        uint8_t *data = memData + m_ImageDataOffset;

        return data;
    }

    /*
    ===============
    bmp::get_color_table
    ===============
    */
    inline mem_ptr bmp::get_color_table()
    {
        if(m_ColorTableOffset == 0)
            return nullptr;

        uint8_t *memData = (uint8_t *) m_MemoryChunk.data();

        return memData + m_ColorTableOffset;
    }

}

#endif