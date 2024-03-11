#include "DE/maths/vec_imp.hpp"
#include "DE/graphics/renderer.hpp"

/*
==========
ivec2::draw
==========
*/
void deep::ivec2::draw(renderer &renderer) const
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
