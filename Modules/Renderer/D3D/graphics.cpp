#include "graphics.hpp"
#include "error.hpp"
#include "resource_factory.hpp"
#include "D3D/shader/shader_factory.hpp"
#include "D3D/shader/vertex_shader.hpp"

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
                  m_device_context(nullptr),
                  m_render_target_view(nullptr),
                  m_resources(context),
                  m_shaders(context)
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
                                  &graph->m_device_context),
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

            graph->m_device_context->OMSetRenderTargets(1, graph->m_render_target_view.GetAddressOf(), nullptr);

            // Configuration du 'viewport'.
            D3D11_VIEWPORT vp = { 0 };
            vp.Width          = static_cast<float>(win.get_width());
            vp.Height         = static_cast<float>(win.get_height());
            vp.MinDepth       = 0;
            vp.MaxDepth       = 1;
            vp.TopLeftX       = 0;
            vp.TopLeftY       = 0;

            graph->m_device_context->RSSetViewports(1, &vp);

            context->out() << " OK\r\n";

            return ref<graphics>(context, graph);
        }

        void graphics::clear_buffer(float r, float g, float b) noexcept
        {
            const float colors[] = { r, g, b, 1.0f };

            m_device_context->ClearRenderTargetView(m_render_target_view.Get(), colors);
        }

        void graphics::draw_test_triangle(float delta) noexcept
        {
            struct vertex
            {
                float x;
                float y;
                uint8 red;
                uint8 green;
                uint8 blue;
                uint8 alpha;
            };

            const vertex vertices[] = {
                { 0.0f, 1.0f, 255, 0, 0, 255 },
                { 0.5f, -0.5f, 0, 255, 0, 255 },
                { -0.5f, -0.5f, 0, 0, 255, 255 }
            };

            fmat4 transformation = fmat4::rotate_z(fmat4(), delta);

            ref<vertex_buffer> vb   = resource_factory::create_vertex_buffer(get_context(), vertices, sizeof(vertices), sizeof(vertex), m_device, m_device_context);
            ref<constant_buffer> cb = resource_factory::create_constant_buffer(get_context(), transformation.get(), sizeof(transformation.data), m_device, m_device_context);

            // wrl::ComPtr<ID3D11VertexShader> vertex_shader;
            wrl::ComPtr<ID3D11PixelShader> pixel_shader;
            wrl::ComPtr<ID3DBlob> blob;

            file_stream fs = file_stream(get_context(), DEEP_TEXT_NATIVE("test_triangle_vs.cso"), core_fs::file_mode::Open, core_fs::file_access::Read, core_fs::file_share::Read);
            fs.open();

            // Lit le contenu d'un fichier et le stock dans un 'blob'.
            // Il n'est pas obligatoire de récupérer un shader de cette manière mais je le garde pour le test.
            // D3DReadFileToBlob(DEEP_TEXT_NATIVE("test_triangle_vs.cso"), &blob);
            // Crée un 'vertex shader' depuis un fichier shader précompilé.
            // DEEP_DX_CHECK(m_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertex_shader), get_context(), m_device)
            // Rend le shader précédemment créé actif.
            // m_device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);

            const D3D11_INPUT_ELEMENT_DESC ied[] = {
                { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, sizeof(float) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            };

            ref<vertex_shader> vs = shader_factory::create_vertex_shader(get_context(), &fs, ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC), m_device);
            vs->bind(m_device_context);

            // wrl::ComPtr<ID3D11InputLayout> input_layout;

            // DEEP_DX_CHECK(m_device->CreateInputLayout(ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC), blob->GetBufferPointer(), blob->GetBufferSize(), &input_layout), get_context(), m_device)
            // m_device_context->IASetInputLayout(input_layout.Get());

            D3DReadFileToBlob(DEEP_TEXT_NATIVE("test_triangle_ps.cso"), &blob);
            DEEP_DX_CHECK(m_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixel_shader), get_context(), m_device)
            m_device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

            m_device_context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            vb->draw(m_device_context);
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
    } // namespace D3D
} // namespace deep
