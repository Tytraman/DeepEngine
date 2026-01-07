#include "graphics.hpp"
#include "error.hpp"
#include "resource_factory.hpp"
#include "D3D/shader/shader_factory.hpp"

#include <DeepLib/context.hpp>
#include <DeepLib/filesystem/filesystem.hpp>
#include <DeepLib/maths/mat.hpp>
#include <DeepLib/stream/file_stream.hpp>

#include <d3dcompiler.h>

namespace wrl = Microsoft::WRL;

namespace deep
{
    namespace D3D
    {
        graphics::graphics(const ref<ctx> &context, window_handle win) noexcept
                : object(context),
                  m_window_handle(win),
                  m_device(nullptr),
                  m_swap_chain(nullptr),
                  m_render_target_view(nullptr),
                  m_drawables(context)
        {
        }

        ref<graphics> graphics::create(const ref<ctx> &context, window &win) noexcept
        {
            context->out() << "Direct3D 11 initialization...";

            graphics *graph = mem::alloc_type<graphics>(context.get(), context, win.get_handle());

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
            sd.OutputWindow                       = win.get_handle();
            sd.Windowed                           = TRUE;
            sd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
            sd.Flags                              = 0;

            // Crée le dispositif de rendu, les front/back buffers et la chaîne de rendu.
            DEEP_DX_CHECK(D3D11CreateDeviceAndSwapChain(
                                  nullptr, // Laisse l'OS choisir l'adaptateur par défaut.
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  nullptr,
                                  D3D11_CREATE_DEVICE_DEBUG,
                                  nullptr,
                                  0,
                                  D3D11_SDK_VERSION,
                                  &sd,
                                  &graph->m_swap_chain,
                                  &graph->m_device,
                                  nullptr,
                                  &graph->m_device_context.m_device_context),
                          context, graph->m_device)

            wrl::ComPtr<ID3D11Resource> back_buffer;

            DEEP_DX_CHECK(graph->m_swap_chain->GetBuffer(0, __uuidof(ID3D11Resource), &back_buffer), context, graph->m_device)

            DEEP_DX_CHECK(graph->m_device->QueryInterface(__uuidof(ID3D11Debug), &graph->m_debug), context, graph->m_device)

            // Met un 'breakpoint' lorsqu'une erreur Direct3D est détectée.
            /*wrl::ComPtr<ID3D11InfoQueue> info_queue;
            if (SUCCEEDED(graph->m_debug.As(&info_queue)))
            {
                info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
                info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
            }*/

            DEEP_DX_CHECK(graph->m_device->CreateRenderTargetView(back_buffer.Get(), nullptr, &graph->m_render_target_view), context, graph->m_device)

            graph->m_device_context.get()->OMSetRenderTargets(1, graph->m_render_target_view.GetAddressOf(), nullptr);

            // Configuration du 'viewport'.
            D3D11_VIEWPORT vp = { 0 };
            vp.Width          = static_cast<float>(win.get_width());
            vp.Height         = static_cast<float>(win.get_height());
            vp.MinDepth       = 0;
            vp.MaxDepth       = 1;
            vp.TopLeftX       = 0;
            vp.TopLeftY       = 0;

            graph->m_device_context.get()->RSSetViewports(1, &vp);

            context->out() << " OK\r\n";

            return ref<graphics>(context, graph);
        }

        void graphics::clear_buffer(float r, float g, float b) noexcept
        {
            const float colors[] = { r, g, b, 1.0f };

            m_device_context.get()->ClearRenderTargetView(m_render_target_view.Get(), colors);
        }

        void graphics::add_drawable(const ref<drawable> &dr) noexcept
        {
            m_drawables.add(dr);
        }

        void graphics::draw_all() noexcept
        {
            usize count = m_drawables.count();
            usize index;

            for (index = 0; index < count; ++index)
            {
                m_drawables[index]->draw(m_device_context);
            }
        }

        void graphics::end_frame() noexcept
        {
            // Affiche l'image finale à l'utilisateur.
            m_swap_chain->Present(1, 0);

            print_debug_messages();
        }

        void graphics::print_debug_messages() noexcept
        {
            if (!m_debug)
            {
                return;
            }

            wrl::ComPtr<ID3D11InfoQueue> info_queue;
            if (FAILED(m_device.As(&info_queue)))
            {
                return;
            }

            UINT64 message_count = info_queue->GetNumStoredMessages();
            UINT64 index;

            for (index = 0; index < message_count; ++index)
            {
                SIZE_T message_size = 0;
                info_queue->GetMessage(index, nullptr, &message_size);

                D3D11_MESSAGE *message = mem::alloc<D3D11_MESSAGE>(get_context_ptr(), message_size);

                if (SUCCEEDED(info_queue->GetMessage(index, message, &message_size)))
                {
                    get_context()->out() << message->pDescription << "\r\n";
                }

                mem::dealloc(get_context_ptr(), message);
            }

            info_queue->ClearStoredMessages();

            if (message_count > 0)
            {
                DebugBreak();
            }
        }

        Microsoft::WRL::ComPtr<ID3D11Device> graphics::get_device() noexcept
        {
            return m_device;
        }

        device_context &graphics::get_device_context() noexcept
        {
            return m_device_context;
        }

        const device_context &graphics::get_device_context() const noexcept
        {
            return m_device_context;
        }
    } // namespace D3D
} // namespace deep
