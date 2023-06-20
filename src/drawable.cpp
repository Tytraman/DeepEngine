#include <DE/drawable.hpp>

/*
==================
Drawable::Drawable
==================
*/
de::Drawable::Drawable(Renderer *renderer, const vec2 &pos, bool visible)
	: _renderer(renderer), _pos(pos), _visible(visible), _drawVectors(false)
{ }

/*
==================
Drawable::Drawable
==================
*/
de::Drawable::Drawable(Renderer *renderer, const vec2 &&pos, bool visible)
	: _renderer(renderer), _pos(pos), _visible(visible), _drawVectors(false)
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
