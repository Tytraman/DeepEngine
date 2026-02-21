#ifndef DEEP_ENGINE_CAMERA_HPP
#define DEEP_ENGINE_CAMERA_HPP

#include "DeepEngine/deep_engine_export.h"

#include <DeepLib/object.hpp>
#include <DeepLib/maths/vec.hpp>
#include <DeepLib/maths/mat.hpp>

namespace deep
{
    class DEEP_ENGINE_API camera : public object
    {
      public:
        camera() = delete;
        camera(const ref<ctx> &context, const fvec3 &location) noexcept;

        void walk(float value) noexcept;
        void strafe(float value) noexcept;
        void move_vertically(float value) noexcept;

        void rotate_horizontally(float degrees) noexcept;
        void rotate_vertically(float degrees) noexcept;

        /**
         * @brief Récupère la position relative au monde.
         */
        const fvec3 get_location() const noexcept;

        /**
         * @brief Modifie la position relative au monde.
         */
        void set_location(const fvec3 &location) noexcept;

        float get_yaw() const noexcept;
        void set_yaw(float degrees) noexcept;
        float get_pitch() const noexcept;
        void set_pitch(float degrees) noexcept;

        float get_z_near() const noexcept;
        float get_z_far() const noexcept;
        float get_aspect_ratio() const noexcept;
        float get_horizontal_fov() const noexcept;
        float get_vertical_fov() const noexcept;
        float get_near_window_width() const noexcept;
        float get_far_window_width() const noexcept;
        float get_near_window_height() const noexcept;
        float get_far_window_height() const noexcept;

        fvec3 get_forward_axis() const noexcept;
        fvec3 get_right_axis() const noexcept;
        fvec3 get_up_axis() const noexcept;

        fmat4 get_view() const noexcept;
        fmat4 get_projection() const noexcept;

        void set_lens(float vertical_fov, float aspect_ratio, float z_near, float z_far) noexcept;

      protected:
        // Coordonnées relative au "world space".
        DEEP_FVEC3 m_location;

        float m_yaw;
        float m_pitch;

        float m_z_near;
        float m_z_far;
        float m_aspect_ratio;
        float m_vertical_fov;
        float m_near_window_height;
        float m_far_window_height;

        fmat4 m_projection;
    };
} // namespace deep

#endif
