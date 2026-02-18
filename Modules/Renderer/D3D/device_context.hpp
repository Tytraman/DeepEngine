#ifndef DEEP_ENGINE_D3D_DEVICE_CONTEXT_HPP
#define DEEP_ENGINE_D3D_DEVICE_CONTEXT_HPP

#include "deep_d3d_export.h"

#include <DeepLib/context.hpp>
#include <DeepLib/memory/ref_counted.hpp>

#include "D3D/shader/vertex_shader.hpp"
#include "D3D/shader/pixel_shader.hpp"
#include "D3D/buffer/vertex_buffer.hpp"
#include "D3D/texture.hpp"
#include "D3D/sampler.hpp"

#include <d3d11.h>
#include <wrl.h>

namespace deep
{
    namespace D3D
    {
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11DeviceContext>;
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11RasterizerState>;

        enum class rasterizer_state
        {
            Unknown,
            CullBackSolid,
            CullBackWireframe,
            CullFrontSolid,
            CullFrontWireframe
        };

        class DEEP_D3D_API device_context
        {
          public:
            device_context()                                  = default;
            device_context(const device_context &)            = delete;
            device_context &operator=(const device_context &) = delete;

            ID3D11DeviceContext *get() const noexcept;
            ID3D11DeviceContext *const *get_address() const noexcept;

            void bind(const ref<vertex_shader> &shader) noexcept;
            void bind(const ref<pixel_shader> &shader) noexcept;
            void bind(const ref<vertex_buffer> &buffer) noexcept;
            void bind(const ref<texture> &tex) noexcept;
            void bind(const ref<sampler> &samp) noexcept;

            ref<vertex_shader> get_binded_vertex_shader() const noexcept;
            ref<pixel_shader> get_binded_pixel_shader() const noexcept;
            ref<texture> get_binded_texture() const noexcept;

            void set_rasterizer_state(rasterizer_state state) noexcept;

          private:
            Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_device_context;
            Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizer_state_cull_back_solid;
            Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizer_state_cull_back_wireframe;
            Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizer_state_cull_front_solid;
            Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizer_state_cull_front_wireframe;
            DEEP_REF(vertex_shader, m_binded_vertex_shader)
            DEEP_REF(pixel_shader, m_binded_pixel_shader)
            DEEP_REF(texture, m_binded_texture)
            rasterizer_state m_rasterizer_state;

          public:
            friend class graphics;
        };
    } // namespace D3D
} // namespace deep

#endif
