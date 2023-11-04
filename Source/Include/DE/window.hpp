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

namespace de
{

	/// @class window
	/// @brief Permet la gestion d'une fenêtre.
	class DE_API window
    {

		public:
			using pre_event_callback = void (*)(window &window);

			typedef void (*event_callback)(window &window, devent e);
			typedef void (*update_callback)(window &window);

		public:
			window(uint16_t targetMS, uint16_t targetFPS);
			
			/// @brief			Crée une fenêtre avec un titre et une taille.
			/// @param win		La fenêtre à créer.
			/// @param title	Titre de la fenêtre.
			/// @param size		Taille de la fenêtre.
			/// @return			Le code d'erreur.
			static error_status create(window &win, const char *title, const size &size);
			
			/// @brief Détruit la fenêtre ainsi que tous ses composants internes.
			void destroy();
			
			/// @brief Game Loop
			void run();

			bool addPreEventCallback(pre_event_callback callback);
			
			/// @brief			Récupère et retire l'évènement le plus vieux de la queue.
			/// @return			Un \ref de::devent lorsqu'un évènement s'est produit ou \c nullptr s'il n'y a aucun évènement.
			/// @remark			La valeur retournée par cette méthode doit être \c delete.
			devent pollEvent() const;

			void setCursorPos(int x, int y);

			static void defaultInputCallback(window &window, devent e);


			//===== GETTERS =====//

			SDL_Window *getWindow();
			size getSize() const;
			uint32_t getWidth() const;
			uint32_t getHeight() const;
			const char *getTitle() const;
			OpenGLRenderer &getRenderer();
			bool isShowingDebugPanel() const;


			//===== SETTERS =====//

			void setEventCallback(event_callback callabck);
			void setUpdateCallback(update_callback callback);
			void setTitle(const char *title) const;
			void setShowingDebugPanel(bool value);

        private:
			SDL_Window              *m_Window;
			list<pre_event_callback> m_PreEventCallbacks;
			event_callback           m_EventCallback;
			update_callback          m_UpdateCallback;
			OpenGLRenderer           m_Renderer;
			uint16_t                 m_TargetMSPerUpdate;
			uint16_t                 m_TargetFPS;
			bool                     m_Running;
			bool                     m_ShowDebugPanel;

			void internalEventCallback(devent e);

	};

	/*
	===========================
	window::addPreEventCallback
	===========================
	*/
	inline bool window::addPreEventCallback(pre_event_callback callback)
	{
		return m_PreEventCallbacks.add(callback);
	}

	/*
	=================
	window::getWindow
	=================
	*/
	inline SDL_Window *window::getWindow()
	{
		return m_Window;
	}

	/*
	===================
	window::getRenderer
	===================
	*/
	inline OpenGLRenderer &window::getRenderer()
	{
		return m_Renderer;
	}

	/*
	===========================
	window::isShowingDebugPanel
	===========================
	*/
	inline bool window::isShowingDebugPanel() const
	{
		return m_ShowDebugPanel;
	}

	/*
	========================
	window::setEventCallback
	========================
	*/
	inline void window::setEventCallback(event_callback callback)
	{
		m_EventCallback = callback;
	}

	/*
	=========================
	window::setUpdateCallback
	=========================
	*/
	inline void window::setUpdateCallback(update_callback callback)
	{
		m_UpdateCallback = callback;
	}

	/*
	============================
	window::setShowingDebugPanel
	============================
	*/
	inline void window::setShowingDebugPanel(bool value)
	{
		if(value)
			im_gui_debug_menu::addWindow(this);
		else
			im_gui_debug_menu::removeWindow(this);
	}

}

#endif