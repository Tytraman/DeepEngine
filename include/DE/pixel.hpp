#ifndef __DEEP_ENGINE_PIXEL_HPP__
#define __DEEP_ENGINE_PIXEL_HPP__

#include <DE/def.h>
#include <DE/drawable.hpp>
#include <DE/color.hpp>

namespace de {

	class DE_API Pixel : public Drawable {

		private:
			colora _color;

		public:
			Pixel() = delete;
			Pixel(Renderer *renderer, const vec2 &&pos, const colora &&color, bool visible = true);

			void rotate(float degrees) override;
			void draw() override;

			uint32 getType() override;

	};

	/*
	=============
	Pixel::rotate
	=============
	*/
	inline void Pixel::rotate(float degrees) { }

	inline void Pixel::draw() {
		_renderer->setColor(_color);
		_renderer->drawPixel(_pos);
	}

	/*
	==============
	Pixel::getType
	==============
	*/
	inline uint32 Pixel::getType() {
		return DE_DRAWABLE_TYPE_PIXEL;
	}

}

#endif