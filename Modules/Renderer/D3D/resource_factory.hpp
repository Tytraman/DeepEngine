#ifndef DEEP_ENGINE_D3D_RESOURCE_FACTORY_HPP
#define DEEP_ENGINE_D3D_RESOURCE_FACTORY_HPP

#include "deep_d3d_export.h"
#include "D3D/vertex_buffer.hpp"

#include <DeepLib/memory/memory.hpp>
#include <DeepLib/memory/ref_counted.hpp>

#include <d3d11.h>
#include <wrl.h>

namespace deep
{
    namespace D3D
    {
        class DEEP_D3D_API resource_factory
        {
          public:
            static ref<vertex_buffer> create_vertex_buffer(const ref<ctx> &context, const void *data, uint32 bytes_size, uint32 stride, Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context) noexcept;
        };
    } // namespace D3D
} // namespace deep

#endif
