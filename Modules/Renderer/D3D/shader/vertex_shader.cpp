#include "vertex_shader.hpp"

namespace deep
{
    namespace D3D
    {
        void vertex_shader::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context) noexcept
        {
            device_context->VSSetShader(m_shader.Get(), nullptr, 0);
            device_context->IASetInputLayout(m_input_layout.Get());
        }

    } // namespace D3D
} // namespace deep
