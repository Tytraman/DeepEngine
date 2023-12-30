#include <DE/rendering/camera.hpp>
#include <DE/mat.hpp>

#include <math.h>

namespace deep {

	/*
	==============
	Camera::Camera
	==============
	*/
	Camera::Camera(const fvec3 &position, const fvec3 &front, const fvec3 &baseUp)
		: m_Position(position),
		  m_Front(front),
		  m_BaseUp(baseUp),
		  m_Yaw(-90.0f),
		  m_Pitch(0.0f)
	{
		updateAngleOfView();
	}

	/*
	==============
	Camera::update
	==============
	*/
	void Camera::updateAngleOfView()
	{
		fvec3 front;

		float cosYaw   = cosf(m_Yaw * static_cast<float>(M_PI) / 180.0f);
		float cosPitch = cosf(m_Pitch * static_cast<float>(M_PI) / 180.0f);
		float sinYaw   = sinf(m_Yaw * static_cast<float>(M_PI) / 180.0f);
		float sinPitch = sinf(m_Pitch * static_cast<float>(M_PI) / 180.0f);

		front.x = cosYaw * cosPitch;
		front.y = sinPitch;
		front.z = sinYaw * cosPitch;

		m_Front     = fvec3::normalize(front);
		fvec3 right = fvec3::normalize(fvec3::cross(m_Front, m_BaseUp));
		m_Up        = fvec3::normalize(fvec3::cross(right, m_Front));
	}

	/*
	==============
	Camera::lookAt
	==============
	*/
	fmat4x4 Camera::lookAt() const
	{
		fvec3 direction = fvec3::normalize(m_Position - (m_Position + m_Front));
		fvec3 right     = fvec3::normalize(fvec3::cross(m_Front, m_BaseUp));

		return {
			right.x,     right.y,     right.z,     -fvec3::dot(right, m_Position),
			m_Up.x,      m_Up.y,      m_Up.z,      -fvec3::dot(m_Up, m_Position),
			direction.x, direction.y, direction.z, -fvec3::dot(direction, m_Position),
			0.0f,        0.0f,        0.0f,        1.0f
		};
	}

	void Camera::moveRight(float value)
	{
		fvec3 right = fvec3::normalize(fvec3::cross(m_Front, m_BaseUp));

		// Bloque l'axe Y pour ne pas "voler".
		right.y = 0.0f;

		m_Position += right * value;
	}

	void Camera::moveLeft(float value)
	{
		fvec3 right = fvec3::normalize(fvec3::cross(m_Front, m_BaseUp));

		// Bloque l'axe Y pour ne pas "voler".
		right.y = 0.0f;

		m_Position -= right * value;
	}

	void Camera::moveForward(float value)
	{
		fvec3 front = m_Front;

		// Bloque l'axe Y pour ne pas "voler".
		front.y = 0.0f;
		float length = fvec3::magn(front);
		front.x *= 1.0f / length;
		front.z *= 1.0f / length;

		m_Position += front * value;
	}

	void Camera::moveBackward(float value)
	{
		fvec3 front = m_Front;

		// Bloque l'axe Y pour ne pas "voler".
		front.y = 0.0f;
		float length = fvec3::magn(front);
		front.x *= 1.0f / length;
		front.z *= 1.0f / length;

		m_Position -= front * value;
	}

	void Camera::moveUp(float value)
	{
		m_Position += m_BaseUp * value;
	}

	void Camera::moveDown(float value)
	{
		m_Position -= m_BaseUp * value;
	}

}
