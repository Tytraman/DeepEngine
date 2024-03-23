#include "core/camera.hpp"
#include "maths/mat.hpp"

#include <math.h>

namespace deep
{

	/*
	==============
	camera::camera
	==============
	*/
	camera::camera(const vec3<float> &position, const vec3<float> &front, const vec3<float> &baseUp)
		: m_Position(position),
		  m_Front(front),
		  m_BaseUp(baseUp),
		  m_Yaw(-90.0f),
		  m_Pitch(0.0f)
	{
		update_angle_of_view();
	}

	/*
	==============
	camera::update
	==============
	*/
	void camera::update_angle_of_view()
	{
		vec3<float> front;

		float cosYaw   = cosf(m_Yaw * static_cast<float>(M_PI) / 180.0f);
		float cosPitch = cosf(m_Pitch * static_cast<float>(M_PI) / 180.0f);
		float sinYaw   = sinf(m_Yaw * static_cast<float>(M_PI) / 180.0f);
		float sinPitch = sinf(m_Pitch * static_cast<float>(M_PI) / 180.0f);

		front.x = cosYaw * cosPitch;
		front.y = sinPitch;
		front.z = sinYaw * cosPitch;

		m_Front     = vec3<float>::normalize(front);
		vec3<float> right = vec3<float>::normalize(vec3<float>::cross(m_Front, m_BaseUp));
		m_Up        = vec3<float>::normalize(vec3<float>::cross(right, m_Front));
	}

	/*
	==============
	camera::get_look_at
	==============
	*/
	fmat4x4 camera::get_look_at() const
	{
		vec3<float> direction = vec3<float>::normalize(m_Position - (m_Position + m_Front));
		vec3<float> right     = vec3<float>::normalize(vec3<float>::cross(m_Front, m_BaseUp));

		return {
			right.x,     right.y,     right.z,     -vec3<float>::dot(right, m_Position),
			m_Up.x,      m_Up.y,      m_Up.z,      -vec3<float>::dot(m_Up, m_Position),
			direction.x, direction.y, direction.z, -vec3<float>::dot(direction, m_Position),
			0.0f,        0.0f,        0.0f,        1.0f
		};
	}

	void camera::move_right(float value)
	{
		vec3<float> right = vec3<float>::normalize(vec3<float>::cross(m_Front, m_BaseUp));

		// Bloque l'axe Y pour ne pas "voler".
		right.y = 0.0f;

		m_Position += right * value;
	}

	void camera::move_left(float value)
	{
		vec3<float> right = vec3<float>::normalize(vec3<float>::cross(m_Front, m_BaseUp));

		// Bloque l'axe Y pour ne pas "voler".
		right.y = 0.0f;

		m_Position -= right * value;
	}

	void camera::move_forward(float value)
	{
		vec3<float> front = m_Front;

		// Bloque l'axe Y pour ne pas "voler".
		front.y = 0.0f;
		float length = vec3<float>::magn(front);
		front.x *= 1.0f / length;
		front.z *= 1.0f / length;

		m_Position += front * value;
	}

	void camera::move_backward(float value)
	{
		vec3<float> front = m_Front;

		// Bloque l'axe Y pour ne pas "voler".
		front.y = 0.0f;
		float length = vec3<float>::magn(front);
		front.x *= 1.0f / length;
		front.z *= 1.0f / length;

		m_Position -= front * value;
	}

	void camera::move_up(float value)
	{
		m_Position += m_BaseUp * value;
	}

	void camera::move_down(float value)
	{
		m_Position -= m_BaseUp * value;
	}

}
