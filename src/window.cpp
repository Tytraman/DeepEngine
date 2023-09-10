#include <DE/window.hpp>
#include <DE/def.hpp>
#include <DE/core.hpp>
#include <DE/key.hpp>
#include <DE/ecs/scene.hpp>
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
		static bool f11Pressed    = false;
		static int lastWindowWidth  = getWidth();
		static int lastWindowHeight = getHeight();

		// V�rifie le type d'�v�nement qui vient de se produire.
		switch(e->getType()) {
			default: break;
			case EventType::KeyDown: {
				switch(e->getKeysym()) {
					default: break;
					case key::Insert: {
						if(!insertPressed) {
							m_ShowDebugPanel = !m_ShowDebugPanel;
							SDL_SetRelativeMouseMode((SDL_bool) !m_ShowDebugPanel);
							setCursorPos(getWidth() / 2, getHeight() / 2);
							insertPressed = true;
						}
					} break;
					case key::F11: {
						if(!f11Pressed) {
							if(SDL_GetWindowFlags(m_Window) & SDL_WINDOW_FULLSCREEN) {
								SDL_SetWindowFullscreen(m_Window, 0);
								SDL_SetWindowSize(m_Window, lastWindowWidth, lastWindowHeight);
								GLCore::updateViewport(lastWindowWidth, lastWindowHeight);
								setCursorPos(getWidth() / 2, getHeight() / 2);
							}else {
								SDL_DisplayMode DM;
								SDL_GetCurrentDisplayMode(0, &DM);

								lastWindowWidth  = getWidth();
								lastWindowHeight = getHeight();
								
								SDL_SetWindowSize(m_Window, DM.w, DM.h);
								GLCore::updateViewport(DM.w, DM.h);
								SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN);
								setCursorPos(getWidth() / 2, getHeight() / 2);
							}
							f11Pressed = true;
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
					case key::F11: {
						if(f11Pressed)
							f11Pressed = false;
					} break;
				}
			} break;
			case EventType::Window: {
				// V�rifie le type d'�v�nement de fen�tre qui vient de se produire
				switch(e->getWindowEventType()) {
					default: break;
					case events::WindowResized: {	// Redimension de la fen�tre
						size newSize = e->getWindowSize();
						GLCore::updateViewport(newSize.width, newSize.height);
						//printf("Window resized: %dx%d\n", newSize.width, newSize.height);
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

		SDL_SetRelativeMouseMode(SDL_TRUE);
		win.setCursorPos(win.getWidth() / 2, win.getHeight() / 2);
		
		if(!OpenGLRenderer::create(win.m_Renderer, &win)) {
			win.destroy();
			return ErrorStatus::GLCreateContext;
		}

		gladLoadGL();
		glViewport(0, 0, size.width, size.height);

		// Permet de tester la profondeur lors du rendu afin de ne pas superposer les triangles.
		DE_GL_CALL(glEnable(GL_DEPTH_TEST));

		// Active la fonctionnalit� d'OpenGL qui permet de g�rer la transparence des couleurs lorsque n�cessaire.
		DE_GL_CALL(glEnable(GL_BLEND));
		DE_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		DE_GL_CALL(glEnable(GL_CULL_FACE));
		GLCore::setCullFace(GLCullFace::Back);
		DE_GL_CALL(glFrontFace(GL_CCW));

		// Une fois qu'OpenGL a bien �t� initialis�,
		// on cr�e une texture blanche de 1x1 par d�faut.

		uint8_t colors[] = {
			255, 255, 255, 255
		};

		gl_texture whiteTex = GLTexture::create();
		GLTexture::bind(whiteTex, 0);

		GLTexture::setTextureWrappingS(GLTextureWrap::Repeat);
		GLTexture::setTextureWrappingT(GLTextureWrap::Repeat);
		GLTexture::setTextureFiltering(GLTextureFilter::Nearest);

		GLTexture::transmitTexture(colors, 1, 1, ImageColorType::RGBA);

		GLTexture::setWhiteTexture(whiteTex);

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

		// D�truit la fen�tre
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

		// Met � jour l'�tat du clavier.
		Key::update();

		// Boucle infinie du jeu
		// TODO: mettre cette boucle autre part que dans la fen�tre car �a n'a pas vraiment de lien.
		uint64_t startTime = Core::getCurrentTimeMillis(), endTime;
		while(m_Running) {
			scene_id sceneID = Scene::getActiveSceneID();

			// Calcule le temps pass� � faire la boucle
			current = Core::getTick();		// R�cup�re le tick actuel.
			elapsed = current - previous;	// Compte combien de temps s'est �coul� entre la derni�re it�ration et maintenant.
			previous = current;				// Sauvegarde le temps actuel pour qu'il devienne le temps pr�c�dent � la prochaine it�ration.
			lag += elapsed;					// Plus le syst�me est lent, et plus le lag sera �lev�.

			__executePreEventCallbacks(&m_PreEventCallbacks, *this);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			if(m_ShowDebugPanel)
				ImGuiDebugMenu::render(this);

			// R�cup�re les �v�nements syst�me, les entr�es utilisateurs etc...
			// et ex�cute un callback s'il y en a un.
			while((e = pollEvent()) != nullptr) {
				// G�re les �v�nements internes.
				internalEventCallback(e);

				// Appelle le callback utilisateur.
				if(m_EventCallback != nullptr)
					m_EventCallback(*this, e);

				delete e;
			}

			// Ne met � jour l'�tat du jeu que si le lag est sup�rieur au ms vis�.
			// m_TargetMSPerUpdate est une constante qui ne doit jamais changer car elle impacte directement
			// la vitesse du jeu et l'enti�ret� de la physique.
			while(lag >= m_TargetMSPerUpdate) {
				// Ex�cute la fonction update callback de l'utilisateur.
				if(m_UpdateCallback != nullptr)
					m_UpdateCallback(*this);

				// Ex�cute les syst�mes cr��s par l'utilisateur.
				SystemManager::executeSystems();

				// Applique l'acc�l�ration � toutes les entit�s en poss�dant.
				SystemManager::accelerationSystem();

				// Applique la v�locit� � toutes les entit�s en poss�dant.
				SystemManager::velocitySystem();

				// Les collisions sont v�rifi�es apr�s tous les d�placements possibles afin d'�viter les probl�mes.
				SystemManager::colliderSystem();

				lag -= m_TargetMSPerUpdate;
				updates++;

				// Note: plus on reste longtemps dans cette boucle, et plus le lag sera �lev�
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

			/*elapsed = end - current;
			if(elapsed < desiredDelta)
				Core::sleep((uint32_t) ((uint64_t ) desiredDelta - elapsed));*/
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

	void Window::setCursorPos(int x, int y)
	{
		SDL_WarpMouseInWindow(m_Window, x, y);
	}

}

