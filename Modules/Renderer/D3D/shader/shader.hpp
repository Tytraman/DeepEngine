#ifndef DEEP_ENGINE_D3D_SHADER_HPP
#define DEEP_ENGINE_D3D_SHADER_HPP

#include "deep_d3d_export.h"
#include <DeepLib/object.hpp>

#include <d3d11.h>
#include <wrl.h>

namespace deep
{
    namespace D3D
    {
        class device_context;

        class DEEP_D3D_API shader : public object
        {
          public:
            shader()                          = delete;
            shader(const shader &)            = delete;
            shader &operator=(const shader &) = delete;

            virtual void bind(device_context &dc) = 0;

          protected:
            using object::object;
        };
    } // namespace D3D

} // namespace deep

#endif
