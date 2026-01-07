#include "vertex_shader.hpp"
#include "D3D/device_context.hpp"

namespace deep
{
    namespace D3D
    {

        ID3D11VertexShader *vertex_shader::get() const noexcept
        {
            return m_shader.Get();
        }

        ID3D11InputLayout *vertex_shader::get_input_layout() const noexcept
        {
            return m_input_layout.Get();
        }

    } // namespace D3D
} // namespace deep
