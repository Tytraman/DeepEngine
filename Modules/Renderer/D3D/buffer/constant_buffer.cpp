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
    } // namespace D3D
} // namespace deep
