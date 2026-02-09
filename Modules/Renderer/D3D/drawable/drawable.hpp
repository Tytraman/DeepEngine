#ifndef DEEP_ENGINE_D3D_DRAWABLE_HPP
#define DEEP_ENGINE_D3D_DRAWABLE_HPP

#include "deep_d3d_export.h"
#include <DeepLib/object.hpp>
#include <DeepLib/maths/vec.hpp>
#include <DeepLib/maths/mat.hpp>

#include "D3D/device_context.hpp"
#include "D3D/buffer/vertex_buffer.hpp"
#include "D3D/buffer/constant_buffer.hpp"
#include "D3D/shader/vertex_shader.hpp"
#include "D3D/shader/pixel_shader.hpp"

namespace deep
{
    namespace D3D
    {
        template class DEEP_D3D_API ref<vertex_buffer>;
        template class DEEP_D3D_API ref<constant_buffer>;
        template class DEEP_D3D_API ref<vertex_shader>;
        template class DEEP_D3D_API ref<pixel_shader>;

        class DEEP_D3D_API drawable : public object
        {
          public:
            drawable()                            = delete;
            drawable(const drawable &)            = delete;
            drawable &operator=(const drawable &) = delete;

            virtual void draw(device_context &dc, const fmat4 &view_projection) = 0;

          protected:
            ref<vertex_buffer> m_vertex_buffer;
            ref<vertex_shader> m_vertex_shader;
            ref<pixel_shader> m_pixel_shader;
            ref<constant_buffer> m_per_object_buffer;

            DEEP_FVEC3 m_location;
            DEEP_FVEC3 m_rotation;
            DEEP_FVEC3 m_scale;

          protected:
            using object::object;
        };
    } // namespace D3D
} // namespace deep

#endif
