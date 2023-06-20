#include <DE/vec.hpp>
#include <DE/renderer.hpp>

/*
===========
fvec2::draw
===========
*/
void de::fvec2::draw(Renderer &renderer) const
{
	renderer.drawLine(0, 0, (int) x, (int) y);
}
