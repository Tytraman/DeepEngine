#ifndef DEEP_ENGINE_D3D_SHADER_FACTORY_HPP
#define DEEP_ENGINE_D3D_SHADER_FACTORY_HPP

#include "deep_d3d_export.h"
#include "D3D/shader/vertex_shader.hpp"
#include "D3D/shader/pixel_shader.hpp"

#include <DeepLib/memory/ref_counted.hpp>
#include <DeepLib/stream/stream.hpp>

#include <d3d11.h>
#include <wrl.h>

namespace deep
{
    namespace D3D
    {
        class DEEP_D3D_API shader_factory
        {
          public:
            static ref<vertex_shader> create_vertex_shader(const ref<ctx> &context, stream *input, const D3D11_INPUT_ELEMENT_DESC *ied, uint32 ied_count, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept;
            static ref<pixel_shader> create_pixel_shader(const ref<ctx> &context, stream *input, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept;
        };
    } // namespace D3D
} // namespace deep

#endif
