#ifndef DEEP_ENGINE_D3D_RESOURCE_FACTORY_HPP
#define DEEP_ENGINE_D3D_RESOURCE_FACTORY_HPP

#include "deep_d3d_export.h"
#include "D3D/device_context.hpp"
#include "D3D/buffer/vertex_buffer.hpp"
#include "D3D/buffer/constant_buffer.hpp"

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
            static ref<vertex_buffer> create_vertex_buffer(const ref<ctx> &context, const void *data, uint32 bytes_size, uint32 stride, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept;
            static ref<constant_buffer> create_constant_buffer(const ref<ctx> &context, const void *data, uint32 bytes_size, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept;
        };
    } // namespace D3D
} // namespace deep

#endif
