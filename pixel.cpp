#include <DE/pixel.hpp>

de::Pixel::Pixel(Renderer *renderer, const vec2 &&pos, const colora &&color, bool visible)
	: Drawable(renderer, pos, visible), _color(color)
{ }
