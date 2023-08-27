#ifndef __DEEP_ENGINE_CAMERA_HPP__
#define __DEEP_ENGINE_CAMERA_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/vec.hpp>

namespace de {

	class DE_API Camera {

		public:
			Camera(const fvec3 &position, const fvec3 &front, const fvec3 &baseUp = fvec3(0.0f, 1.0f, 0.0f));

			void update();

			fvec3 position() const;
			fvec3 baseUp() const;
			fvec3 front() const;
			fvec3 right() const;
			fvec3 up() const;

			fmat4x4 lookAt() const;

		private:
			fvec3 m_Position;
			fvec3 m_BaseUp;
			fvec3 m_Front;
			fvec3 m_Right;
			fvec3 m_Up;

			float m_Yaw;
			float m_Pitch;

	};

	/*
	================
	Camera::position
	================
	*/
	inline fvec3 Camera::position() const
	{
		return m_Position;
	}
	
	/*
	==============
	Camera::baseUp
	==============
	*/
	inline fvec3 Camera::baseUp() const
	{
		return m_BaseUp;
	}
			
	/*
	=============
	Camera::front
	=============
	*/
	inline fvec3 Camera::front() const
	{
		return m_Front;
	}
	
	/*
	=============
	Camera::right
	=============
	*/
	inline fvec3 Camera::right() const
	{
		return m_Right;
	}
			
	/*
	==========
	Camera::up
	==========
	*/
	inline fvec3 Camera::up() const
	{
		return m_Up;
	}


}

#endif