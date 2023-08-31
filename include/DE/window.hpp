#ifndef __DEEP_ENGINE_WINDOW_HPP__
#define __DEEP_ENGINE_WINDOW_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/error.hpp>
#include <DE/events.hpp>
#include <DE/renderer.hpp>
#include <DE/memory/list.hpp>
#include <DE/imgui/deimgui.hpp>

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

#include <SDL.h>

namespace de {

	/// @class Window
	/// @brief Permet la gestion d'une fenêtre.
	class DE_API Window {

		public:
			using PreEventCallback = void (*)(Window &window);

			typedef void (*EventCallback)(Window &window, devent e);
			typedef void (*UpdateCallback)(Window &window);

		private:
			SDL_Window      *m_Window;
			List             m_PreEventCallbacks;
			EventCallback    m_EventCallback;
			UpdateCallback   m_UpdateCallback;
			OpenGLRenderer   m_Renderer;
			uint16_t         m_TargetMSPerUpdate;
			uint16_t         m_TargetFPS;
			bool             m_Running;
			bool             m_ShowDebugPanel;

		public:
			Window(uint16_t targetMS, uint16_t targetFPS);
			
			/// @brief			Crée une fenêtre avec un titre et une taille.
			/// @param win		La fenêtre à créer.
			/// @param title	Titre de la fenêtre.
			/// @param size		Taille de la fenêtre.
			/// @return			Le code d'erreur.
			static ErrorStatus create(Window &win, const char *title, const size &size);
			
			/// @brief Détruit la fenêtre ainsi que tous ses composants internes.
			void destroy();
			
			/// @brief Game Loop
			void run();

			bool addPreEventCallback(PreEventCallback callback);
			
			/// @brief			Récupère et retire l'évènement le plus vieux de la queue.
			/// @return			Un \ref de::devent lorsqu'un évènement s'est produit ou \c nullptr s'il n'y a aucun évènement.
			/// @remark			La valeur retournée par cette méthode doit être \c delete.
			devent pollEvent() const;

			void setCursorPos(int x, int y);

			static void defaultInputCallback(Window &window, devent e);


			//===== GETTERS =====//

			SDL_Window *getWindow();
			size getSize() const;
			uint32_t getWidth() const;
			uint32_t getHeight() const;
			const char *getTitle() const;
			OpenGLRenderer &getRenderer();
			bool isShowingDebugPanel() const;


			//===== SETTERS =====//

			void setEventCallback(EventCallback callabck);
			void setUpdateCallback(UpdateCallback callback);
			void setTitle(const char *title) const;
			void setShowingDebugPanel(bool value);

		private:
			void internalEventCallback(devent e);

	};

	/*
	===========================
	Window::addPreEventCallback
	===========================
	*/
	inline bool Window::addPreEventCallback(PreEventCallback callback)
	{
		return m_PreEventCallbacks.addCopy(&callback);
	}

	/*
	=================
	Window::getWindow
	=================
	*/
	inline SDL_Window *Window::getWindow()
	{
		return m_Window;
	}

	/*
	===================
	Window::getRenderer
	===================
	*/
	inline OpenGLRenderer &Window::getRenderer()
	{
		return m_Renderer;
	}

	/*
	===========================
	Window::isShowingDebugPanel
	===========================
	*/
	inline bool Window::isShowingDebugPanel() const
	{
		return m_ShowDebugPanel;
	}

	/*
	========================
	Window::setEventCallback
	========================
	*/
	inline void Window::setEventCallback(EventCallback callback)
	{
		m_EventCallback = callback;
	}

	/*
	=========================
	Window::setUpdateCallback
	=========================
	*/
	inline void Window::setUpdateCallback(UpdateCallback callback)
	{
		m_UpdateCallback = callback;
	}

	/*
	============================
	Window::setShowingDebugPanel
	============================
	*/
	inline void Window::setShowingDebugPanel(bool value)
	{
		if(value)
			ImGuiDebugMenu::addWindow(this);
		else
			ImGuiDebugMenu::removeWindow(this);
	}

}

#endif