#ifndef DEEP_ENGINE_D3D_CUBE_HPP
#define DEEP_ENGINE_D3D_CUBE_HPP

#include "deep_d3d_export.h"
#include "D3D/drawable/drawable.hpp"

namespace deep
{
    namespace D3D
    {
        class DEEP_D3D_API cube : public drawable
        {
          public:
            virtual void draw(device_context &dc) override;

          protected:
            ref<constant_buffer> m_transform_buffer;
            ref<constant_buffer> m_color_buffer;

          protected:
            using drawable::drawable;

          public:
            friend class drawable_factory;
        };
    } // namespace D3D
} // namespace deep

#endif
