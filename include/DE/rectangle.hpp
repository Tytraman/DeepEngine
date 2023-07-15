#ifndef __DEEP_ENGINE_RECTANGLE_HPP__
#define __DEEP_ENGINE_RECTANGLE_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/drawable.hpp>
#include <DE/color.hpp>
#include <DE/vec.hpp>

namespace de {

	class DE_API Rectangle : public ColoredDrawable {

		private:
			vertex _vertices[6];

		public:
			Rectangle() = delete;
			Rectangle(DrawablePanel *panel, const fvec2 &pos, const colora &color, uint32_t width, uint32_t height, bool visible = true);

			void rotate(float degrees) override;
			void draw(const fmat2x2 &baseVectors) override;

			uint32_t getType() const override;

			fvec2 getVertex0Pos() const;
			fvec2 getVertex1Pos() const;
			fvec2 getVertex2Pos() const;
			fvec2 getVertex3Pos() const;
			fvec2 getVertex4Pos() const;
			fvec2 getVertex5Pos() const;

			void setVertex0Pos(const fvec2 &pos);
			void setVertex1Pos(const fvec2 &pos);
			void setVertex2Pos(const fvec2 &pos);
			void setVertex3Pos(const fvec2 &pos);
			void setVertex4Pos(const fvec2 &pos);
			void setVertex5Pos(const fvec2 &pos);

	};

	/*
	==================
	Rectangle::getType
	==================
	*/
	inline uint32_t Rectangle::getType() const
	{
		return DE_DRAWABLE_TYPE_RECTANGLE;
	}

	fvec2 Rectangle::getVertex0Pos() const
	{
		return { _vertices[0].position.x, _vertices[0].position.y };
	}

	fvec2 Rectangle::getVertex1Pos() const
	{
		return { _vertices[1].position.x, _vertices[1].position.y };
	}

	fvec2 Rectangle::getVertex2Pos() const
	{
		return { _vertices[2].position.x, _vertices[2].position.y };
	}

	fvec2 Rectangle::getVertex3Pos() const
	{
		return { _vertices[3].position.x, _vertices[3].position.y };
	}

	fvec2 Rectangle::getVertex4Pos() const
	{
		return { _vertices[4].position.x, _vertices[4].position.y };
	}

	fvec2 Rectangle::getVertex5Pos() const
	{
		return { _vertices[5].position.x, _vertices[5].position.y };
	}

	void Rectangle::setVertex0Pos(const fvec2 &pos)
	{
		_vertices[0].position.x = pos.x;
		_vertices[0].position.y = pos.y;
	}

	void Rectangle::setVertex1Pos(const fvec2 &pos)
	{
		_vertices[1].position.x = pos.x;
		_vertices[1].position.y = pos.y;
	}

	void Rectangle::setVertex2Pos(const fvec2 &pos)
	{
		_vertices[2].position.x = pos.x;
		_vertices[2].position.y = pos.y;
	}

	void Rectangle::setVertex3Pos(const fvec2 &pos)
	{
		_vertices[3].position.x = pos.x;
		_vertices[3].position.y = pos.y;
	}

	void Rectangle::setVertex4Pos(const fvec2 &pos)
	{
		_vertices[4].position.x = pos.x;
		_vertices[4].position.y = pos.y;
	}

	void Rectangle::setVertex5Pos(const fvec2 &pos)
	{
		_vertices[5].position.x = pos.x;
		_vertices[5].position.y = pos.y;
	}

}


#endif