#include "drawable_factory.hpp"
#include "D3D/resource_factory.hpp"

#include <DeepLib/memory/memory.hpp>
#include <DeepLib/maths/mat.hpp>

namespace deep
{
    namespace D3D
    {
        ref<triangle> drawable_factory::create_triangle(const ref<ctx> &context, const ref<vertex_shader> &vs, const ref<pixel_shader> &ps, Microsoft::WRL::ComPtr<ID3D11Device> device, const device_context &dc) noexcept
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

            triangle *tr = mem::alloc_type<triangle>(context.get(), context);

            if (tr == nullptr)
            {
                return ref<triangle>();
            }

            fmat4 transformation = fmat4();

            tr->m_vertex_buffer   = resource_factory::create_vertex_buffer(context, vertices, sizeof(vertices), sizeof(vertex), device, dc);
            tr->m_constant_buffer = resource_factory::create_constant_buffer(context, transformation.get(), sizeof(transformation.data), device, dc);
            tr->m_vertex_shader   = vs;
            tr->m_pixel_shader    = ps;

            return ref<triangle>(context, tr);
        }
    } // namespace D3D
} // namespace deep
