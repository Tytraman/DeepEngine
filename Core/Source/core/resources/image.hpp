#ifndef __DEEP_ENGINE_IMAGE_HPP__
#define __DEEP_ENGINE_IMAGE_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/resources/resource.hpp"
#include "maths/vec.hpp"
#include "core/templates/list.hpp"

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

            /// @brief Copie une colonne de l'image vers une autre colonne.
            /// @param columnToCopy L'index de la colonne à copier.
            /// @param destColumn L'index de la colonne destination.
            /// @return \c false si l'index d'une des deux colonnes dépasse la largeur de l'image.
            DE_API bool copy_column(uint64_t columnToCopy, uint64_t destColumn);

            DE_API bool copy_row(const vec2<uint64_t> &fromRowPosition, const vec2<uint64_t> &toRowPosition, uint64_t rowLength, ref<image> from);

            /// @brief Copie une ligne de l'image vers une autre ligne.
            /// @param rowToCopy L'index de la ligne à copier.
            /// @param destRow L'index de la ligne destination.
            /// @return \c false si l'index d'une des deux lignes dépasse la hauteur de l'image.
            DE_API bool copy_row(const vec2<uint64_t> &fromRowPosition, const vec2<uint64_t> &toRowPosition);

            DE_API bool resize(uint64_t width, uint64_t height);

            DE_API bool add(ref<image> other, const vec2<uint64_t> &position);

            DE_API uint64_t get_width() const;
            DE_API uint64_t get_height() const;
            DE_API color_model get_color_model() const;

            DE_API bool get(vec3<uint8_t> &dest, uint64_t row, uint64_t column) const;
            DE_API bool get(vec4<uint8_t> &dest, uint64_t row, uint64_t column) const;

        protected:
            DE_API uint8_t get_components_number() const;

            DE_API bool copy_row(const vec2<uint64_t> &fromRowPosition, const vec2<uint64_t> &toRowPosition, uint64_t rowLength, uint64_t fromWidth, uint64_t toWidth, uint8_t bytes, mem_ptr from, mem_ptr to);

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
    ===============
    image::copy_row
    ===============
    */
    inline bool image::copy_row(const vec2<uint64_t> &fromRowPosition, const vec2<uint64_t> &toRowPosition, uint64_t rowLength, ref<image> from)
    {
        return copy_row(fromRowPosition, toRowPosition, rowLength, from->get_width(), m_Width, get_components_number(), from->get_resource_data(), m_ColorData);
    }

    /*
    ===============
    image::copy_row
    ===============
    */
    inline bool image::copy_row(const vec2<uint64_t> &fromRowPosition, const vec2<uint64_t> &toRowPosition)
    {
        return copy_row(fromRowPosition, toRowPosition, m_Width, this);
    }

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
    ============================
    image::get_components_number
    ============================
    */
    inline uint8_t image::get_components_number() const
    {
        switch(m_ColorModel)
        {
            default:                return 0;
            case color_model::RGB:
            case color_model::BGR:  return 3;
            case color_model::RGBA:
            case color_model::ARGB:
            case color_model::BGRA: return 4;
        }
    }

}

#endif