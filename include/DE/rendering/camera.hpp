#ifndef __DEEP_ENGINE_CAMERA_HPP__
#define __DEEP_ENGINE_CAMERA_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/vec.hpp>

namespace de {

	/// @brief Les caméras sont ce qui donnent l'effet de voir le monde selon une position.
	class DE_API Camera {

		public:
			Camera(const fvec3 &position, const fvec3 &front, const fvec3 &baseUp = fvec3(0.0f, 1.0f, 0.0f));

			/// @brief Met à jour l'angle de vision après avoir modifié le 'yaw' et le 'pitch' de la caméra.
			void updateAngleOfView();

			/// @brief  Récupère la position relative au monde de la caméra.
			/// @return La position relative au monde de la caméra.
			fvec3 position() const;

			/// @brief  Récupère le vecteur qui pointe vers le haut.
			/// @return Le vecteur qui pointe vers le haut.
			fvec3 baseUp() const;

			/// @brief  Récupère le vecteur qui pointe vers l'avant de la caméra.
			/// @return Le vecteur qui pointe vers l'avant de la caméra.
			fvec3 front() const;

			/// @brief  Récupère le vecteur qui pointe vers le haut de la caméra en prenant en compte l'angle de vision.
			/// @return Le vecteur qui pointe vers le haut de la caméra.
			fvec3 up() const;

			/// @brief  Récupère l'angle de vision horizontale de la caméra.
			/// @return L'angle de vision horizontale en degrées de la caméra.
			float yaw() const;

			/// @brief  Récupère l'angle de vision horizontale de la caméra.
			/// @return L'angle de vision horizontale en degrées de la caméra.
			float pitch() const;

			void setYaw(float value);
			void setPitch(float value);

			void setPosition(const fvec3 &pos);

			void moveRight(float value);
			void moveLeft(float value);
			void moveForward(float value);
			void moveBackward(float value);
			void moveUp(float value);
			void moveDown(float value);

			fmat4x4 lookAt() const;

		private:
			fvec3 m_Position;    ///< Position de la caméra.
			fvec3 m_BaseUp;      ///< Vecteur qui pointe vers le haut sans jamais changer.
			fvec3 m_Front;       ///< Correspond à l'axe Z de la caméra.
			fvec3 m_Up;          ///< Correspond à l'axe Y de la caméra.

			float m_Yaw;         ///< Angle de vision horizontale.
			float m_Pitch;       ///< Angle de vision verticale.

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
	==========
	Camera::up
	==========
	*/
	inline fvec3 Camera::up() const
	{
		return m_Up;
	}

	/*
	===========
	Camera::yaw
	===========
	*/
	inline float Camera::yaw() const
	{
		return m_Yaw;
	}

	/*
	=============
	Camera::pitch
	=============
	*/
	inline float Camera::pitch() const
	{
		return m_Pitch;
	}

	/*
	==============
	Camera::setYaw
	==============
	*/
	inline void Camera::setYaw(float value)
	{
		m_Yaw = value;
	}

	/*
	================
	Camera::setPitch
	================
	*/
	inline void Camera::setPitch(float value)
	{
		m_Pitch = value;
	}

	/*
	===================
	Camera::setPosition
	===================
	*/
	inline void Camera::setPosition(const fvec3 &pos)
	{
		m_Position = pos;
	}


}

#endif