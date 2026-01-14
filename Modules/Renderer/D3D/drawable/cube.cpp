#include "cube.hpp"

namespace deep
{
    namespace D3D
    {
        void cube::draw(device_context &dc)
        {
            dc.bind_shader(m_vertex_shader);
            dc.bind_shader(m_pixel_shader);

            dc.bind(m_vertex_buffer);

            dc.get()->VSSetConstantBuffers(0, 1, m_transform_buffer->get_address());
            dc.get()->PSSetConstantBuffers(0, 1, m_color_buffer->get_address());

            dc.get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            dc.get()->Draw(6 * 6, 0);
        }
    } // namespace D3D
} // namespace deep
