#include <DE/drawable.hpp>

/*
==================
Drawable::Drawable
==================
*/
de::Drawable::Drawable(DrawablePanel *panel, const fvec2 &pos, bool visible)
	: _pos(pos),
	  _visible(visible),
	  _drawVectors(false),
	  _panel(panel)
{ }

/*
==============
Drawable::move
==============
*/
void de::Drawable::move(direction::Direction direction, int value)
{
	switch(direction) {
		default: return;
		case direction::Left: {
			_pos.x -= value;
		} return;
		case direction::Right: {
			_pos.x += value;
		} return;
		case direction::Up: {
			_pos.y -= value;
		} return;
		case direction::Down: {
			_pos.y += value;
		} return;
	}
}

/*
================================
ColoredDrawable::ColoredDrawable
================================
*/
de::ColoredDrawable::ColoredDrawable(DrawablePanel *panel, const fvec2 &pos, const colora &color, bool visible)
	: Drawable(panel, pos, visible),
	  _color(color)
{ }
