#include "constant_buffer.hpp"
#include "D3D/device_context.hpp"

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

        void constant_buffer::update(const void *data, const device_context &dc) noexcept
        {
            dc.get()->UpdateSubresource(m_buffer.Get(), 0, nullptr, data, 0, 0);
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

        uint32 constant_buffer::get_bytes_size() const noexcept
        {
            return m_bytes_size;
        }
    } // namespace D3D
} // namespace deep
