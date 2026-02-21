#include "camera.hpp"

#include <cmath>

namespace deep
{
    camera::camera(const ref<ctx> &context, const fvec3 &location) noexcept
            : object(context),
              m_location(location),
              m_yaw(0.0f),
              m_pitch(0.0f),
              m_z_near(0.0f),
              m_z_far(0.0f),
              m_aspect_ratio(0.0f),
              m_vertical_fov(0.0f),
              m_near_window_height(0.0f),
              m_far_window_height(0.0f)
    {
    }

    void camera::walk(float value) noexcept
    {
        fvec3 forward = get_forward_axis();

        forward.y = 0.0f;
        forward   = fvec3::norm(forward);

        m_location += forward * value;
    }

    void camera::strafe(float value) noexcept
    {
        m_location += get_right_axis() * value;
    }

    void camera::move_vertically(float value) noexcept
    {
        m_location.y += value;
    }

    void camera::rotate_horizontally(float degrees) noexcept
    {
        m_yaw += degrees;

        m_yaw = std::fmod(m_yaw, 360.0f);
    }

    void camera::rotate_vertically(float degrees) noexcept
    {
        m_pitch += degrees;

        if (m_pitch > 89.0f)
        {
            m_pitch = 89.0f;
        }
        else if (m_pitch < -89.0f)
        {
            m_pitch = -89.0f;
        }
    }

    const fvec3 camera::get_location() const noexcept
    {
        return m_location;
    }

    void camera::set_location(const fvec3 &location) noexcept
    {
        m_location = location;
    }

    float camera::get_yaw() const noexcept
    {
        return m_yaw;
    }

    void camera::set_yaw(float degrees) noexcept
    {
        m_yaw = degrees;
    }

    float camera::get_pitch() const noexcept
    {
        return m_pitch;
    }

    void camera::set_pitch(float degrees) noexcept
    {
        m_pitch = degrees;
    }

    float camera::get_z_near() const noexcept
    {
        return m_z_near;
    }

    float camera::get_z_far() const noexcept
    {
        return m_z_far;
    }

    float camera::get_aspect_ratio() const noexcept
    {
        return m_aspect_ratio;
    }

    float camera::get_horizontal_fov() const noexcept
    {
        float half_width = 0.5f * m_near_window_height;

        return 2.0f * std::atan(half_width / m_z_near);
    }

    float camera::get_vertical_fov() const noexcept
    {
        return m_vertical_fov;
    }

    float camera::get_near_window_width() const noexcept
    {
        return m_aspect_ratio * m_near_window_height;
    }

    float camera::get_far_window_width() const noexcept
    {
        return m_aspect_ratio * m_far_window_height;
    }

    float camera::get_near_window_height() const noexcept
    {
        return m_near_window_height;
    }

    float camera::get_far_window_height() const noexcept
    {
        return m_far_window_height;
    }

    fvec3 camera::get_forward_axis() const noexcept
    {
        float yaw_rad   = math::deg_to_rad(m_yaw);
        float pitch_rad = math::deg_to_rad(m_pitch);

        float cos_yaw   = std::cos(yaw_rad);
        float sin_yaw   = std::sin(yaw_rad);
        float cos_pitch = std::cos(pitch_rad);
        float sin_pitch = std::sin(pitch_rad);

        float x = sin_yaw * cos_pitch;
        float y = sin_pitch;
        float z = cos_yaw * cos_pitch;

        return fvec3::norm(fvec3(x, y, z));
    }

    fvec3 camera::get_right_axis() const noexcept
    {
        fvec3 forward_axis = get_forward_axis();

        return fvec3::norm(fvec3::cross(fvec3(0.0f, 1.0f, 0.0f), forward_axis));
    }

    fvec3 camera::get_up_axis() const noexcept
    {
        float yaw_rad   = math::deg_to_rad(m_yaw);
        float pitch_rad = math::deg_to_rad(m_pitch);

        float cos_yaw   = std::cos(yaw_rad);
        float sin_yaw   = std::sin(yaw_rad);
        float cos_pitch = std::cos(pitch_rad);
        float sin_pitch = std::sin(pitch_rad);

        float x = sin_yaw * cos_pitch;
        float y = sin_pitch;
        float z = cos_yaw * cos_pitch;

        fvec3 forward_axis = fvec3::norm(fvec3(x, y, z));
        fvec3 right_axis   = fvec3::norm(fvec3::cross(fvec3(0.0f, 1.0f, 0.0f), forward_axis));

        return fvec3::cross(forward_axis, right_axis);
    }

    fmat4 camera::get_view() const noexcept
    {
        fvec3 forward = get_forward_axis();
        fvec3 target  = m_location + forward;

        return fmat4::d3d_look_at_lh(m_location, target, fvec3(0.0f, 1.0f, 0.0f));
    }

    fmat4 camera::get_projection() const noexcept
    {
        return m_projection;
    }

    void camera::set_lens(float vertical_fov, float aspect_ratio, float z_near, float z_far) noexcept
    {
        m_vertical_fov = vertical_fov;
        m_aspect_ratio = aspect_ratio;
        m_z_near       = z_near;
        m_z_far        = z_far;

        m_near_window_height = 2.0f * z_near * std::tan(0.5f * vertical_fov);
        m_far_window_height  = 2.0f * z_far * std::tan(0.5f * vertical_fov);

        fmat4 projection = fmat4();
        projection       = fmat4::d3d_perspective_fov_lh(vertical_fov, aspect_ratio, z_near, z_far);

        m_projection = projection;
    }
} // namespace deep
