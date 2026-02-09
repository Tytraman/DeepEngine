#include "drawable.hpp"
#include "triangle.hpp"

namespace deep
{
    namespace D3D
    {
        void triangle::draw(device_context &dc, const fmat4 & /*view_projection*/)
        {
            dc.bind_shader(m_vertex_shader);
            dc.bind_shader(m_pixel_shader);

            dc.get()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            dc.get()->Draw(3, 0);
        }
    } // namespace D3D
} // namespace deep
