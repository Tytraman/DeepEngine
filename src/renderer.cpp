#include <DE/renderer.hpp>
#include <DE/window.hpp>


/*
==================
Renderer::Renderer
==================
*/
de::Renderer::Renderer()
	: _renderer(NULL)
{ }

/*
================
Renderer::create
================
*/
de::ErrorStatus de::Renderer::create(Renderer &dest, const Window &window)
{
	dest._renderer = SDL_CreateRenderer(window._window, -1, SDL_RENDERER_ACCELERATED);
	if(dest._renderer == NULL)
		return ErrorStatus::CreateRendererSDL;

	return ErrorStatus::NoError;
}

/*
=======================
Renderer::drawRectangle
=======================
*/
void de::Renderer::drawRectangle(const rect &&rect, bool fill) const
{
	SDL_Rect r {
		r.x = rect.pos.x,
		r.y = rect.pos.y,
		r.w = rect.dim.width,
		r.h = rect.dim.height
	};

	if(fill)
		SDL_RenderFillRect(_renderer, &r);
	else
		SDL_RenderDrawRect(_renderer, &r);
}

/*
===============
Renderer::clear
===============
*/
void de::Renderer::clear() const
{
		SDL_RenderClear(_renderer);
}

/*
==================
Renderer::setColor
==================
*/
void de::Renderer::setColor(const colora &color)
{
	_color = color;
	SDL_SetRenderDrawColor(_renderer, color.R, color.G, color.B, color.A);
}

/*
==================
Renderer::setColor
==================
*/
void de::Renderer::setColor(const colora &&color)
{
	_color = color;
	SDL_SetRenderDrawColor(_renderer, color.R, color.G, color.B, color.A);
}

/*
===================
Renderer::drawPixel
===================
*/
void de::Renderer::drawPixel(const vec2 &pos) const
{
	SDL_RenderDrawPoint(_renderer, pos.x, pos.y);
}

/*
===================
Renderer::drawPixel
===================
*/
void de::Renderer::drawPixel(const vec2 &&pos) const
{
	SDL_RenderDrawPoint(_renderer, pos.x, pos.y);
}

/*
==================
Renderer::drawLine
==================
*/
void de::Renderer::drawLine(int x1, int y1, int x2, int y2) const
{
	SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);
}

/*
===================
Renderer::drawShape
===================
*/
void de::Renderer::drawShape(const vertex *vertices, int verticesNumber) const
{
	SDL_RenderGeometry(_renderer, nullptr, vertices, verticesNumber, nullptr, 0);
}

/*
=====================
Renderer::swapBuffers
=====================
*/
void de::Renderer::swapBuffers() const
{
	SDL_RenderPresent(_renderer);
}

/*
=================
Renderer::destroy
=================
*/
void de::Renderer::destroy()
{
	SDL_DestroyRenderer(_renderer);
	_renderer = NULL;
}


