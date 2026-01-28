#include "rectangle.hpp"

namespace deep
{
    namespace D3D
    {
        void rectangle::draw(device_context &dc, const fvec3 & /*camera_location*/)
        {
            dc.bind_shader(m_vertex_shader);
            dc.bind_shader(m_pixel_shader);

            dc.get()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            dc.get()->Draw(6, 0);
        }
    } // namespace D3D
} // namespace deep
