#ifndef DEEP_ENGINE_D3D_TEXTURE_HPP
#define DEEP_ENGINE_D3D_TEXTURE_HPP

#include "deep_d3d_export.h"
#include <DeepLib/object.hpp>
#include <d3d11.h>
#include <wrl.h>

namespace deep
{
    namespace D3D
    {
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>;

        class DEEP_D3D_API texture : public object
        {
          public:
          protected:
            using object::object;

          protected:
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture_view;

          public:
            friend class resource_factory;
            friend class device_context;
        };
    } // namespace D3D
} // namespace deep

#endif
