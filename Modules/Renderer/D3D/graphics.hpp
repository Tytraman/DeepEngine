#ifndef DEEP_ENGINE_D3D_GRAPHICS_HPP
#define DEEP_ENGINE_D3D_GRAPHICS_HPP

#include "deep_d3d_export.h"
#include <DeepCore/types.hpp>
#include <DeepLib/object.hpp>
#include <DeepLib/memory/memory.hpp>
#include <DeepLib/collection/list.hpp>
#include <DeepLib/window/window.hpp>

#include "D3D/device_context.hpp"
#include "D3D/drawable/drawable.hpp"
#include "D3D/resource.hpp"
#include "D3D/shader/shader.hpp"

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
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11Debug>;

        template class DEEP_D3D_API list<ref<drawable>>;

        class DEEP_D3D_API graphics : public object
        {
          public:
            graphics()                            = delete;
            graphics(const graphics &)            = delete;
            graphics &operator=(const graphics &) = delete;
            ~graphics()                           = default;

            static ref<graphics> create(const ref<ctx> &context, window &win) noexcept;

            void clear_buffer(float r, float g, float b) noexcept;

            void add_drawable(const ref<drawable> &dr) noexcept;

            void draw_all() noexcept;

            void end_frame() noexcept;
            void print_debug_messages() noexcept;

            Microsoft::WRL::ComPtr<ID3D11Device> get_device() noexcept;

            device_context &get_device_context() noexcept;
            const device_context &get_device_context() const noexcept;

          protected:
            graphics(const ref<ctx> &context, window_handle win) noexcept;

          private:
            window_handle m_window_handle;
            // Le 'device' permet la création de ressources Direct3D.
            Microsoft::WRL::ComPtr<ID3D11Device> m_device;
            // La 'device context' permet l'utilisation des ressources créées avec le 'device'.
            device_context m_device_context;
            Microsoft::WRL::ComPtr<IDXGISwapChain> m_swap_chain;
            Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_render_target_view;
            Microsoft::WRL::ComPtr<ID3D11Debug> m_debug;

            list<ref<drawable>> m_drawables;

          public:
            friend memory_manager;
        };
    } // namespace D3D
} // namespace deep

#endif
