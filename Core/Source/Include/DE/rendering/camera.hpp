#ifndef __DEEP_ENGINE_CAMERA_HPP__
#define __DEEP_ENGINE_CAMERA_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/vec.hpp"

namespace deep
{

	/// @brief Les caméras sont ce qui donnent l'effet de voir le monde selon une position.
	class camera
	{

		public:
			DE_API camera(const fvec3 &position, const fvec3 &front, const fvec3 &baseUp = fvec3(0.0f, 1.0f, 0.0f));

			/// @brief Met à jour l'angle de vision après avoir modifié le 'yaw' et le 'pitch' de la caméra.
			DE_API void update_angle_of_view();

			/// @brief  Récupère la position relative au monde de la caméra.
			/// @return La position relative au monde de la caméra.
			DE_API fvec3 get_position() const;

			/// @brief  Récupère le vecteur qui pointe vers le haut.
			/// @return Le vecteur qui pointe vers le haut.
			DE_API fvec3 get_base_up() const;

			/// @brief  Récupère le vecteur qui pointe vers l'avant de la caméra.
			/// @return Le vecteur qui pointe vers l'avant de la caméra.
			DE_API fvec3 get_front() const;

			/// @brief  Récupère le vecteur qui pointe vers le haut de la caméra en prenant en compte l'angle de vision.
			/// @return Le vecteur qui pointe vers le haut de la caméra.
			DE_API fvec3 get_up() const;

			/// @brief  Récupère l'angle de vision horizontale de la caméra.
			/// @return L'angle de vision horizontale en degrées de la caméra.
			DE_API float get_yaw() const;

			/// @brief  Récupère l'angle de vision horizontale de la caméra.
			/// @return L'angle de vision horizontale en degrées de la caméra.
			DE_API float get_pitch() const;

			DE_API void set_yaw(float value);
			DE_API void set_pitch(float value);

			DE_API void set_position(const fvec3 &pos);

			DE_API void move_right(float value);
			DE_API void move_left(float value);
			DE_API void move_forward(float value);
			DE_API void move_backward(float value);
			DE_API void move_up(float value);
			DE_API void move_down(float value);

			DE_API fmat4x4 get_look_at() const;

		private:
			fvec3 m_Position;    ///< Position de la caméra.
			fvec3 m_BaseUp;      ///< Vecteur qui pointe vers le haut sans jamais changer.
			fvec3 m_Front;       ///< Correspond à l'axe Z de la caméra.
			fvec3 m_Up;          ///< Correspond à l'axe Y de la caméra.

			float m_Yaw;         ///< Angle de vision horizontale.
			float m_Pitch;       ///< Angle de vision verticale.

	};

	/*
	====================
	camera::get_position
	====================
	*/
	inline fvec3 camera::get_position() const
	{
		return m_Position;
	}
	
	/*
	===================
	camera::get_base_up
	===================
	*/
	inline fvec3 camera::get_base_up() const
	{
		return m_BaseUp;
	}
			
	/*
	=================
	camera::get_front
	=================
	*/
	inline fvec3 camera::get_front() const
	{
		return m_Front;
	}
			
	/*
	==============
	camera::get_up
	==============
	*/
	inline fvec3 camera::get_up() const
	{
		return m_Up;
	}

	/*
	===============
	camera::get_yaw
	===============
	*/
	inline float camera::get_yaw() const
	{
		return m_Yaw;
	}

	/*
	=================
	camera::get_pitch
	=================
	*/
	inline float camera::get_pitch() const
	{
		return m_Pitch;
	}

	/*
	===============
	camera::set_yaw
	===============
	*/
	inline void camera::set_yaw(float value)
	{
		m_Yaw = value;
	}

	/*
	=================
	camera::set_pitch
	=================
	*/
	inline void camera::set_pitch(float value)
	{
		m_Pitch = value;
	}

	/*
	====================
	camera::set_position
	====================
	*/
	inline void camera::set_position(const fvec3 &pos)
	{
		m_Position = pos;
	}


}

#endif