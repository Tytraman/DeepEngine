#include "cube.hpp"
#include "D3D/buffer/per_object_buffer.hpp"

namespace deep
{
    namespace D3D
    {
        void cube::draw(device_context &dc, const fmat4 &view_projection)
        {
            dc.bind_shader(m_vertex_shader);
            dc.bind_shader(m_pixel_shader);

            dc.bind(m_vertex_buffer);

            dc.get()->VSSetConstantBuffers(1, 1, m_per_object_buffer->get_address());
            dc.get()->PSSetConstantBuffers(0, 1, m_color_buffer->get_address());

            fmat4 model = fmat4();
            model       = fmat4::translate(model, m_location);
            model       = fmat4::rotate_x(model, m_rotation.x);
            model       = fmat4::rotate_y(model, m_rotation.y);
            model       = fmat4::rotate_z(model, m_rotation.z);
            model       = fmat4::scale(model, m_scale);

            fmat4 projection = fmat4::d3d_perspective_lh(1.0f, 3.0f / 4.0f, 0.5f, 10.0f);

            const per_object_buffer pob = {
                view_projection * model
            };

            m_per_object_buffer->update(&pob, dc);

            dc.get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            dc.get()->Draw(6 * 6, 0);
        }
    } // namespace D3D
} // namespace deep
