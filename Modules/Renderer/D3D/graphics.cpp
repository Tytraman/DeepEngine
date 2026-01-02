#include "graphics.hpp"

#include <DeepLib/context.hpp>

namespace deep
{
    namespace D3D
    {
        graphics::graphics(const ref<ctx> &context, window_handle win) noexcept
                : object(context),
                  m_window_handle(win),
                  m_device(nullptr),
                  m_swap_chain(nullptr),
                  m_device_context(nullptr),
                  m_render_target_view(nullptr)
        {
        }

        ref<graphics> graphics::create(const ref<ctx> &context, window_handle win) noexcept
        {
            graphics *graph = mem::alloc_type<graphics>(context.get(), context, win);

            DXGI_SWAP_CHAIN_DESC sd               = { 0 };
            sd.BufferDesc.Width                   = 0;
            sd.BufferDesc.Height                  = 0;
            sd.BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM;
            sd.BufferDesc.RefreshRate.Numerator   = 0;
            sd.BufferDesc.RefreshRate.Denominator = 0;
            sd.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
            sd.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            sd.SampleDesc.Count                   = 1;
            sd.SampleDesc.Quality                 = 0;
            sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.BufferCount                        = 1;
            sd.OutputWindow                       = win;
            sd.Windowed                           = TRUE;
            sd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
            sd.Flags                              = 0;

            // Crée le dispositif de rendu, les front/back buffers et la chaîne de rendu.
            D3D11CreateDeviceAndSwapChain(
                    nullptr, // Laisse l'OS choisir l'adaptateur par défaut.
                    D3D_DRIVER_TYPE_HARDWARE,
                    nullptr,
                    0,
                    nullptr,
                    0,
                    D3D11_SDK_VERSION,
                    &sd,
                    &graph->m_swap_chain,
                    &graph->m_device,
                    nullptr,
                    &graph->m_device_context);

            ID3D11Resource *back_buffer = nullptr;

            graph->m_swap_chain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void **>(&back_buffer));

            graph->m_device->CreateRenderTargetView(back_buffer, nullptr, &graph->m_render_target_view);

            back_buffer->Release();

            return ref<graphics>(context, graph);
        }

        void graphics::end_frame() noexcept
        {
            // Affiche l'image finale à l'utilisateur.
            m_swap_chain->Present(1, 0);
        }

        void graphics::destroy() noexcept
        {
            if (m_render_target_view != nullptr)
            {
                m_render_target_view->Release();
            }

            if (m_device_context != nullptr)
            {
                m_device_context->Release();
            }

            if (m_swap_chain != nullptr)
            {
                m_swap_chain->Release();
            }

            if (m_device != nullptr)
            {
                m_device->Release();
            }
        }
    } // namespace D3D
} // namespace deep
