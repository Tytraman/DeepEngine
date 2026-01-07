#ifndef DEEP_ENGINE_D3D_PIXEL_SHADER_HPP
#define DEEP_ENGINE_D3D_PIXEL_SHADER_HPP

#include "deep_d3d_export.h"
#include <DeepLib/object.hpp>

#include <d3d11.h>
#include <wrl.h>

namespace deep
{
    namespace D3D
    {
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11PixelShader>;

        class DEEP_D3D_API pixel_shader : public object
        {
          public:
            pixel_shader()                                = delete;
            pixel_shader(const pixel_shader &)            = delete;
            pixel_shader &operator=(const pixel_shader &) = delete;

            ID3D11PixelShader *get() const noexcept;

          private:
            Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;

          protected:
            using object::object;

          public:
            friend class shader_factory;
        };
    } // namespace D3D
} // namespace deep

#endif
