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
    } // namespace D3D
} // namespace deep
