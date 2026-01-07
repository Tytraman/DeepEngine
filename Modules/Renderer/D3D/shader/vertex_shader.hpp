#ifndef DEEP_ENGINE_D3D_VERTEX_SHADER_HPP
#define DEEP_ENGINE_D3D_VERTEX_SHADER_HPP

#include "deep_d3d_export.h"
#include "D3D/shader/shader.hpp"

namespace deep
{
    namespace D3D
    {
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11VertexShader>;
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11InputLayout>;

        class DEEP_D3D_API vertex_shader : public shader
        {
          public:
            vertex_shader()                                 = delete;
            vertex_shader(const vertex_shader &)            = delete;
            vertex_shader &operator=(const vertex_shader &) = delete;

            virtual void bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context) noexcept override;

          private:
            Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
            Microsoft::WRL::ComPtr<ID3D11InputLayout> m_input_layout;

          protected:
            using shader::shader;

          public:
            friend class shader_factory;
        };
    } // namespace D3D
} // namespace deep

#endif
