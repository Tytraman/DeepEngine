#include <DE/window.hpp>
#include <DE/def.h>
#include <DE/core.hpp>
#include <DE/key.hpp>

struct enum_panel_callback_s {
	de::size currentSize;
	de::size newSize;
};

void __enumerate_callback(de::Panel *panel, void *args)
{
	enum_panel_callback_s *enm = (enum_panel_callback_s *) args;

	if(panel->getType() != de::PanelType::Window) {
		de::size currentSize = panel->getSize();
		float hPercent = 100.0f * enm->newSize.width / enm->currentSize.width;
		float vPercent = 100.0f * enm->newSize.height / enm->currentSize.height;

		panel->setSize(de::size(currentSize.width * hPercent / 100.0f, currentSize.height * vPercent / 100.0f));
	}
}

/*
=============================
Window::internalEventCallback
=============================
*/
void de::Window::internalEventCallback(devent e)
{
	// Vérifie le type d'évènement qui vient de se produire
	switch(e->getType()) {
		default: break;
		case de::EventType::Window: {
			// Vérifie le type d'évènement de fenêtre qui vient de se produire
			switch(e->getWindowEventType()) {
				default: break;
				case de::events::WindowResized: {	// Redimension de la fenêtre
					size newSize = e->getWindowSize();

					enum_panel_callback_s s = {
						_size,
						newSize
					};

					setSize(newSize);

					enumeratePanels(__enumerate_callback, &s);
				} break;
			}
		} break;
	}
}

/*
==============
Window::Window
==============
*/
de::Window::Window(uint16_t targetMSPerUpdate, const size &size)
	: Panel(nullptr, PanelType::Window, size),
	  _window(NULL),
	  _eventCallback(defaultInputCallback),
	  _updateCallback(nullptr),
	  _targetMSPerUpdate(targetMSPerUpdate),
	  _running(false)
{ }

/*
==============
Window::create
==============
*/
de::ErrorStatus de::Window::create(Window &win, const char *title)
{
	win._panels = new std::vector<Panel *>();

	win._window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, win._size.width, win._size.height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(win._window == NULL)
		return ErrorStatus::CreateWindowSDL;

	return ErrorStatus::NoError;
}

/*
===============
Window::destroy
===============
*/
void de::Window::destroy()
{
	// Détruit tous les panels
	size_t i;
	for(i = 0; i < _panels->size(); ++i) {
		(*_panels)[i]->destroy();
		(*_panels)[i] = nullptr;
	}
	delete _panels;
	_panels = nullptr;

	// Détruit la fenêtre
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
	uint64_t lag = 0;
	uint64_t previous = Core::getTick();
	uint64_t current;
	uint64_t elapsed;
	size_t iPanel;

	_running = true;

	// Boucle infinie du jeu
	// TODO: mettre cette boucle autre part que dans la fenêtre car ça n'a pas vraiment de lien
	while(_running) {
		// Calcule le temps passé à faire la boucle
		current = Core::getTick();		// Récupère le tick actuel.
		elapsed = current - previous;	// Compte combien de temps s'est écoulé entre la dernière itération et maintenant.
		previous = current;				// Sauvegarde le temps actuel pour qu'il devienne le temps précédent à la prochaine itération.
		lag += elapsed;					// Plus le système est lent, et plus le lag sera élevé.

		// Récupère les évènements système, les entrées utilisateurs etc...
		// et exécute un callback s'il y en a un.
		while((e = pollEvent()) != nullptr) {
			// Gère les évènements internes
			internalEventCallback(e);

			// Appelle le callback s'il y en a un spécifié
			if(_eventCallback != nullptr)
				_eventCallback(*this, e);

			delete e;
		}

		// Met à jour l'état du clavier.
		Key::update();

		// Ne met à jour l'état du jeu que si le lag est supérieur au ms visé.
		// _targetMSPerUpdate est une constante qui ne doit jamais changer car elle impacte directement
		// la vitesse du jeu et l'entièreté de la physique
		while(lag >= _targetMSPerUpdate) {
			if(_updateCallback != nullptr)
				_updateCallback(*this);
			lag -= _targetMSPerUpdate;

			// Note: plus on reste longtemps dans cette boucle, et plus le lag sera élevé
		}

		// Nettoie tous les panels et fait le rendu
		// Fait le rendu à l'écran.
		// A un impact direct sur les performances du jeu et le lag peut considérablement augmenter.
		for(iPanel = 0; iPanel < _panels->size(); ++iPanel) {
			Panel *panel = (*_panels)[iPanel];
			if(panel->getType() == PanelType::Drawable) {
				((DrawablePanel *) panel)->clear();
				((DrawablePanel *) panel)->render();
			}
		}
	}
}

/*
=================
Window::pollEvent
=================
*/
de::devent de::Window::pollEvent() const
{
	devent e = devent_s::create();
	if(!e->pollEvent()) {
		delete e;
		return nullptr;
	}
	return e;
}

/*
============================
Window::defaultInputCallback
============================
*/
void de::Window::defaultInputCallback(Window &window, devent e)
{
	switch(e->getType()) {
		default: break;
		case de::events::Quit: {
			window._running = false;
		} break;
		case de::events::KeyDown: {
			switch(e->getKeysym()) {
				default: break;
				case de::dkey::Esc: {
					window._running = false;
				} break;
			}
		} break;
	}
}

/*
===============
Window::getSize
===============
*/
de::size de::Window::getSize() const
{
	size windowSize;
	SDL_GetWindowSize(_window, (int *) &windowSize.width, (int *) &windowSize.height);
	return windowSize;
}

/*
================
Window::getWidth
================
*/
uint32_t de::Window::getWidth() const
{
	int w, h;
	SDL_GetWindowSize(_window, &w, &h);
	return w;
}

/*
=================
Window::getHeight
=================
*/
uint32_t de::Window::getHeight() const
{
	int w, h;
	SDL_GetWindowSize(_window, &w, &h);
	return h;
}
