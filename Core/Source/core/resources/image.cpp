#include "core/resources/image.hpp"

namespace deep
{

    /*
    ============
    image::image
    ============
    */
    image::image()
        : m_ColorData(nullptr),
          m_Width(0),
          m_Height(0),
          m_ColorModel(color_model::RGB)
    { }

    /*
    ============
    image::image
    ============
    */
    image::image(color_model colorModel)
        : m_ColorData(nullptr),
          m_Width(0),
          m_Height(0),
          m_ColorModel(colorModel)
    { }

}
