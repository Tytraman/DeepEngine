#include <DE/triangle.hpp>

#include <math.h>

/*
==================
Triangle::Triangle
==================
*/
de::Triangle::Triangle(Renderer *renderer, const vec2 &&centerPos, const colora &&color, float radius, bool visible)
	: Drawable(renderer, centerPos, visible), _color(color), _radius(radius)
{
	fvec2 vec0(0.0f, radius);
	fvec2 vec1 = vec0 * fmat2x2::rotate(120.0f);
	fvec2 vec2 = vec0 * fmat2x2::rotate(240.0f);

	_vertices[0].color.r = _vertices[1].color.r = _vertices[2].color.r = color.R;
	_vertices[0].color.g = _vertices[1].color.g = _vertices[2].color.g = color.G;
	_vertices[0].color.b = _vertices[1].color.b = _vertices[2].color.b = color.B;
	_vertices[0].color.a = _vertices[1].color.a = _vertices[2].color.a = color.A;

	setVertex0Pos(vec0 + centerPos);
	setVertex1Pos(vec1 + centerPos);
	setVertex2Pos(vec2 + centerPos);
}

/*
==============
Triangle::draw
==============
*/
void de::Triangle::draw()
{
	_renderer->drawShape(_vertices, 3);
	if(_drawVectors) {
		fvec2 vec0 = getVertex0Pos();
		fvec2 vec1 = getVertex1Pos();
		fvec2 vec2 = getVertex2Pos();

		vec0.draw(*_renderer);
		vec1.draw(*_renderer);
		vec2.draw(*_renderer);
	}
}

/*
================
Triangle::rotate
================
*/
void de::Triangle::rotate(float degrees)
{
	fmat2x2 rotation = fmat2x2::rotate(degrees);
	setVerticesPos(getVertex0Pos() * rotation, getVertex1Pos() * rotation, getVertex2Pos() * rotation);
}

/*
===================
Triangle::getAngleA
===================
*/
float de::Triangle::getAngleA() const
{
	fvec2 B = getVertex1Pos() - getVertex0Pos();
	fvec2 C = getVertex2Pos() - getVertex0Pos();

	return fvec2::angle(B, C);
}

/*
===================
Triangle::getAngleB
===================
*/
float de::Triangle::getAngleB() const
{
	fvec2 A = getVertex0Pos() - getVertex1Pos();
	fvec2 C = getVertex2Pos() - getVertex1Pos();

	return fvec2::angle(A, C);
}

/*
===================
Triangle::getAngleC
===================
*/
float de::Triangle::getAngleC() const
{
	fvec2 A = getVertex0Pos() - getVertex2Pos();
	fvec2 B = getVertex1Pos() - getVertex2Pos();

	return fvec2::angle(A, B);
}

/*
=====================
Triangle::isRectangle
=====================
*/
de::int8 de::Triangle::isRectangle() const
{
	if(getAngleA() == 90.0f)
		return 0;
	if(getAngleB() == 90.0f)
		return 1;
	if(getAngleC() == 90.0f)
		return 2;
	return -1;
}

/*
=================
Triangle::getArea
=================
*/
float de::Triangle::getArea() const
{
	int8 rectangle = isRectangle();
	fvec2 a, b, c;

	// Si le triangle est rectangle, la formule est très simple
	switch(rectangle) {
		default: {
			float AB = getLengthOfAB();
			float AC = getLengthOfAC();
			float BC = getLengthOfBC();
			float p = (AB + AC + BC) / 2.0f;
			return sqrtf(p * (p - AB) * (p - AC) * (p - BC));
		}
		case 0: {
			a = getVertex1Pos();
			b = getVertex2Pos();
			c = getVertex0Pos();
		} break;
		case 1: {
			a = getVertex0Pos();
			b = getVertex2Pos();
			c = getVertex1Pos();
		} break;
		case 2: {
			a = getVertex0Pos();
			b = getVertex1Pos();
			c = getVertex2Pos();
		} break;
	}

	return 0.5f * fvec2::magn(a - c) * fvec2::magn(b - c);
}

/*
================
Triangle::setPos
================
*/
void de::Triangle::setPos(const vec2 &pos)
{
	Drawable::setPos(pos);
	// TODO: mettre à jour la position des vertices
}

/*
========================
Triangle::setVerticesPos
========================
*/
void de::Triangle::setVerticesPos(const fvec2 &vertex0, const fvec2 &vertex1, const fvec2 &vertex2)
{
	_vertices[0].position.x = vertex0.x;
	_vertices[0].position.y = vertex0.y;

	_vertices[1].position.x = vertex1.x;
	_vertices[1].position.y = vertex1.y;

	_vertices[2].position.x = vertex2.x;
	_vertices[2].position.y = vertex2.y;

	// TODO: mettre à jour la position pour qu'elle soit au centre du triangle
}
