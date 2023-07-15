#ifndef __DEEP_ENGINE_TRIANGLE_HPP__
#define __DEEP_ENGINE_TRIANGLE_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/drawable.hpp>
#include <DE/color.hpp>
#include <DE/vec.hpp>

#include <stdint.h>

namespace de {

	/// @class Triangle
	/// @brief Représente un triangle à dessiner sur l'écran.
	class DE_API Triangle : public ColoredDrawable {

		private:
			vertex _vertices[3];
			float _radius;

		public:
			Triangle() = delete;
			Triangle(DrawablePanel *panel, const fvec2 &centerPos, const colora &color, float radius, bool visible = true);

			void rotate(float degrees) override;
			void draw(const fmat2x2 &baseVectors) override;

			//===== Getters =====//

			uint32_t getType() const override;

			fvec2 getVertex0Pos() const;
			fvec2 getVertex1Pos() const;
			fvec2 getVertex2Pos() const;
			fvec2 getVertexPos(int8_t number) const;

			/// @brief	Calcule l'angle A en degrés.
			/// @return La valeur en degrés de l'angle A.
			float getAngleA() const;
			/// @brief	Calcule l'angle B en degrés.
			/// @return La valeur en degrés de l'angle B.
			float getAngleB() const;
			/// @brief	Calcule l'angle C en degrés.
			/// @return La valeur en degrés de l'angle C.
			float getAngleC() const;

			float getLengthOfAB() const;
			float getLengthOfAC() const;
			float getLengthOfBC() const;

			/// @brief	Vérifie si le triangle est rectangle.
			/// @return L'index de l'angle rectangle ou -1 si le triangle n'est pas rectangle.
			int8_t isRectangle() const;

			float getArea() const;

			//===== Setters =====//

			void setPos(const fvec2 &pos) override;

			void setVertex0Pos(const fvec2 &pos);
			void setVertex1Pos(const fvec2 &pos);
			void setVertex2Pos(const fvec2 &pos);
			void setVerticesPos(const fvec2 &vertex0, const fvec2 &vertex1, const fvec2 &vertex2);

	};

	/*
	=================
	Triangle::getType
	=================
	*/
	inline uint32_t Triangle::getType() const
	{
		return DE_DRAWABLE_TYPE_TRIANGLE;
	}

	/*
	=======================
	Triangle::getVertex0Pos
	=======================
	*/
	inline fvec2 Triangle::getVertex0Pos() const
	{
		return { _vertices[0].position.x, _vertices[0].position.y };
	}

	/*
	=======================
	Triangle::getVertex1Pos
	=======================
	*/
	inline fvec2 Triangle::getVertex1Pos() const
	{
		return { _vertices[1].position.x, _vertices[1].position.y };
	}

	/*
	=======================
	Triangle::getVertex2Pos
	=======================
	*/
	inline fvec2 Triangle::getVertex2Pos() const
	{
		return { _vertices[2].position.x, _vertices[2].position.y };
	}

	/*
	======================
	Triangle::getVertexPos
	======================
	*/
	inline fvec2 Triangle::getVertexPos(int8_t number) const
	{
		return { _vertices[number].position.x, _vertices[number].position.y };
	}

	/*
	=======================
	Triangle::getLengthOfAB
	=======================
	*/
	inline float Triangle::getLengthOfAB() const
	{
		return fvec2::magn(getVertex1Pos() - getVertex0Pos());
	}

	/*
	=======================
	Triangle::getLengthOfAC
	=======================
	*/
	inline float Triangle::getLengthOfAC() const
	{
		return fvec2::magn(getVertex2Pos() - getVertex0Pos());
	}

	/*
	=======================
	Triangle::getLengthOfBC
	=======================
	*/
	inline float Triangle::getLengthOfBC() const
	{
		return fvec2::magn(getVertex2Pos() - getVertex1Pos());
	}

	/*
	=======================
	Triangle::setVertex0Pos
	=======================
	*/
	inline void Triangle::setVertex0Pos(const fvec2 &pos)
	{
		_vertices[0].position.x = pos.x;
		_vertices[0].position.y = pos.y;
	}

	/*
	=======================
	Triangle::setVertex1Pos
	=======================
	*/
	inline void Triangle::setVertex1Pos(const fvec2 &pos)
	{
		_vertices[1].position.x = pos.x;
		_vertices[1].position.y = pos.y;
	}

	/*
	=======================
	Triangle::setVertex2Pos
	=======================
	*/
	inline void Triangle::setVertex2Pos(const fvec2 &pos)
	{
		_vertices[2].position.x = pos.x;
		_vertices[2].position.y = pos.y;
	}

}

#endif