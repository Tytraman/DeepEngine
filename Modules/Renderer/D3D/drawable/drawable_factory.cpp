#include "drawable_factory.hpp"
#include "D3D/resource_factory.hpp"
#include "D3D/buffer/per_object_buffer.hpp"

#include <DeepLib/memory/memory.hpp>
#include <DeepLib/maths/mat.hpp>
#include <DeepLib/maths/vec.hpp>
#include <DeepLib/maths/math.hpp>

namespace deep
{
    namespace D3D
    {
        ref<triangle> drawable_factory::create_triangle(const ref<ctx> &context, const ref<vertex_shader> &vs, const ref<pixel_shader> &ps, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept
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

            tr->m_vertex_buffer    = resource_factory::create_vertex_buffer(context, vertices, sizeof(vertices), sizeof(vertex), device);
            tr->m_transform_buffer = resource_factory::create_constant_buffer(context, transformation.get(), sizeof(transformation.rows), device);
            tr->m_vertex_shader    = vs;
            tr->m_pixel_shader     = ps;

            return ref<triangle>(context, tr);
        }

        ref<rectangle> drawable_factory::create_rectangle(const ref<ctx> &context, const ref<vertex_shader> &vs, const ref<pixel_shader> &ps, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept
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

            rect->m_vertex_buffer    = resource_factory::create_vertex_buffer(context, vertices, sizeof(vertices), sizeof(vertex), device);
            rect->m_transform_buffer = resource_factory::create_constant_buffer(context, model.get(), sizeof(model.rows), device);
            rect->m_vertex_shader    = vs;
            rect->m_pixel_shader     = ps;

            return ref<rectangle>(context, rect);
        }

        ref<cube> drawable_factory::create_cube(const ref<ctx> &context, const ref<vertex_shader> &vs, const ref<pixel_shader> &ps, const fvec3 &position, const fvec3 &rotation, const fvec3 &scale, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept
        {
            struct vertex
            {
                struct
                {
                    float x;
                    float y;
                    float z;
                } position;
            };

            struct color_buffer
            {
                struct
                {
                    float red;
                    float green;
                    float blue;
                    float alpha;
                } face_color[6];
            };

            const vertex vertices[] = {
                // Avant
                { -1.0f, 1.0f, -1.0f },
                { 1.0f, 1.0f, -1.0f },
                { 1.0f, -1.0f, -1.0f },
                { -1.0f, 1.0f, -1.0f },
                { 1.0f, -1.0f, -1.0f },
                { -1.0f, -1.0f, -1.0f },

                // Droite
                { 1.0f, 1.0f, -1.0f },
                { 1.0f, 1.0f, 1.0f },
                { 1.0f, -1.0f, 1.0f },
                { 1.0f, 1.0f, -1.0f },
                { 1.0f, -1.0f, 1.0f },
                { 1.0f, -1.0f, -1.0f },

                // Arrière
                { -1.0f, 1.0f, 1.0f },
                { 1.0f, -1.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f },
                { -1.0f, 1.0f, 1.0f },
                { -1.0f, -1.0f, 1.0f },
                { 1.0f, -1.0f, 1.0f },

                // Gauche
                { -1.0f, 1.0f, 1.0f },
                { -1.0f, 1.0f, -1.0f },
                { -1.0f, -1.0f, -1.0f },
                { -1.0f, 1.0f, 1.0f },
                { -1.0f, -1.0f, -1.0f },
                { -1.0f, -1.0f, 1.0f },

                // Bas
                { -1.0f, -1.0f, -1.0f },
                { 1.0f, -1.0f, -1.0f },
                { 1.0f, -1.0f, 1.0f },
                { 1.0f, -1.0f, 1.0f },
                { -1.0f, -1.0f, 1.0f },
                { -1.0f, -1.0f, -1.0f },

                // Haut
                { -1.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, -1.0f },
                { -1.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, -1.0f },
                { -1.0f, 1.0f, -1.0f }

            };

            const color_buffer colors = {
                { { 1.0f, 0.0f, 0.0f, 1.0f },
                  { 0.0f, 1.0f, 0.0f, 1.0f },
                  { 1.0f, 1.0f, 1.0f, 1.0f },
                  { 1.0f, 1.0f, 0.0f, 1.0f },
                  { 0.0f, 1.0f, 1.0f, 1.0f },
                  { 0.0f, 0.0f, 1.0f, 1.0f } }
            };

            cube *c = mem::alloc_type<cube>(context.get(), context);

            if (c == nullptr)
            {
                return ref<cube>();
            }

            fmat4 model = fmat4();
            model       = fmat4::translate(model, position);
            model       = fmat4::rotate_x(model, rotation.x);
            model       = fmat4::rotate_y(model, rotation.y);
            model       = fmat4::rotate_z(model, rotation.z);
            model       = fmat4::scale(model, scale);

            fmat4 projection = fmat4::d3d_perspective_lh(1.0f, 3.0f / 4.0f, 0.5f, 10.0f);

            const per_object_buffer pob = {
                projection * model
            };

            c->m_vertex_buffer     = resource_factory::create_vertex_buffer(context, vertices, sizeof(vertices), sizeof(vertex), device);
            c->m_per_object_buffer = resource_factory::create_constant_buffer(context, &pob, sizeof(pob), device);
            c->m_color_buffer      = resource_factory::create_constant_buffer(context, &colors, sizeof(colors), device);
            c->m_vertex_shader     = vs;
            c->m_pixel_shader      = ps;

            c->m_location = position;
            c->m_rotation = rotation;
            c->m_scale    = fvec3(1.0f, 1.0f, 1.0f);

            return ref<cube>(context, c);
        }

        ref<textured_cube> drawable_factory::create_textured_cube(const ref<ctx> &context, const ref<vertex_shader> &vs, const ref<pixel_shader> &ps, const fvec3 &position, const fvec3 &rotation, const fvec3 &scale, const ref<texture> &tex, const ref<sampler> &samp, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept
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
                    float u;
                    float v;
                } tex;
            };

