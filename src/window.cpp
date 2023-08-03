#include <DE/window.hpp>
#include <DE/def.h>
#include <DE/core.hpp>
#include <DE/key.hpp>
#include <DE/scene.hpp>
#include <DE/ecs/system.hpp>

/*
=============================
Window::internalEventCallback
=============================
*/
void de::Window::internalEventCallback(devent e)
{
	// V�rifie le type d'�v�nement qui vient de se produire
	switch(e->getType()) {
		default: break;
		case de::EventType::Window: {
			// V�rifie le type d'�v�nement de fen�tre qui vient de se produire
			switch(e->getWindowEventType()) {
				default: break;
				case de::events::WindowResized: {	// Redimension de la fen�tre
					size newSize = e->getWindowSize();

					// TODO: ajouter du code...
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
de::Window::Window(uint16_t targetMS, uint16_t targetFPS)
	: m_Window(NULL),
	  m_EventCallback(defaultInputCallback),
	  m_UpdateCallback(nullptr),
	  m_TargetMSPerUpdate(targetMS),
	  m_TargetFPS(targetFPS),
	  m_Running(false)
{ }

/*
==============
Window::create
==============
*/
de::ErrorStatus de::Window::create(Window &win, const char *title, const size &size)
{
	win.m_Window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.width, size.height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(win.m_Window == NULL)
		return ErrorStatus::CreateWindowSDL;

	if(Renderer::create(win.m_Renderer, win) != ErrorStatus::NoError) {
		win.destroy();
		return ErrorStatus::CreateRendererSDL;
	}

	return ErrorStatus::NoError;
}

/*
===============
Window::destroy
===============
*/
void de::Window::destroy()
{
	// D�truit la fen�tre
	SDL_DestroyWindow(m_Window);
	m_Window = NULL;
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
	uint64_t end;
	uint64_t current;
	uint64_t elapsed;
	uint32_t cn = 0;
	uint32_t updates = 0;
	uint16_t desiredDelta = 1000 / m_TargetFPS;
	std::string title(getTitle());

	m_Running = true;

	// Met � jour l'�tat du clavier.
	Key::update();

	// Boucle infinie du jeu
	// TODO: mettre cette boucle autre part que dans la fen�tre car �a n'a pas vraiment de lien
	uint64_t startTime = Core::getCurrentTimeMillis(), endTime;
	while(m_Running) {
		scene_id sceneID = Scene::getActiveSceneID();

		// Calcule le temps pass� � faire la boucle
		current = Core::getTick();		// R�cup�re le tick actuel.
		elapsed = current - previous;	// Compte combien de temps s'est �coul� entre la derni�re it�ration et maintenant.
		previous = current;				// Sauvegarde le temps actuel pour qu'il devienne le temps pr�c�dent � la prochaine it�ration.
		lag += elapsed;					// Plus le syst�me est lent, et plus le lag sera �lev�.

		// R�cup�re les �v�nements syst�me, les entr�es utilisateurs etc...
		// et ex�cute un callback s'il y en a un.
		while((e = pollEvent()) != nullptr) {
			// G�re les �v�nements internes
			internalEventCallback(e);

			// Appelle le callback s'il y en a un sp�cifi�
			if(m_EventCallback != nullptr)
				m_EventCallback(*this, e);

			delete e;
		}

		// Ne met � jour l'�tat du jeu que si le lag est sup�rieur au ms vis�.
		// _targetMSPerUpdate est une constante qui ne doit jamais changer car elle impacte directement
		// la vitesse du jeu et l'enti�ret� de la physique
		while(lag >= m_TargetMSPerUpdate) {
			if(m_UpdateCallback != nullptr)
				m_UpdateCallback(*this);

			SystemManager::executeSystems();

			lag -= m_TargetMSPerUpdate;
			updates++;

			// Note: plus on reste longtemps dans cette boucle, et plus le lag sera �lev�
		}

		SystemManager::renderSystem(m_Renderer);

		// Nettoie tous les panels et fait le rendu
		// Fait le rendu � l'�cran.
		// A un impact direct sur les performances du jeu et le lag peut consid�rablement augmenter.
		/*for(iPanel = 0; iPanel < _panels->size(); ++iPanel) {
			Panel *panel = (*_panels)[iPanel];
			if(panel->getType() == PanelType::Drawable) {
				((DrawablePanel *) panel)->clear();
				((DrawablePanel *) panel)->render();
			}
		}*/

		end = Core::getTick();

		cn++;
		endTime = Core::getCurrentTimeMillis();
		elapsed = endTime - startTime;
		if(elapsed >= 1000) {
			setTitle((title + " | UPS: " + std::to_string(updates) + " | FPS: " + std::to_string(cn)).c_str());
			cn = 0;
			updates = 0;
			startTime = endTime;
		}

		elapsed = end - current;
		if(elapsed < desiredDelta)
			Core::sleep((uint32_t) ((uint64_t ) desiredDelta - elapsed));
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
			window.m_Running = false;
		} break;
		case de::events::KeyDown: {
			switch(e->getKeysym()) {
				default: break;
				case de::dkey::Esc: {
					window.m_Running = false;
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
	SDL_GetWindowSize(m_Window, (int *) &windowSize.width, (int *) &windowSize.height);
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
	SDL_GetWindowSize(m_Window, &w, &h);
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
	SDL_GetWindowSize(m_Window, &w, &h);
	return h;
}

const char *de::Window::getTitle() const
{
	return SDL_GetWindowTitle(m_Window);
}

void de::Window::setTitle(const char *title) const
{
	SDL_SetWindowTitle(m_Window, title);
}
