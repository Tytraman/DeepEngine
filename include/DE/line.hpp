#ifndef __DEEP_ENGINE_LINE_HPP__
#define __DEEP_ENGINE_LINE_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/drawable.hpp>
#include <DE/vec.hpp>

namespace de {

	class DE_API Line : public ColoredDrawable {

		private:
			fvec2 _p2;

		public:
			Line() = delete;
			Line(DrawablePanel *panel, const fvec2 &p1, const fvec2 &p2, const colora &color, bool visible = true);

			void draw(const fmat2x2 &baseVectors) override;
			void rotate(float degrees) override;
			uint32_t getType() const override;

			void setPos2(const fvec2 &pos);

	};

	/*
	=============
	Line::getType
	=============
	*/
	inline uint32_t Line::getType() const
	{
		return DE_DRAWABLE_TYPE_LINE;
	}

	/*
	=============
	Line::setPos2
	=============
	*/
	inline void Line::setPos2(const fvec2 &pos)
	{
		_p2 = pos;
	}

}


#endif