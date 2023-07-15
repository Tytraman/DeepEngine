#include <DE/vec.hpp>
#include <DE/renderer.hpp>

/*
==========
vec2::draw
==========
*/
void de::vec2::draw(Renderer &renderer) const
{
	renderer.drawLine(0, 0, x, y);
}

/*
===========
fvec2::draw
===========
*/
void de::fvec2::draw(Renderer &renderer) const
{
	renderer.drawLine(0, 0, (int) x, (int) y);
}
