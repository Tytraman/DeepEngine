#include "resource_factory.hpp"
#include "error.hpp"

#include <DeepLib/context.hpp>

namespace deep
{
    namespace D3D
    {
        ref<vertex_buffer> resource_factory::create_vertex_buffer(const ref<ctx> &context, const void *data, uint32 bytes_size, uint32 stride, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept
        {
            vertex_buffer *vb = mem::alloc_type<vertex_buffer>(context.get(), context);

            if (vb == nullptr)
            {
                return ref<vertex_buffer>();
            }

            vb->m_offset = 0;
            vb->m_stride = stride;

            D3D11_BUFFER_DESC bd   = {};
            bd.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
            bd.Usage               = D3D11_USAGE_DEFAULT;
            bd.CPUAccessFlags      = 0;
            bd.MiscFlags           = 0;
            bd.ByteWidth           = bytes_size;
            bd.StructureByteStride = stride;

            D3D11_SUBRESOURCE_DATA sd = {};
            sd.pSysMem                = data;

            DEEP_DX_CHECK(device->CreateBuffer(&bd, &sd, &vb->m_buffer), context, device)

            return ref<vertex_buffer>(context, vb);
        }

        ref<constant_buffer> resource_factory::create_constant_buffer(const ref<ctx> &context, const void *data, uint32 bytes_size, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept
        {
            constant_buffer *cb = mem::alloc_type<constant_buffer>(context.get(), context);

            if (cb == nullptr)
            {
                return ref<constant_buffer>();
            }

            cb->m_bytes_size = bytes_size;

            D3D11_BUFFER_DESC bd   = {};
            bd.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
            bd.Usage               = D3D11_USAGE_DEFAULT;
            bd.CPUAccessFlags      = 0 /*D3D11_CPU_ACCESS_WRITE*/;
            bd.MiscFlags           = 0;
            bd.ByteWidth           = bytes_size;
            bd.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA sd = {};
            sd.pSysMem                = data;

            DEEP_DX_CHECK(device->CreateBuffer(&bd, &sd, &cb->m_buffer), context, device)

            return ref<constant_buffer>(context, cb);
        }

        ref<texture> resource_factory::create_texture(const ref<ctx> &context, const image &img, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept
        {
            texture *tex = mem::alloc_type<texture>(context.get(), context);

            if (tex == nullptr)
            {
                return ref<texture>();
            }

            D3D11_TEXTURE2D_DESC texture_desc = {};
            texture_desc.Width                = img.get_width();
            texture_desc.Height               = img.get_height();
            texture_desc.MipLevels            = 1;
            texture_desc.ArraySize            = 1;
            texture_desc.Format               = DXGI_FORMAT_B8G8R8A8_UNORM;
            texture_desc.SampleDesc.Count     = 1;
            texture_desc.SampleDesc.Quality   = 0;
            texture_desc.Usage                = D3D11_USAGE_DEFAULT;
            texture_desc.BindFlags            = D3D11_BIND_SHADER_RESOURCE;
            texture_desc.CPUAccessFlags       = 0;
            texture_desc.MiscFlags            = 0;

            D3D11_SUBRESOURCE_DATA sd = { 0 };
            sd.pSysMem                = *img;
            sd.SysMemPitch            = static_cast<UINT>(img.get_row_bytes());

            // La texture sera ensuite liée à une 'Shader Resource View'.
            Microsoft::WRL::ComPtr<ID3D11Texture2D> d3d_texture;

            DEEP_DX_CHECK(device->CreateTexture2D(&texture_desc, &sd, &d3d_texture), context, device)

            D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
            srv_desc.Format                          = texture_desc.Format;
            srv_desc.ViewDimension                   = D3D11_SRV_DIMENSION_TEXTURE2D;
            srv_desc.Texture2D.MostDetailedMip       = 0;
            srv_desc.Texture2D.MipLevels             = 1;

            DEEP_DX_CHECK(device->CreateShaderResourceView(d3d_texture.Get(), &srv_desc, &tex->m_texture_view), context, device)

            return ref<texture>(context, tex);
        }

        ref<sampler> resource_factory::create_sampler(const ref<ctx> &context, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept
        {
            sampler *s = mem::alloc_type<sampler>(context.get(), context);

            if (s == nullptr)
            {
                return ref<sampler>();
            }

            D3D11_SAMPLER_DESC sampler_desc = {};
            sampler_desc.Filter             = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            sampler_desc.AddressU           = D3D11_TEXTURE_ADDRESS_WRAP;
            sampler_desc.AddressV           = D3D11_TEXTURE_ADDRESS_WRAP;
            sampler_desc.AddressW           = D3D11_TEXTURE_ADDRESS_WRAP;

            DEEP_DX_CHECK(device->CreateSamplerState(&sampler_desc, &s->m_sampler_state), context, device)

            return ref<sampler>(context, s);
        }
    } // namespace D3D
} // namespace deep
