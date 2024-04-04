#ifndef __DEEP_ENGINE_IMAGE_HPP__
#define __DEEP_ENGINE_IMAGE_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/resources/resource.hpp"
#include "maths/vec.hpp"

namespace deep
{

    class bmp;
    class png;

    class image : public resource
    {

        public:
            enum class color_model
            {
                RGB,
                BGR,
                RGBA,
                ARGB,
                BGRA
            };

        public:
            DE_API image();
            DE_API image(color_model colorModel);

            DE_API mem_ptr get_resource_data() const override;
            DE_API size_t get_resource_size() const override;

            DE_API uint64_t get_width() const;
            DE_API uint64_t get_height() const;
            DE_API color_model get_color_model() const;

            DE_API bool get(vec3<uint8_t> &dest, uint64_t row, uint64_t column) const;
            DE_API bool get(vec4<uint8_t> &dest, uint64_t row, uint64_t column) const;

        protected:
            mem_ptr m_ColorData;
            uint64_t m_Width;
            uint64_t m_Height;
            color_model m_ColorModel;

        public:
            friend bmp;
            friend png;

    };

    /*
    =====================
    image::get_color_data
    =====================
    */
    inline mem_ptr image::get_resource_data() const
    {
        return m_ColorData;
    }

    /*
    ========================
    image::get_resource_size
    ========================
    */
    inline size_t image::get_resource_size() const
    {
        size_t size = m_Width * m_Height;

        if(m_ColorModel == color_model::RGB || m_ColorModel == color_model::BGR)
        {
            size *= 3;
        }
        else
        {
            size *= 4;
        }

        return size;
    }

    /*
    ================
    image::get_width
    ================
    */
    inline uint64_t image::get_width() const
    {
        return m_Width;
    }

    /*
    =================
    image::get_height
    =================
    */
    inline uint64_t image::get_height() const
    {
        return m_Height;
    }

    /*
    ======================
    image::get_color_model
    ======================
    */
    inline image::color_model image::get_color_model() const
    {
        return m_ColorModel;
    }

    /*
    ==========
    image::get
    ==========
    */
    inline bool image::get(vec3<uint8_t> &dest, uint64_t row, uint64_t column) const
    {
        if(column > m_Width || row > m_Height)
        {
            return false;
        }

        uint64_t index = row * column;
        uint8_t *ptr = static_cast<uint8_t *>(m_ColorData);

        switch(m_ColorModel)
        {
            default: return false;
            case color_model::RGB:
            case color_model::BGR:
            case color_model::RGBA:
            case color_model::BGRA:
            {
                dest.x = ptr[index];
                dest.y = ptr[index + 1];
                dest.z = ptr[index + 2];
            } break;
            case color_model::ARGB:
            {
                dest.x = ptr[index + 1];
                dest.y = ptr[index + 2];
                dest.z = ptr[index + 3];
            } break;
        }

        return true;
    }

    /*
    ==========
    image::get
    ==========
    */
    inline bool image::get(vec4<uint8_t> &dest, uint64_t row, uint64_t column) const
    {
        if(column > m_Width || row > m_Height)
        {
            return false;
        }

        uint64_t index = row * column;
        uint8_t *ptr = static_cast<uint8_t *>(m_ColorData);

        switch(m_ColorModel)
        {
            default: return false;
            case color_model::RGB:
            case color_model::BGR:
            {
                dest.x = ptr[index];
                dest.y = ptr[index + 1];
                dest.z = ptr[index + 2];
                dest.w = 255;
            } break;
            case color_model::RGBA:
            case color_model::BGRA:
            case color_model::ARGB:
            {
                dest.x = ptr[index];
                dest.y = ptr[index + 1];
                dest.z = ptr[index + 2];
                dest.w = ptr[index + 3];
            } break;
        }

        return true;
    }

}

#endif