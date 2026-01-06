#include "resource_factory.hpp"
#include "error.hpp"

#include <DeepLib/context.hpp>

namespace deep
{
    namespace D3D
    {
        ref<vertex_buffer> resource_factory::create_vertex_buffer(const ref<ctx> &context, const void *data, uint32 bytes_size, uint32 stride, Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context) noexcept
        {
            vertex_buffer *vb = mem::alloc_type<vertex_buffer>(context.get(), context);

            if (vb == nullptr)
            {
                return ref<vertex_buffer>();
            }

            const UINT offset = 0;

            D3D11_BUFFER_DESC bd   = { 0 };
            bd.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
            bd.Usage               = D3D11_USAGE_DEFAULT;
            bd.CPUAccessFlags      = 0;
            bd.MiscFlags           = 0;
            bd.ByteWidth           = bytes_size;
            bd.StructureByteStride = stride;

            D3D11_SUBRESOURCE_DATA sd = { 0 };
            sd.pSysMem                = data;

            DEEP_DX_CHECK(device->CreateBuffer(&bd, &sd, &vb->m_buffer), context, device)

            device_context->IASetVertexBuffers(0, 1, vb->m_buffer.GetAddressOf(), &stride, &offset);

            return ref<vertex_buffer>(context, vb);
        }
    } // namespace D3D
} // namespace deep
