#include <DE/pixel.hpp>

de::Pixel::Pixel(DrawablePanel *panel, const fvec2 &pos, const colora &color, bool visible)
	: ColoredDrawable(panel, pos, color, visible)
{ }

void de::Pixel::draw(const fmat2x2 &baseVectors) {
	Renderer *renderer = _panel->getRenderer();
	fvec2 pos = _pos * baseVectors;

	renderer->setColor(_color);
	renderer->drawPixel(pos);
}
