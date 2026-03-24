#ifndef DEEP_ENGINE_D3D_MESH_HPP
#define DEEP_ENGINE_D3D_MESH_HPP

#include "deep_d3d_export.h"
#include "D3D/drawable/drawable.hpp"
#include "D3D/buffer/index_buffer.hpp"

#include <DeepLib/memory/ref_counted.hpp>

namespace deep
{
    namespace D3D
    {
        class DEEP_D3D_API mesh : public drawable
        {
          public:
            virtual void draw(device_context &dc, const fmat4 &view_projection) override;

          protected:
            DEEP_REF(index_buffer, m_index_buffer)

          protected:
            using drawable::drawable;

          public:
            friend class drawable_factory;
        };
    } // namespace D3D
} // namespace deep

#endif
