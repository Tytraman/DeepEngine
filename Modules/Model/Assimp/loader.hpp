#ifndef DEEP_ENGINE_MODEL_ASSIMP_HPP
#define DEEP_ENGINE_MODEL_ASSIMP_HPP

#include "DeepEngine/deep_engine_export.h"

#include "DeepLib/context.hpp"

#include "D3D/drawable/mesh.hpp"

namespace deep
{
    namespace model
    {
        class DEEP_ENGINE_API loader
        {
          public:
            static void print_info(const ref<ctx> &context, const char *filename) noexcept;

            static ref<D3D::mesh> load(const ref<ctx> &context,
                                       const char *filename,
                                       const ref<D3D::vertex_shader> &vs,
                                       const ref<D3D::pixel_shader> &ps,
                                       const fvec3 &position,
                                       const fvec3 &rotation,
                                       const fvec3 &scale,
                                       const Microsoft::WRL::ComPtr<ID3D11Device> &device) noexcept;
        };
    } // namespace model
} // namespace deep

#endif
