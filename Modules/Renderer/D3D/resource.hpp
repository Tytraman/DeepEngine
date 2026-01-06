#ifndef DEEP_ENGINE_D3D_RESOURCE_HPP
#define DEEP_ENGINE_D3D_RESOURCE_HPP

#include "deep_d3d_export.h"
#include <DeepLib/object.hpp>

#include <d3d11.h>
#include <wrl.h>

namespace deep
{
    namespace D3D
    {
        class DEEP_D3D_API resource : public object
        {
          public:
            resource()                            = delete;
            resource(const resource &)            = delete;
            resource &operator=(const resource &) = delete;

            virtual ~resource() = default;

            virtual void bind()                                                           = 0;
            virtual void draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context) = 0;

            virtual bool can_draw() const = 0;

          protected:
            using object::object;
        };
    } // namespace D3D
} // namespace deep

#endif
