#include "drawable_factory.hpp"
#include "D3D/resource_factory.hpp"

#include <DeepLib/memory/memory.hpp>
#include <DeepLib/maths/mat.hpp>
#include <DeepLib/maths/vec.hpp>
#include <DeepLib/maths/math.hpp>

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
                { 0.0f, 0.5f, 255, 255, 255, 255 },
                { 0.5f, -0.5f, 255, 255, 255, 255 },
                { -0.5f, -0.5f, 255, 255, 255, 255 }
            };

            triangle *tr = mem::alloc_type<triangle>(context.get(), context);

            if (tr == nullptr)
            {
                return ref<triangle>();
            }

            fmat4 transformation = fmat4();

            tr->m_vertex_buffer   = resource_factory::create_vertex_buffer(context, vertices, sizeof(vertices), sizeof(vertex), device, dc);
            tr->m_constant_buffer = resource_factory::create_constant_buffer(context, transformation.get(), sizeof(transformation.rows), device, dc);
            tr->m_vertex_shader   = vs;
            tr->m_pixel_shader    = ps;

            return ref<triangle>(context, tr);
        }

        ref<rectangle> drawable_factory::create_rectangle(const ref<ctx> &context, const ref<vertex_shader> &vs, const ref<pixel_shader> &ps, Microsoft::WRL::ComPtr<ID3D11Device> device, const device_context &dc) noexcept
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
                { -0.5f, 0.5f, 255, 255, 255, 255 },
                { 0.5f, -0.5f, 255, 255, 255, 255 },
                { -0.5f, -0.5f, 255, 255, 255, 255 },
                { -0.5f, 0.5f, 255, 255, 255, 255 },
                { 0.5f, 0.5f, 255, 255, 255, 255 },
                { 0.5f, -0.5f, 255, 255, 255, 255 }
            };

            rectangle *rect = mem::alloc_type<rectangle>(context.get(), context);

            if (rect == nullptr)
            {
                return ref<rectangle>();
            }

            // INFO: juste pour le test.
            fmat4 model = fmat4();
            model       = fmat4::translate(model, fvec3(0.25f, 0.0f, 0.0f));

            rect->m_vertex_buffer   = resource_factory::create_vertex_buffer(context, vertices, sizeof(vertices), sizeof(vertex), device, dc);
            rect->m_constant_buffer = resource_factory::create_constant_buffer(context, model.get(), sizeof(model.rows), device, dc);
            rect->m_vertex_shader   = vs;
            rect->m_pixel_shader    = ps;

            return ref<rectangle>(context, rect);
        }

        ref<cube> drawable_factory::create_cube(const ref<ctx> &context, const ref<vertex_shader> &vs, const ref<pixel_shader> &ps, Microsoft::WRL::ComPtr<ID3D11Device> device, const device_context &dc) noexcept
        {
            struct vertex
            {
                struct
                {
                    float x;
                    float y;
                    float z;
                } position;
                struct
                {
                    uint8 red;
                    uint8 green;
                    uint8 blue;
                    uint8 alpha;
                } color;
            };

            const vertex vertices[] = {
                // Avant - Rouge
                { -1.0f, 1.0f, -1.0f, 255, 0, 0, 255 },
                { 1.0f, 1.0f, -1.0f, 255, 0, 0, 255 },
                { 1.0f, -1.0f, -1.0f, 255, 0, 0, 255 },
                { -1.0f, 1.0f, -1.0f, 255, 0, 0, 255 },
                { 1.0f, -1.0f, -1.0f, 255, 0, 0, 255 },
                { -1.0f, -1.0f, -1.0f, 255, 0, 0, 255 },

                // Droite - Vert
                { 1.0f, 1.0f, -1.0f, 0, 255, 0, 255 },
                { 1.0f, 1.0f, 1.0f, 0, 255, 0, 255 },
                { 1.0f, -1.0f, 1.0f, 0, 255, 0, 255 },
                { 1.0f, 1.0f, -1.0f, 0, 255, 0, 255 },
                { 1.0f, -1.0f, 1.0f, 0, 255, 0, 255 },
                { 1.0f, -1.0f, -1.0f, 0, 255, 0, 255 },

                // Arrière - Blanc
                { -1.0f, 1.0f, 1.0f, 255, 255, 255, 255 },
                { 1.0f, -1.0f, 1.0f, 255, 255, 255, 255 },
                { 1.0f, 1.0f, 1.0f, 255, 255, 255, 255 },
                { -1.0f, 1.0f, 1.0f, 255, 255, 255, 255 },
                { -1.0f, -1.0f, 1.0f, 255, 255, 255, 255 },
                { 1.0f, -1.0f, 1.0f, 255, 255, 255, 255 },

                // Gauche - Jaune
                { -1.0f, 1.0f, 1.0f, 255, 255, 0, 255 },
                { -1.0f, 1.0f, -1.0f, 255, 255, 0, 255 },
                { -1.0f, -1.0f, -1.0f, 255, 255, 0, 255 },
                { -1.0f, 1.0f, 1.0f, 255, 255, 0, 255 },
                { -1.0f, -1.0f, -1.0f, 255, 255, 0, 255 },
                { -1.0f, -1.0f, 1.0f, 255, 255, 0, 255 },

                // Bas - Aqua
                { -1.0f, -1.0f, -1.0f, 0, 255, 255, 255 },
                { 1.0f, -1.0f, -1.0f, 0, 255, 255, 255 },
                { 1.0f, -1.0f, 1.0f, 0, 255, 255, 255 },
                { 1.0f, -1.0f, 1.0f, 0, 255, 255, 255 },
                { -1.0f, -1.0f, 1.0f, 0, 255, 255, 255 },
                { -1.0f, -1.0f, -1.0f, 0, 255, 255, 255 },

                // Haut - Bleu
                { -1.0f, 1.0f, 1.0f, 0, 0, 255, 255 },
                { 1.0f, 1.0f, 1.0f, 0, 0, 255, 255 },
                { 1.0f, 1.0f, -1.0f, 0, 0, 255, 255 },
                { -1.0f, 1.0f, 1.0f, 0, 0, 255, 255 },
                { 1.0f, 1.0f, -1.0f, 0, 0, 255, 255 },
                { -1.0f, 1.0f, -1.0f, 0, 0, 255, 255 }

            };

            cube *c = mem::alloc_type<cube>(context.get(), context);

            if (c == nullptr)
            {
                return ref<cube>();
            }

            // INFO: juste pour le test.
            fmat4 model = fmat4();
            model       = fmat4::translate(model, fvec3(0.0f, 0.0f, 4.0f));
            model       = fmat4::rotate_x(model, -20.0f);
            model       = fmat4::rotate_y(model, 45.0f);

            fmat4 projection = fmat4::d3d_perspective_lh(1.0f, 3.0f / 4.0f, 0.5f, 10.0f);

            fmat4 m = projection * model;

            c->m_vertex_buffer   = resource_factory::create_vertex_buffer(context, vertices, sizeof(vertices), sizeof(vertex), device, dc);
            c->m_constant_buffer = resource_factory::create_constant_buffer(context, m.get(), sizeof(m.rows), device, dc);
            c->m_vertex_shader   = vs;
            c->m_pixel_shader    = ps;

            return ref<cube>(context, c);
        }
    } // namespace D3D
} // namespace deep
