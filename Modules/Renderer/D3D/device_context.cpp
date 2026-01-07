#include "device_context.hpp"

namespace deep
{
    namespace D3D
    {
        ID3D11DeviceContext *device_context::get() const noexcept
        {
            return m_device_context.Get();
        }

        ID3D11DeviceContext *const *device_context::get_address() const noexcept
        {
            return m_device_context.GetAddressOf();
        }

        void device_context::bind_shader(const ref<vertex_shader> &shader) noexcept
        {
            if (shader == m_binded_vertex_shader)
            {
                return;
            }

            m_device_context->VSSetShader(shader->get(), nullptr, 0);
            m_device_context->IASetInputLayout(shader->get_input_layout());

            m_binded_vertex_shader = shader;
        }

        void device_context::bind_shader(const ref<pixel_shader> &shader) noexcept
        {
            if (shader == m_binded_pixel_shader)
            {
                return;
            }

            m_device_context->PSSetShader(shader->get(), nullptr, 0);

            m_binded_pixel_shader = shader;
        }

        ref<vertex_shader> device_context::get_binded_vertex_shader() const noexcept
        {
            return m_binded_vertex_shader;
        }

        ref<pixel_shader> device_context::get_binded_pixel_shader() const noexcept
        {
            return m_binded_pixel_shader;
        }
    } // namespace D3D
} // namespace deep
