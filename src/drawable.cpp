#include <DE/drawable.hpp>

/*
==================
Drawable::Drawable
==================
*/
de::Drawable::Drawable(const fvec2 &pos, bool visible)
	: _pos(pos),
	  _visible(visible),
	  _drawVectors(false)
{ }

/*
================================
ColoredDrawable::ColoredDrawable
================================
*/
de::ColoredDrawable::ColoredDrawable(const fvec2 &pos, const colora &color, bool visible)
	: Drawable(pos, visible),
	  _color(color)
{ }
