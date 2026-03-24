#include "D3D/buffer/index_buffer.hpp"

namespace deep
{
    namespace D3D
    {
        ID3D11Buffer *index_buffer::get() const noexcept
        {
            return m_buffer.Get();
        }

        ID3D11Buffer *const *index_buffer::get_address() const noexcept
        {
            return m_buffer.GetAddressOf();
        }

        uint16 index_buffer::count() const noexcept
        {
            return m_count;
        }
    } // namespace D3D
} // namespace deep
