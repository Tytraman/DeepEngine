#include <DE/rendering/camera.hpp>
#include <DE/mat.hpp>

#include <math.h>

namespace de {

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
		update();
	}

	/*
	==============
	Camera::update
	==============
	*/
	void Camera::update()
	{
		fvec3 front;

		float cosYaw   = cosf(m_Yaw * M_PI / 180.0f);
		float cosPitch = cosf(m_Pitch * M_PI / 180.0f);
		float sinYaw = sinf(m_Yaw * M_PI / 180.0f);
		float sinPitch = sinf(m_Pitch * M_PI / 180.0f);

		front.x = cosYaw * cosPitch;
		front.y = sinPitch;
		front.z = sinYaw * cosPitch;

		m_Front = fvec3::normalize(front);
		m_Right = fvec3::normalize(fvec3::cross(m_Front, m_BaseUp));
		m_Up    = fvec3::normalize(fvec3::cross(m_Right, m_Front));
	}

	fmat4x4 Camera::lookAt() const
	{
		fvec3 direction = fvec3::normalize(m_Position - (m_Position + m_Front));
		fvec3 right     = fvec3::normalize(fvec3::cross(m_Up, direction));
		fvec3 up        = fvec3::cross(direction, right);

		return {
			right.x,     right.y,     right.z,     -m_Position.x,
			up.x,        up.y,        up.z,        -m_Position.y,
			direction.x, direction.y, direction.z, -m_Position.z,
			0.0f,        0.0f,        0.0f,        1.0f
		};
	}

}
