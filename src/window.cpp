#include <DE/window.hpp>
#include <DE/core.hpp>
#include <DE/key.hpp>

/*
==============
Window::create
==============
*/
de::ErrorStatus de::Window::create(Window &win, const char *title, size &&size)
{
	ErrorStatus errorStatus;

	win._window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.width, size.height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(win._window == NULL)
		return ErrorStatus::CreateWindowSDL;

	errorStatus = Renderer::create(win._renderer, win);
	if(errorStatus != ErrorStatus::NoError)
		return errorStatus;

	return ErrorStatus::NoError;
}

/*
===============
Window::destroy
===============
*/
void de::Window::destroy()
{
	_renderer.destroy();
	SDL_DestroyWindow(_window);
	_window = NULL;
}

/*
===========
Window::run
===========
*/
void de::Window::run()
{
	devent e;
	uint64 lag = 0;
	uint64 previous = Core::getTick();
	uint64 current;
	uint64 elapsed;

	_running = true;

	// Boucle infinie de la fenêtre de jeu
	while(_running) {
		// Calcule le temps passé à faire la boucle
		current = Core::getTick();
		elapsed = current - previous;
		previous = current;
		lag += elapsed;

		// Récupère les évènements système, les entrées utilisateurs etc...
		// et exécute un callback s'il y en a un
		while(pollEvent(e))
			if(_eventCallback != nullptr)
				_eventCallback(*this, e);
		Key::update();

		while(lag >= _targetMSPerUpdate) {
			if(_updateCallback != nullptr)
				_updateCallback(*this);
			lag -= _targetMSPerUpdate;
		}

		// Nettoie l'écran
		clear();
		// Fait le rendu à l'écran
		render();
	}
}

/*
==============
Window::render
==============
*/
void de::Window::render()
{
	size_t i;
	for(i = 0; i < _drawables->size(); ++i)
		if((*_drawables)[i]->_visible)
			(*_drawables)[i]->draw();
	_renderer.swapBuffers();
}

/*
=================
Window::pollEvent
=================
*/
bool de::Window::pollEvent(devent &event) const
{
	SDL_Event e;
	bool ret = SDL_PollEvent(&e);
	event.ev = e.type;
	event.key = e.key.keysym.sym;
	return ret;
}

/*
============================
Window::defaultInputCallback
============================
*/
void de::Window::defaultInputCallback(Window &window, devent e)
{
	switch(e.ev) {
		default: break;
		case de::events::Quit: {
			window._running = false;
		} break;
		case de::events::KeyDown: {
			switch(e.key) {
				default: break;
				case de::dkey::Esc: {
					window._running = false;
				} break;
			}
		} break;
	}
}
