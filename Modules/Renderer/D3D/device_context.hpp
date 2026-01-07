#ifndef DEEP_ENGINE_D3D_DEVICE_CONTEXT_HPP
#define DEEP_ENGINE_D3D_DEVICE_CONTEXT_HPP

#include "deep_d3d_export.h"
#include "D3D/shader/vertex_shader.hpp"
#include "D3D/shader/pixel_shader.hpp"

#include <DeepLib/context.hpp>
#include <DeepLib/memory/ref_counted.hpp>

#include <d3d11.h>
#include <wrl.h>

namespace deep
{
    namespace D3D
    {
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11DeviceContext>;

        template class DEEP_D3D_API ref<vertex_shader>;
        template class DEEP_D3D_API ref<pixel_shader>;

        class DEEP_D3D_API device_context
        {
          public:
            device_context()                                  = default;
            device_context(const device_context &)            = delete;
            device_context &operator=(const device_context &) = delete;

            ID3D11DeviceContext *get() const noexcept;
            ID3D11DeviceContext *const *get_address() const noexcept;

            void bind_shader(const ref<vertex_shader> &shader) noexcept;
            void bind_shader(const ref<pixel_shader> &shader) noexcept;

            ref<vertex_shader> get_binded_vertex_shader() const noexcept;
            ref<pixel_shader> get_binded_pixel_shader() const noexcept;

          private:
            Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_device_context;
            ref<vertex_shader> m_binded_vertex_shader;
            ref<pixel_shader> m_binded_pixel_shader;

          public:
            friend class graphics;
        };
    } // namespace D3D
} // namespace deep

#endif
