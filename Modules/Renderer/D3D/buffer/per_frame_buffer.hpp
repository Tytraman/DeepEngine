#ifndef DEEP_ENGINE_D3D_PER_FRAME_BUFFER_HPP
#define DEEP_ENGINE_D3D_PER_FRAME_BUFFER_HPP

#include "deep_d3d_export.h"
#include <DeepLib/maths/mat.hpp>

namespace deep
{
    namespace D3D
    {
        template struct DEEP_D3D_API mat4x4<float>;

        struct alignas(16) DEEP_D3D_API per_frame_buffer
        {
            fmat4 view;
            fmat4 projection;
        };
    } // namespace D3D
} // namespace deep

#endif
