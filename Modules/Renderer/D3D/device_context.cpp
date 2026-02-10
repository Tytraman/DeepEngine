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

        void device_context::bind(const ref<vertex_shader> &shader) noexcept
        {
            if (!shader.is_valid() || shader == m_binded_vertex_shader)
            {
                return;
            }

            m_device_context->VSSetShader(shader->get(), nullptr, 0);
            m_device_context->IASetInputLayout(shader->get_input_layout());

            m_binded_vertex_shader = shader;
        }

        void device_context::bind(const ref<pixel_shader> &shader) noexcept
        {
            if (!shader.is_valid() || shader == m_binded_pixel_shader)
            {
                return;
            }

            m_device_context->PSSetShader(shader->get(), nullptr, 0);

            m_binded_pixel_shader = shader;
        }

        void device_context::bind(const ref<vertex_buffer> &buffer) noexcept
        {
            if (!buffer.is_valid())
            {
                return;
            }

            m_device_context->IASetVertexBuffers(0, 1, buffer->get_address(), &buffer->m_stride, &buffer->m_offset);
        }

        void device_context::bind(const ref<texture> &tex) noexcept
        {
            if (!tex.is_valid())
            {
                return;
            }

            m_device_context->PSSetShaderResources(0, 1, tex->m_texture_view.GetAddressOf());
        }

        void device_context::bind(const ref<sampler> &samp) noexcept
        {
            if (!samp.is_valid())
            {
                return;
            }

            m_device_context->PSSetSamplers(0, 1, samp->m_sampler_state.GetAddressOf());
        }

        ref<vertex_shader> device_context::get_binded_vertex_shader() const noexcept
        {
            return m_binded_vertex_shader;
        }

        ref<pixel_shader> device_context::get_binded_pixel_shader() const noexcept
        {
            return m_binded_pixel_shader;
        }

        ref<texture> device_context::get_binded_texture() const noexcept
        {
            return m_binded_texture;
        }

        void device_context::set_rasterizer_state(rasterizer_state state) noexcept
        {
            switch (state)
            {
                default:
                    break;
                case rasterizer_state::CullBackSolid:
                {
                    m_device_context->RSSetState(m_rasterizer_state_cull_back_solid.Get());
                    m_rasterizer_state = rasterizer_state::CullBackSolid;
                }
                break;
                case rasterizer_state::CullBackWireframe:
                {
                    m_device_context->RSSetState(m_rasterizer_state_cull_back_wireframe.Get());
                    m_rasterizer_state = rasterizer_state::CullBackWireframe;
                }
                break;
                case rasterizer_state::CullFrontSolid:
                {
                    m_device_context->RSSetState(m_rasterizer_state_cull_front_solid.Get());
                    m_rasterizer_state = rasterizer_state::CullFrontSolid;
                }
                break;
                case rasterizer_state::CullFrontWireframe:
                {
                    m_device_context->RSSetState(m_rasterizer_state_cull_front_wireframe.Get());
                    m_rasterizer_state = rasterizer_state::CullFrontWireframe;
                }
                break;
            }
        }
    } // namespace D3D
} // namespace deep
