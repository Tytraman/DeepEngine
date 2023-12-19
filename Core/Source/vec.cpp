#include <DE/vec_imp.hpp>
#include <DE/renderer.hpp>

/*
==========
vec2::draw
==========
*/
void deep::vec2::draw(renderer &renderer) const
{
	renderer.drawLine(0, 0, x, y);
}

/*
===========
fvec2::draw
===========
*/
void deep::fvec2::draw(renderer &renderer) const
{
	renderer.drawLine(0, 0, (int) x, (int) y);
}
