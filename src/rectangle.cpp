#include <DE/rectangle.hpp>

/*
====================
Rectangle::Rectangle
====================
*/
de::Rectangle::Rectangle(DrawablePanel *panel, const fvec2 &pos, const colora &color, uint32_t width, uint32_t height, bool visible)
	: ColoredDrawable(panel, pos, color, visible)
{
	_vertices[0].color.r = _vertices[1].color.r = _vertices[2].color.r = _vertices[3].color.r = _vertices[4].color.r = _vertices[5].color.r = color.R;
	_vertices[0].color.g = _vertices[1].color.g = _vertices[2].color.g = _vertices[3].color.g = _vertices[4].color.g = _vertices[5].color.g = color.G;
	_vertices[0].color.b = _vertices[1].color.b = _vertices[2].color.b = _vertices[3].color.b = _vertices[4].color.b = _vertices[5].color.b = color.B;
	_vertices[0].color.a = _vertices[1].color.a = _vertices[2].color.a = _vertices[3].color.a = _vertices[4].color.a = _vertices[5].color.a = color.A;

	// Position de base
	_vertices[0].position.x = pos.x;
	_vertices[0].position.y = pos.y;

	// Base + width
	_vertices[1].position.x = pos.x + width;
	_vertices[1].position.y = pos.y;
	_vertices[4].position = _vertices[1].position;

	// Base + height
	_vertices[2].position.x = pos.x;
	_vertices[2].position.y = pos.y + height;
	_vertices[3].position = _vertices[2].position;

	// Base + width + height
	_vertices[5].position.x = _vertices[1].position.x;
	_vertices[5].position.y = _vertices[2].position.y;
}

/*
=================
Rectangle::rotate
=================
*/
void de::Rectangle::rotate(float degrees)
{

}

/*
===============
Rectangle::draw
===============
*/
void de::Rectangle::draw(const fmat2x2 &baseVectors)
{
	Renderer *renderer = _panel->getRenderer();

	fvec2 basePos0 = getVertex0Pos();
	fvec2 basePos1 = getVertex1Pos();
	fvec2 basePos2 = getVertex2Pos();
	fvec2 basePos3 = getVertex3Pos();
	fvec2 basePos4 = getVertex4Pos();
	fvec2 basePos5 = getVertex5Pos();

	setVertex0Pos(basePos0 * baseVectors);
	setVertex1Pos(basePos1 * baseVectors);
	setVertex2Pos(basePos2 * baseVectors);
	setVertex5Pos(basePos5 * baseVectors);

	_vertices[4].position = _vertices[1].position;
	_vertices[3].position = _vertices[2].position;

	renderer->drawShape(_vertices, 6);

	setVertex0Pos(basePos0);
	setVertex1Pos(basePos1);
	setVertex2Pos(basePos2);
	setVertex3Pos(basePos3);
	setVertex4Pos(basePos4);
	setVertex5Pos(basePos5);
}
