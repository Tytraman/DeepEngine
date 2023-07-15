#ifndef __DEEP_ENGINE_PIXEL_HPP__
#define __DEEP_ENGINE_PIXEL_HPP__

#include <DE/def.h>
#include <DE/drawable.hpp>
#include <DE/color.hpp>

#include <stdint.h>

namespace de {

	class DE_API Pixel : public ColoredDrawable {

		public:
			Pixel() = delete;
			Pixel(DrawablePanel *panel, const fvec2 &pos, const colora &color, bool visible = true);

			void rotate(float degrees) override;
			void draw(const fmat2x2 &baseVectors) override;

			uint32_t getType() const override;

	};

	/*
	=============
	Pixel::rotate
	=============
	*/
	inline void Pixel::rotate(float degrees) { }

	/*
	==============
	Pixel::getType
	==============
	*/
	inline uint32_t Pixel::getType() const {
		return DE_DRAWABLE_TYPE_PIXEL;
	}

}

#endif