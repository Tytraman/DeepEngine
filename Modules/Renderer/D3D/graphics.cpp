#include "graphics.hpp"
#include "error.hpp"
#include "resource_factory.hpp"
#include "D3D/shader/shader_factory.hpp"
#include "D3D/buffer/per_frame_buffer.hpp"

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
                  m_background_color(),
                  m_window_handle(win),
                  m_device(nullptr),
                  m_swap_chain(nullptr),
                  m_render_target_view(nullptr),
                  m_drawables(context)
        {
        }

        ref<graphics> graphics::create(const ref<ctx> &context, window &win, const fvec4 &background_color, const fvec3 &initial_location, post_init_callback post_init) noexcept
        {
            context->out() << "Direct3D 11 initialization...";

            int32 width  = win.get_width();
            int32 height = win.get_height();

            graphics *graph = mem::alloc_type<graphics>(context.get(), context, win.get_handle());

            if (graph == nullptr)
            {
                return ref<graphics>();
            }

            graph->m_background_color = background_color;

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

            DEEP_DX_CHECK(graph->m_device->CreateRenderTargetView(back_buffer.Get(), nullptr, &graph->m_render_target_view), context, graph->m_device)

            D3D11_DEPTH_STENCIL_DESC ds_desc = { 0 };
            ds_desc.DepthEnable              = TRUE;
            ds_desc.DepthWriteMask           = D3D11_DEPTH_WRITE_MASK_ALL;
            ds_desc.DepthFunc                = D3D11_COMPARISON_LESS;

            wrl::ComPtr<ID3D11DepthStencilState> ds_state;

            DEEP_DX_CHECK(graph->m_device->CreateDepthStencilState(&ds_desc, &ds_state), context, graph->m_device)

            graph->m_device_context.get()->OMSetDepthStencilState(ds_state.Get(), 1);

            D3D11_TEXTURE2D_DESC depth_stencil_texture_desc = { 0 };
            depth_stencil_texture_desc.Width                = static_cast<UINT>(width);
            depth_stencil_texture_desc.Height               = static_cast<UINT>(height);
            depth_stencil_texture_desc.MipLevels            = 1;
            depth_stencil_texture_desc.ArraySize            = 1;
            depth_stencil_texture_desc.Format               = DXGI_FORMAT_D32_FLOAT;
            depth_stencil_texture_desc.SampleDesc.Count     = 1;
            depth_stencil_texture_desc.SampleDesc.Quality   = 0;
            depth_stencil_texture_desc.Usage                = D3D11_USAGE_DEFAULT;
            depth_stencil_texture_desc.BindFlags            = D3D11_BIND_DEPTH_STENCIL;

            wrl::ComPtr<ID3D11Texture2D> depth_stencil_texture;

            DEEP_DX_CHECK(graph->m_device->CreateTexture2D(&depth_stencil_texture_desc, nullptr, &depth_stencil_texture), context, graph->m_device)

            D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
            dsv_desc.Format                        = DXGI_FORMAT_D32_FLOAT;
            dsv_desc.ViewDimension                 = D3D11_DSV_DIMENSION_TEXTURE2D;
            dsv_desc.Texture2D.MipSlice            = 0;

            DEEP_DX_CHECK(graph->m_device->CreateDepthStencilView(depth_stencil_texture.Get(), &dsv_desc, &graph->m_depth_stencil_view), context, graph->m_device)

            graph->m_device_context.get()->OMSetRenderTargets(1, graph->m_render_target_view.GetAddressOf(), graph->m_depth_stencil_view.Get());

            // Configuration du 'viewport'.
            D3D11_VIEWPORT vp = { 0 };
            vp.Width          = static_cast<FLOAT>(width);
            vp.Height         = static_cast<FLOAT>(height);
            vp.MinDepth       = 0;
            vp.MaxDepth       = 1;
            vp.TopLeftX       = 0;
            vp.TopLeftY       = 0;

            graph->m_device_context.get()->RSSetViewports(1, &vp);

            D3D11_RASTERIZER_DESC raster_desc = {};
            raster_desc.FillMode              = D3D11_FILL_SOLID;
            raster_desc.CullMode              = D3D11_CULL_BACK;
            raster_desc.FrontCounterClockwise = false;

            graph->m_device->CreateRasterizerState(&raster_desc, &graph->m_device_context.m_rasterizer_state_cull_back_solid);

            raster_desc.FillMode = D3D11_FILL_WIREFRAME;
            graph->m_device->CreateRasterizerState(&raster_desc, &graph->m_device_context.m_rasterizer_state_cull_back_wireframe);

            raster_desc.FillMode = D3D11_FILL_SOLID;
            raster_desc.CullMode = D3D11_CULL_FRONT;
            graph->m_device->CreateRasterizerState(&raster_desc, &graph->m_device_context.m_rasterizer_state_cull_front_solid);

            raster_desc.FillMode = D3D11_FILL_WIREFRAME;
            graph->m_device->CreateRasterizerState(&raster_desc, &graph->m_device_context.m_rasterizer_state_cull_front_wireframe);

            graph->m_device_context.set_rasterizer_state(rasterizer_state::CullBackSolid);

            fmat4 view;
            view = fmat4::translate(view, initial_location);

            fmat4 projection = fmat4::d3d_perspective_lh(1.0f, 3.0f / 4.0f, 0.5f, 10.0f);

            const per_frame_buffer pfb = {
                view,
                projection
            };

            graph->m_per_frame_buffer = resource_factory::create_constant_buffer(context, &pfb, sizeof(pfb), graph->m_device);

            if (post_init != nullptr)
            {
                post_init(graph);
            }

            context->out() << " OK\r\n";

            return ref<graphics>(context, graph);
        }

        void graphics::clear_buffer() noexcept
        {
            const float color[] = {
                m_background_color.x,
                m_background_color.y,
                m_background_color.z,
                m_background_color.w
            };

            m_device_context.get()->ClearRenderTargetView(m_render_target_view.Get(), color);
            m_device_context.get()->ClearDepthStencilView(m_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
        }

        void graphics::add_drawable(const ref<drawable> &dr) noexcept
        {
            m_drawables.add(dr);
        }

        void graphics::draw_all(const fmat4 &projection, const fmat4 &view) noexcept
        {
            usize count = m_drawables.count();
            usize index;

            m_device_context.get()->VSSetConstantBuffers(0, 1, m_per_frame_buffer->get_address());
            m_device_context.get()->PSSetConstantBuffers(0, 1, m_per_frame_buffer->get_address());

            const fmat4 view_projection = projection * view;

            for (index = 0; index < count; ++index)
            {
                if (m_drawables[index].is_valid())
                {
                    m_drawables[index]->draw(m_device_context, view_projection);
                }
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

        ref<constant_buffer> graphics::get_per_frame_buffer() noexcept
        {
            return m_per_frame_buffer;
        }
    } // namespace D3D
} // namespace deep
