#ifndef DEEP_ENGINE_D3D_GRAPHICS_HPP
#define DEEP_ENGINE_D3D_GRAPHICS_HPP

#include "deep_d3d_export.h"
#include <DeepCore/types.hpp>
#include <DeepLib/object.hpp>
#include <DeepLib/memory/memory.hpp>

#include <d3d11.h>
#include <wrl.h>

namespace deep
{
    namespace D3D
    {
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11Device>;
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<IDXGISwapChain>;
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11DeviceContext>;
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11RenderTargetView>;

        class DEEP_D3D_API graphics : public object
        {
          public:
            graphics()                            = delete;
            graphics(const graphics &)            = delete;
            graphics &operator=(const graphics &) = delete;
            ~graphics()                           = default;

            static ref<graphics> create(const ref<ctx> &context, window_handle win) noexcept;

            void clear_buffer(float r, float g, float b) noexcept;

            void draw_test_triangle() noexcept;

            void end_frame() noexcept;

          protected:
            graphics(const ref<ctx> &context, window_handle win) noexcept;

          private:
            window_handle m_window_handle;
            Microsoft::WRL::ComPtr<ID3D11Device> m_device;
            Microsoft::WRL::ComPtr<IDXGISwapChain> m_swap_chain;
            Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_device_context;
            Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_render_target_view;

          public:
            friend memory_manager;
        };
    } // namespace D3D
} // namespace deep

#endif
