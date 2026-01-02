#include "graphics.hpp"

#include <DeepLib/context.hpp>

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

            wrl::ComPtr<ID3D11Resource> back_buffer;

            graph->m_swap_chain->GetBuffer(0, __uuidof(ID3D11Resource), &back_buffer);

            graph->m_device->CreateRenderTargetView(back_buffer.Get(), nullptr, &graph->m_render_target_view);

            return ref<graphics>(context, graph);
        }

        void graphics::clear_buffer(float r, float g, float b) noexcept
        {
            const float colors[] = { r, g, b, 1.0f };

            m_device_context->ClearRenderTargetView(m_render_target_view.Get(), colors);
        }

        void graphics::draw_test_triangle() noexcept
        {
            const float data[] = {
                0.0f, 0.5f,
                0.5f, -0.5f,
                -0.5f, -0.5f
            };

            const UINT stride = sizeof(float) * 2;
            const UINT offset = 0;

            D3D11_BUFFER_DESC bd   = { 0 };
            bd.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
            bd.Usage               = D3D11_USAGE_DEFAULT;
            bd.CPUAccessFlags      = 0;
            bd.MiscFlags           = 0;
            bd.ByteWidth           = sizeof(data);
            bd.StructureByteStride = stride;

            D3D11_SUBRESOURCE_DATA sd = { 0 };
            sd.pSysMem                = data;

            wrl::ComPtr<ID3D11Buffer> vertex_buffer;

            m_device->CreateBuffer(&bd, &sd, &vertex_buffer);

            m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

            m_device_context->Draw(3, 0);
        }

        void graphics::end_frame() noexcept
        {
            // Affiche l'image finale à l'utilisateur.
            m_swap_chain->Present(1, 0);
        }
    } // namespace D3D
} // namespace deep
