#include <DE/drawable.hpp>

/*
==================
drawable::drawable
==================
*/
deep::drawable::drawable(const fvec2 &pos, bool visible)
	: m_Pos(pos),
	  m_Visible(visible),
	  m_DrawVectors(false)
{ }

/*
================================
ColoredDrawable::ColoredDrawable
================================
*/
deep::ColoredDrawable::ColoredDrawable(const fvec2 &pos, const colora &color, bool visible)
	: drawable(pos, visible),
	  m_Color(color)
{ }
