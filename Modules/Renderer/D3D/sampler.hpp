#ifndef DEEP_ENGINE_D3D_SAMPLER_HPP
#define DEEP_ENGINE_D3D_SAMPLER_HPP

#include "deep_d3d_export.h"
#include <DeepLib/object.hpp>
#include <d3d11.h>
#include <wrl.h>

namespace deep
{
    namespace D3D
    {
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11SamplerState>;

        class DEEP_D3D_API sampler : public object
        {
          public:
          protected:
            Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler_state;

          protected:
            using object::object;

          public:
            friend class resource_factory;
            friend class device_context;
        };
    } // namespace D3D
} // namespace deep

#endif
