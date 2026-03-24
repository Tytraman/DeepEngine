#include "Assimp/loader.hpp"

#include "D3D/resource_factory.hpp"
#include "D3D/buffer/per_object_buffer.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace deep
{
    namespace model
    {
        void loader::print_info(const ref<ctx> &context, const char *filename) noexcept
        {
            unsigned int index;

            if (!context.is_valid())
            {
                return;
            }

            Assimp::Importer importer;

            const aiScene *scene = importer.ReadFile(filename,
                                                     aiProcess_Triangulate |
                                                             aiProcess_JoinIdenticalVertices);

            if (scene == nullptr)
            {
                context->err() << "[ERROR] Unable to load '" << filename << "' model.\r\n";

                return;
            }

            context->out() << "'" << filename << "' model infos:\r\n";

            context->out() << "    Meshes: " << scene->mNumMeshes << "\r\n";
            context->out() << "    Materials: " << scene->mNumMaterials << "\r\n";
            context->out() << "    Textures: " << scene->mNumTextures << "\r\n";
            context->out() << "    Skeletons: " << scene->mNumSkeletons << "\r\n";
            context->out() << "    Animations: " << scene->mNumAnimations << "\r\n";

            for (index = 0; index < scene->mNumMeshes; ++index)
            {
                const aiMesh *mesh = scene->mMeshes[index];

                context->out() << "Mesh " << index << " infos:\r\n";

                context->out() << "    Name: " << mesh->mName.C_Str() << "\r\n";
                context->out() << "    Vertices: " << mesh->mNumVertices << "\r\n";
                context->out() << "    Faces: " << mesh->mNumFaces << "\r\n";
            }
        }

        ref<D3D::mesh> loader::load(const ref<ctx> &context,
                                    const char *filename,
                                    const ref<D3D::vertex_shader> &vs,
                                    const ref<D3D::pixel_shader> &ps,
                                    const fvec3 &position,
                                    const fvec3 &rotation,
                                    const fvec3 &scale,
                                    const Microsoft::WRL::ComPtr<ID3D11Device> &device) noexcept
        {
            D3D::mesh *c = mem::alloc_type<D3D::mesh>(context.get(), context);

            if (c == nullptr)
            {
                return ref<D3D::mesh>();
            }

            fmat4 model = fmat4();
            model       = fmat4::translate(model, position);
            model       = fmat4::rotate_x(model, rotation.x);
            model       = fmat4::rotate_y(model, rotation.y);
            model       = fmat4::rotate_z(model, rotation.z);
            model       = fmat4::scale(model, scale);

            fmat4 projection = fmat4::d3d_perspective_lh(1.0f, 3.0f / 4.0f, 0.5f, 10.0f);

            const D3D::per_object_buffer pob = {
                projection * model
            };

            Assimp::Importer importer;

            const aiScene *scene = importer.ReadFile(filename,
                                                     aiProcess_Triangulate |
                                                             aiProcess_JoinIdenticalVertices);

            if (scene == nullptr)
            {
                return ref<D3D::mesh>();
            }

            // c->m_vertex_buffer = D3D::resource_factory::create_vertex_buffer(context, vertices, sizeof(vertices), sizeof(vertex), device);
            c->set_per_object_buffer(D3D::resource_factory::create_constant_buffer(context, &pob, sizeof(pob), device));
            c->set_vertex_shader(vs);
            c->set_pixel_shader(ps);

            c->set_location(position);
            c->set_rotation(rotation);
            c->set_scale(scale);

            return ref<D3D::mesh>(context, c);
        }
    } // namespace model
} // namespace deep