            const vertex vertices[] = {
                // Avant
                { -1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
                { 1.0f, 1.0f, -1.0f, 1.0f, 0.0f },
                { 1.0f, -1.0f, -1.0f, 1.0f, 1.0f },
                { -1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
                { 1.0f, -1.0f, -1.0f, 1.0f, 1.0f },
                { -1.0f, -1.0f, -1.0f, 0.0f, 1.0f },

                // Droite
                { 1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
                { 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
                { 1.0f, -1.0f, 1.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
                { 1.0f, -1.0f, 1.0f, 1.0f, 1.0f },
                { 1.0f, -1.0f, -1.0f, 0.0f, 1.0f },

                // Arrière
                { -1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
                { 1.0f, -1.0f, 1.0f, 0.0f, 1.0f },
                { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
                { -1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
                { -1.0f, -1.0f, 1.0f, 1.0f, 1.0f },
                { 1.0f, -1.0f, 1.0f, 0.0f, 1.0f },

                // Gauche
                { -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
                { -1.0f, 1.0f, -1.0f, 1.0f, 0.0f },
                { -1.0f, -1.0f, -1.0f, 1.0f, 1.0f },
                { -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
                { -1.0f, -1.0f, -1.0f, 1.0f, 1.0f },
                { -1.0f, -1.0f, 1.0f, 0.0f, 1.0f },

                // Bas
                { -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },
                { 1.0f, -1.0f, -1.0f, 1.0f, 0.0f },
                { 1.0f, -1.0f, 1.0f, 1.0f, 1.0f },
                { 1.0f, -1.0f, 1.0f, 1.0f, 1.0f },
                { -1.0f, -1.0f, 1.0f, 0.0f, 1.0f },
                { -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },

                // Haut
                { -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
                { 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
                { 1.0f, 1.0f, -1.0f, 1.0f, 1.0f },
                { -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
                { 1.0f, 1.0f, -1.0f, 1.0f, 1.0f },
                { -1.0f, 1.0f, -1.0f, 0.0f, 1.0f }

            };

            textured_cube *c = mem::alloc_type<textured_cube>(context.get(), context);

            if (c == nullptr)
            {
                return ref<textured_cube>();
            }

            c->m_texture = tex;
            c->m_sampler = samp;

            fmat4 model = fmat4();
            model       = fmat4::translate(model, position);
            model       = fmat4::rotate_x(model, rotation.x);
            model       = fmat4::rotate_y(model, rotation.y);
            model       = fmat4::rotate_z(model, rotation.z);
            model       = fmat4::scale(model, scale);

            fmat4 projection = fmat4::d3d_perspective_lh(1.0f, 3.0f / 4.0f, 0.5f, 10.0f);

            const per_object_buffer pob = {
                projection * model
            };

            c->m_vertex_buffer     = resource_factory::create_vertex_buffer(context, vertices, sizeof(vertices), sizeof(vertex), device);
            c->m_per_object_buffer = resource_factory::create_constant_buffer(context, &pob, sizeof(pob), device);
            c->m_vertex_shader     = vs;
            c->m_pixel_shader      = ps;

            c->m_location = position;
            c->m_rotation = rotation;
            c->m_scale    = fvec3(1.0f, 1.0f, 1.0f);

            return ref<textured_cube>(context, c);
        }

        ref<cube> drawable_factory::from(const ref<ctx> &context, ref<cube> &from_cube, const ref<vertex_shader> &vs, const ref<pixel_shader> &ps, const fvec3 &position, const fvec3 &rotation, const fvec3 &scale, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept
        {
            if (!from_cube.is_valid())
            {
                return ref<cube>();
            }

            cube *c = mem::alloc_type<cube>(context.get(), context);

            if (c == nullptr)
            {
                return ref<cube>();
            }

            fmat4 model = fmat4();
            model       = fmat4::translate(model, position);
            model       = fmat4::rotate_x(model, rotation.x);
            model       = fmat4::rotate_y(model, rotation.y);
            model       = fmat4::rotate_z(model, rotation.z);
            model       = fmat4::scale(model, scale);

            fmat4 projection = fmat4::d3d_perspective_lh(1.0f, 3.0f / 4.0f, 0.5f, 10.0f);

            const per_object_buffer pob = {
                projection * model
            };

            c->m_vertex_buffer     = from_cube->m_vertex_buffer;
            c->m_per_object_buffer = resource_factory::create_constant_buffer(context, &pob, sizeof(pob), device);
            c->m_color_buffer      = from_cube->m_color_buffer;
            c->m_vertex_shader     = vs;
            c->m_pixel_shader      = ps;

            return ref<cube>(context, c);
        }
    } // namespace D3D
} // namespace deep
