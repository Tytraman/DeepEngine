#ifndef DEEP_ENGINE_D3D_DRAWABLE_FACTORY_HPP
#define DEEP_ENGINE_D3D_DRAWABLE_FACTORY_HPP

#include "deep_d3d_export.h"
#include "D3D/device_context.hpp"
#include "D3D/shader/vertex_shader.hpp"
#include "D3D/shader/pixel_shader.hpp"
#include "D3D/drawable/triangle.hpp"
#include "D3D/drawable/rectangle.hpp"
#include "D3D/drawable/cube.hpp"

#include <DeepLib/memory/ref_counted.hpp>

namespace deep
{
    namespace D3D
    {
        class DEEP_D3D_API drawable_factory
        {
          public:
            static ref<triangle> create_triangle(const ref<ctx> &context, const ref<vertex_shader> &vs, const ref<pixel_shader> &ps, Microsoft::WRL::ComPtr<ID3D11Device> device, const device_context &dc) noexcept;
            static ref<rectangle> create_rectangle(const ref<ctx> &context, const ref<vertex_shader> &vs, const ref<pixel_shader> &ps, Microsoft::WRL::ComPtr<ID3D11Device> device, const device_context &dc) noexcept;
            static ref<cube> create_cube(const ref<ctx> &context, const ref<vertex_shader> &vs, const ref<pixel_shader> &ps, Microsoft::WRL::ComPtr<ID3D11Device> device, const device_context &dc) noexcept;
        };
    } // namespace D3D
} // namespace deep

#endif
