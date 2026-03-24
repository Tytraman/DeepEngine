#include "D3D/drawable/drawable.hpp"

namespace deep
{
    namespace D3D
    {
        ref<vertex_buffer> drawable::get_vertex_buffer() const noexcept
        {
            return m_vertex_buffer;
        }

        ref<vertex_shader> drawable::get_vertex_shader() const noexcept
        {
            return m_vertex_shader;
        }

        ref<pixel_shader> drawable::get_pixel_shader() const noexcept
        {
            return m_pixel_shader;
        }

        ref<constant_buffer> drawable::get_per_object_buffer() const noexcept
        {
            return m_per_object_buffer;
        }

        void drawable::set_vertex_buffer(const ref<vertex_buffer> &buffer) noexcept
        {
            m_vertex_buffer = buffer;
        }

        void drawable::set_vertex_shader(const ref<vertex_shader> &shader) noexcept
        {
            m_vertex_shader = shader;
        }

        void drawable::set_pixel_shader(const ref<pixel_shader> &shader) noexcept
        {
            m_pixel_shader = shader;
        }

        void drawable::set_per_object_buffer(const ref<constant_buffer> &buffer) noexcept
        {
            m_per_object_buffer = buffer;
        }

        fvec3 drawable::get_location() const noexcept
        {
            return m_location;
        }

        fvec3 drawable::get_rotation() const noexcept
        {
            return m_rotation;
        }

        fvec3 drawable::get_scale() const noexcept
        {
            return m_scale;
        }

        void drawable::set_location(const fvec3 &location) noexcept
        {
            m_location = location;
        }

        void drawable::set_rotation(const fvec3 &rotation) noexcept
        {
            m_rotation = rotation;
        }

        void drawable::set_scale(const fvec3 &scale) noexcept
        {
            m_scale = scale;
        }
    } // namespace D3D
} // namespace deep
