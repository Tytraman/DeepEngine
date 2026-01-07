#include "vertex_buffer.hpp"

namespace deep
{
    namespace D3D
    {
        void vertex_buffer::bind() noexcept
        {
        }

        void vertex_buffer::draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context) noexcept
        {
            device_context->Draw(3, 0);
        }

        bool vertex_buffer::can_draw() const noexcept
        {
            return true;
        }
    } // namespace D3D
} // namespace deep
