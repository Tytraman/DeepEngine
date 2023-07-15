#include <DE/line.hpp>

/*
==========
Line::Line
==========
*/
de::Line::Line(DrawablePanel *panel, const fvec2 &p1, const fvec2 &p2, const colora &color, bool visible)
	: ColoredDrawable(panel, p1, color, visible), _p2(p2)
{ }

/*
==========
Line::draw
==========
*/
void de::Line::draw(const fmat2x2 &baseVectors)
{
	Renderer *renderer = _panel->getRenderer();
	fvec2 pos1 = _pos * baseVectors;
	fvec2 pos2 = _p2 * baseVectors;

	renderer->setColor(_color);
	renderer->drawLine((int) pos1.x, (int) pos1.y, (int) pos2.x, (int) pos2.y);
	if(_drawVectors) {
		pos1.draw(*renderer);
		pos2.draw(*renderer);
	}
}

/*
============
Line::rotate
============
*/
void de::Line::rotate(float degrees)
{

}
