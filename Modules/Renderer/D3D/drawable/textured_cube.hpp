#ifndef DEEP_ENGINE_D3D_TEXTURED_CUBE_HPP
#define DEEP_ENGINE_D3D_TEXTURED_CUBE_HPP

#include "deep_d3d_export.h"
#include "D3D/drawable/drawable.hpp"
#include "D3D/texture.hpp"
#include "D3D/sampler.hpp"

namespace deep
{
    namespace D3D
    {
        class DEEP_D3D_API textured_cube : public drawable
        {
          public:
            virtual void draw(device_context &dc, const fmat4 &view_projection) override;

          protected:
            ref<texture> m_texture;
            ref<sampler> m_sampler;

          protected:
            using drawable::drawable;

          public:
            friend class drawable_factory;
        };
    } // namespace D3D
} // namespace deep

#endif
