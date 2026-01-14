#include "constant_buffer.hpp"

namespace deep
{
    namespace D3D
    {
        void constant_buffer::bind() noexcept
        {
        }

        void constant_buffer::draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> /*device_context*/) noexcept
        {
        }

        bool constant_buffer::can_draw() const noexcept
        {
            return false;
        }

        ID3D11Buffer *constant_buffer::get() const noexcept
        {
            return m_buffer.Get();
        }

        ID3D11Buffer *const *constant_buffer::get_address() const noexcept
        {
            return m_buffer.GetAddressOf();
        }
    } // namespace D3D
} // namespace deep
