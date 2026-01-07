#include "pixel_shader.hpp"

namespace deep
{
    namespace D3D
    {
        void pixel_shader::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context) noexcept
        {
            device_context->PSSetShader(m_shader.Get(), nullptr, 0);
        }
    } // namespace D3D
} // namespace deep
