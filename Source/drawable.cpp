#include <DE/drawable.hpp>

/*
==================
drawable::drawable
==================
*/
de::drawable::drawable(const fvec2 &pos, bool visible)
	: m_Pos(pos),
	  m_Visible(visible),
	  m_DrawVectors(false)
{ }

/*
================================
ColoredDrawable::ColoredDrawable
================================
*/
de::ColoredDrawable::ColoredDrawable(const fvec2 &pos, const colora &color, bool visible)
	: drawable(pos, visible),
	  m_Color(color)
{ }
