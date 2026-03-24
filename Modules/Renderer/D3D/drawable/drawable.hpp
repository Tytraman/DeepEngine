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

            virtual ref<vertex_buffer> get_vertex_buffer() const noexcept;
            virtual ref<vertex_shader> get_vertex_shader() const noexcept;
            virtual ref<pixel_shader> get_pixel_shader() const noexcept;
            virtual ref<constant_buffer> get_per_object_buffer() const noexcept;

            virtual void set_vertex_buffer(const ref<vertex_buffer> &buffer) noexcept;
            virtual void set_vertex_shader(const ref<vertex_shader> &shader) noexcept;
            virtual void set_pixel_shader(const ref<pixel_shader> &shader) noexcept;
            virtual void set_per_object_buffer(const ref<constant_buffer> &buffer) noexcept;

            virtual fvec3 get_location() const noexcept;
            virtual fvec3 get_rotation() const noexcept;
            virtual fvec3 get_scale() const noexcept;

            virtual void set_location(const fvec3 &location) noexcept;
            virtual void set_rotation(const fvec3 &rotation) noexcept;
            virtual void set_scale(const fvec3 &scale) noexcept;

          protected:
            ref<vertex_buffer> m_vertex_buffer;
            ref<vertex_shader> m_vertex_shader;
            ref<pixel_shader> m_pixel_shader;
            ref<constant_buffer> m_per_object_buffer;

            DEEP_FVEC3(m_location)
            DEEP_FVEC3(m_rotation)
            DEEP_FVEC3(m_scale)

          protected:
            using object::object;
        };
    } // namespace D3D
} // namespace deep

#endif
