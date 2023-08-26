#include <DE/window.hpp>
#include <DE/def.h>
#include <DE/core.hpp>
#include <DE/key.hpp>
#include <DE/scene.hpp>
#include <DE/ecs/system.hpp>
#include <DE/rendering/opengl_utils.hpp>

#include <glad/glad.h>

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

namespace de {

	/*
	=============================
	Window::internalEventCallback
	=============================
	*/
	void Window::internalEventCallback(devent e)
	{
		static bool insertPressed = false;

		// Vérifie le type d'évènement qui vient de se produire
		switch(e->getType()) {
			default: break;
			case EventType::KeyDown: {
				switch(e->getKeysym()) {
					default: break;
					case key::Insert: {
						if(!insertPressed) {
							m_ShowDebugPanel = !m_ShowDebugPanel;
							insertPressed = true;
						}
					} break;
				}
			} break;
			case EventType::KeyUp: {
				switch(e->getKeysym()) {
					default: break;
					case key::Insert: {
						if(insertPressed)
							insertPressed = false;
					} break;
				}
			} break;
			case EventType::Window: {
				// Vérifie le type d'évènement de fenêtre qui vient de se produire
				switch(e->getWindowEventType()) {
					default: break;
					case events::WindowResized: {	// Redimension de la fenêtre
						size newSize = e->getWindowSize();
						GLCore::updateViewport(newSize.width, newSize.height);
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
	Window::Window(uint16_t targetMS, uint16_t targetFPS)
		: m_Window(NULL),
		  m_PreEventCallbacks(sizeof(PreEventCallback)),
		  m_EventCallback(defaultInputCallback),
		  m_UpdateCallback(nullptr),
		  m_TargetMSPerUpdate(targetMS),
		  m_TargetFPS(targetFPS),
		  m_Running(false),
		  m_ShowDebugPanel(false)
	{ }

	/*
	==============
	Window::create
	==============
	*/
	ErrorStatus Window::create(Window &win, const char *title, const size &size)
	{
		win.m_Window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.width, size.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if(win.m_Window == NULL)
			return ErrorStatus::CreateWindowSDL;

		
		if(!OpenGLRenderer::create(win.m_Renderer, win)) {
			win.destroy();
			return ErrorStatus::GLCreateContext;
		}

		gladLoadGL();
		glViewport(0, 0, size.width, size.height);

		return ErrorStatus::NoError;
	}

	/*
	===============
	Window::destroy
	===============
	*/
	void Window::destroy()
	{
		m_PreEventCallbacks.free();

		// Détruit la fenêtre
		SDL_DestroyWindow(m_Window);
		m_Window = NULL;
	}

	void __executePreEventCallbacks(List *preEventCallbacks, Window &window)
	{
		size_t length = preEventCallbacks->getNumberOfElements();
		size_t i;
		Window::PreEventCallback callback;

		for(i = 0; i < length; ++i) {
			if(preEventCallbacks->getCopy(i, &callback)) {
				if(callback != nullptr)
					callback(window);
			}
		}
	}

	/*
	===========
	Window::run
	===========
	*/
	void Window::run()
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

		// Met à jour l'état du clavier.
		Key::update();

		// Boucle infinie du jeu
		// TODO: mettre cette boucle autre part que dans la fenêtre car ça n'a pas vraiment de lien.
		uint64_t startTime = Core::getCurrentTimeMillis(), endTime;
		while(m_Running) {
			scene_id sceneID = Scene::getActiveSceneID();

			// Calcule le temps passé à faire la boucle
			current = Core::getTick();		// Récupère le tick actuel.
			elapsed = current - previous;	// Compte combien de temps s'est écoulé entre la dernière itération et maintenant.
			previous = current;				// Sauvegarde le temps actuel pour qu'il devienne le temps précédent à la prochaine itération.
			lag += elapsed;					// Plus le système est lent, et plus le lag sera élevé.

			__executePreEventCallbacks(&m_PreEventCallbacks, *this);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			if(m_ShowDebugPanel)
				ImGuiDebugMenu::render(this);

			// Récupère les évènements système, les entrées utilisateurs etc...
			// et exécute un callback s'il y en a un.
			while((e = pollEvent()) != nullptr) {
				// Gère les évènements internes.
				internalEventCallback(e);

				// Appelle le callback utilisateur.
				if(m_EventCallback != nullptr)
					m_EventCallback(*this, e);

				delete e;
			}

			// Ne met à jour l'état du jeu que si le lag est supérieur au ms visé.
			// m_TargetMSPerUpdate est une constante qui ne doit jamais changer car elle impacte directement
			// la vitesse du jeu et l'entièreté de la physique.
			while(lag >= m_TargetMSPerUpdate) {
				// Exécute la fonction update callback de l'utilisateur.
				if(m_UpdateCallback != nullptr)
					m_UpdateCallback(*this);

				// Exécute les systèmes créés par l'utilisateur.
				SystemManager::executeSystems();

				// Applique l'accélération à toutes les entités en possédant.
				SystemManager::accelerationSystem();

				// Applique la vélocité à toutes les entités en possédant.
				SystemManager::velocitySystem();

				// Les collisions sont vérifiées après tous les déplacements possibles afin d'éviter les problèmes.
				SystemManager::colliderSystem();

				lag -= m_TargetMSPerUpdate;
				updates++;

				// Note: plus on reste longtemps dans cette boucle, et plus le lag sera élevé
			}

			// Fait le rendu final de la frame !
			SystemManager::renderSystem(m_Renderer, sceneID);

			end = Core::getTick();

			cn++;
			endTime = Core::getCurrentTimeMillis();
			elapsed = endTime - startTime;
			if(elapsed >= 1000) {
				// TODO: Retirer au moment de la release.
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
	devent Window::pollEvent() const
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
	void Window::defaultInputCallback(Window &window, devent e)
	{
		switch(e->getType()) {
			default: break;
			case EventType::Window: {
				switch(e->getWindowEventType()) {
					default: break;
					case events::WindowClosed: {
						window.m_Running = false;
					} break;
				}
			} break;
			case EventType::KeyDown: {
				switch(e->getKeysym()) {
					default: break;
					case key::Esc: {
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
	size Window::getSize() const
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
	uint32_t Window::getWidth() const
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
	uint32_t Window::getHeight() const
	{
		int w, h;
		SDL_GetWindowSize(m_Window, &w, &h);
		return h;
	}

	const char *Window::getTitle() const
	{
		return SDL_GetWindowTitle(m_Window);
	}

	void Window::setTitle(const char *title) const
	{
		SDL_SetWindowTitle(m_Window, title);
	}

}

