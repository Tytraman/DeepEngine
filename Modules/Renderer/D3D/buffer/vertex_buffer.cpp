#include "vertex_buffer.hpp"

namespace deep
{
    namespace D3D
    {
        ID3D11Buffer *vertex_buffer::get() const noexcept
        {
            return m_buffer.Get();
        }

        ID3D11Buffer *const *vertex_buffer::get_address() const noexcept
        {
            return m_buffer.GetAddressOf();
        }
    } // namespace D3D
} // namespace deep
