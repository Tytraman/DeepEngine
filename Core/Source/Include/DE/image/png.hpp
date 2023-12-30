#ifndef __DEEP_ENGINE_MYPNG_HPP__
#define __DEEP_ENGINE_MYPNG_HPP__

#include <DE/image/image.hpp>
#include <DE/stream.hpp>

namespace deep
{

    class DE_API png
    {

        public:
            png();

            bool loadFile(input_file_stream &ifs);

            /// @brief          Charge le contenu d'un fichier dans la mémoire.
            /// @param filename Nom du fichier à charger.
            /// @return         \c true si le fichier à été chargé en mémoire, sinon \c false.
            bool loadFile(const char *filename);
            bool loadFile(const wchar_t *filename);

            /// @brief Libère la mémoire utilisée.
            void destroy();

            /// @brief  Vérifie si les données chargées correspondent au format PNG.
            /// @return  \c true si le format est PNG, sinon \c false.
            bool check();

            bool readPNGInfo();
            bool readPNGImage();

            bool loadAndRead(const char *filename);
            bool loadAndRead(const wchar_t *filename);

            void applyHorizontalMirrorEffect();
            void applyVerticalMirrorEffect();
            void copyChannelColors(image_channel from, image_channel to);
            void swapChannelColors(image_channel channel1, image_channel channel2);
            void setChannelColor(image_channel channel, uint8_t value);

            uint8_t *data();
            size_t size() const;
            size_t position() const;
            uint32_t width() const;
            uint32_t height() const;
            int bitDepth() const;
            image_color_space colorType() const;
            int interlaceType() const;
            uint8_t channels() const;
            uint32_t colorDepth() const;
            uint8_t *image();
            mem_ptr rawImage();

            void setPosition(size_t pos);

            static const char *colorTypeName(image_color_space colorType);

        private:
            memory_chunk m_MemoryChunk;
            uint8_t *m_Image;
            size_t   m_Position;

            png_structp m_PNG;
            png_infop   m_PNGInfo;

            uint32_t m_Width;
            uint32_t m_Height;
            int m_BitDepth;
            image_color_space m_ColorType;
            int m_InterlaceType;
            uint8_t m_Channels;
    };

    /*
    =============
    png::loadFile
    =============
    */
    inline bool png::loadFile(const char *filename)
    {
        input_file_stream ifs(filename);
        bool ret = loadFile(ifs);
        ifs.close();

        return ret;
    }

    /*
    =============
    png::loadFile
    =============
    */
    inline bool png::loadFile(const wchar_t *filename)
    {
        input_file_stream ifs(filename);
        bool ret = loadFile(ifs);
        ifs.close();

        return ret;
    }


    /*
    =========
    png::data
    =========
    */
    inline uint8_t *png::data()
    {
        return (uint8_t *) m_MemoryChunk.data();
    }

    /*
    =========
    png::size
    =========
    */
    inline size_t png::size() const
    {
        return m_MemoryChunk.size();
    }

    /*
    =============
    png::position
    =============
    */
    inline size_t png::position() const
    {
        return m_Position;
    }

    /*
    ==========
    png::width
    ==========
    */
    inline uint32_t png::width() const
    {
        return m_Width;
    }

    /*
    ===========
    png::height
    ===========
    */
    inline uint32_t png::height() const
    {
        return m_Height;
    }

    /*
    =============
    png::bitDepth
    =============
    */
    inline int png::bitDepth() const
    {
        return m_BitDepth;
    }

    /*
    ==============
    png::colorType
    ==============
    */
    inline image_color_space png::colorType() const
    {
        return m_ColorType;
    }

    /*
    ==================
    png::interlaceType
    ==================
    */
    inline int png::interlaceType() const
    {
        return m_InterlaceType;
    }

    /*
    =============
    png::channels
    =============
    */
    inline uint8_t png::channels() const
    {
        return m_Channels;
    }

    /*
    ===============
    png::colorDepth
    ===============
    */
    inline uint32_t png::colorDepth() const
    {
        return m_BitDepth * m_Channels;
    }

    /*
    ==========
    png::image
    ==========
    */
    inline uint8_t *png::image()
    {
        return m_Image;
    }

    /*
    ================
    png::setPosition
    ================
    */
    inline void png::setPosition(size_t pos)
    {
        m_Position = pos;
    }

}

#endif