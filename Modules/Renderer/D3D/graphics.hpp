#ifndef DEEP_ENGINE_D3D_GRAPHICS_HPP
#define DEEP_ENGINE_D3D_GRAPHICS_HPP

#include "deep_d3d_export.h"
#include <DeepCore/types.hpp>
#include <DeepLib/object.hpp>
#include <DeepLib/memory/memory.hpp>
#include <DeepLib/collection/list.hpp>
#include <DeepLib/window/window.hpp>
#include <DeepLib/maths/vec.hpp>

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
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11DepthStencilView>;
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11Debug>;

        template class DEEP_D3D_API list<ref<drawable>>;

        class DEEP_D3D_API graphics : public object
        {
          public:
            using post_init_callback = void (*)(graphics *graph);

          public:
            graphics()                            = delete;
            graphics(const graphics &)            = delete;
            graphics &operator=(const graphics &) = delete;
            ~graphics()                           = default;

            static ref<graphics> create(const ref<ctx> &context, window &win, const fvec4 &background_color, const fvec3 &initial_location, post_init_callback post_init = nullptr) noexcept;

            void clear_buffer() noexcept;

            void add_drawable(const ref<drawable> &dr) noexcept;

            void draw_all(const fmat4 &projection, const fmat4 &view) noexcept;

            void end_frame() noexcept;
            void print_debug_messages() noexcept;

            fvec4 get_background_color() const noexcept;
            void set_background_color(const fvec4 &color) noexcept;

            Microsoft::WRL::ComPtr<ID3D11Device> get_device() noexcept;

            device_context &get_device_context() noexcept;
            const device_context &get_device_context() const noexcept;

            ref<constant_buffer> get_per_frame_buffer() noexcept;

          protected:
            graphics(const ref<ctx> &context, window_handle win) noexcept;

          private:
            DEEP_FVEC4 m_background_color;

            window_handle m_window_handle;
            // Le 'device' permet la création de ressources Direct3D.
            Microsoft::WRL::ComPtr<ID3D11Device> m_device;
            // La 'device context' permet l'utilisation des ressources créées avec le 'device'.
            device_context m_device_context;
            Microsoft::WRL::ComPtr<IDXGISwapChain> m_swap_chain;
            Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_render_target_view;
            Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depth_stencil_view;
            Microsoft::WRL::ComPtr<ID3D11Debug> m_debug;

            ref<constant_buffer> m_per_frame_buffer;

            list<ref<drawable>> m_drawables;

          public:
            friend memory_manager;
        };
    } // namespace D3D
} // namespace deep

#endif
