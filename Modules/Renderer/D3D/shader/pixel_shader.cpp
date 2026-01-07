#include "pixel_shader.hpp"
#include "D3D/device_context.hpp"

namespace deep
{
    namespace D3D
    {
        ID3D11PixelShader *pixel_shader::get() const noexcept
        {
            return m_shader.Get();
        }
    } // namespace D3D
} // namespace deep
