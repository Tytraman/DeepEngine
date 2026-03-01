#ifndef DEEP_ENGINE_BASIC_SHAPES_HPP
#define DEEP_ENGINE_BASIC_SHAPES_HPP

#include <DeepLib/memory/ref_counted.hpp>

#include "D3D/drawable/cube.hpp"
#include "D3D/drawable/textured_cube.hpp"
#include "D3D/drawable/plane.hpp"

namespace deep
{
    struct basic_shapes
    {
        ref<D3D::cube> cube;
        ref<D3D::textured_cube> textured_cube;
        ref<D3D::plane> plane;
    };
} // namespace deep

#endif
