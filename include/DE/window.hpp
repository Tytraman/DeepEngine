#ifndef __DEEP_ENGINE_WINDOW_HPP__
#define __DEEP_ENGINE_WINDOW_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/error.hpp>
#include <DE/events.hpp>
#include <DE/panel.hpp>

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

namespace de {

	/// @class Window
	/// @brief Permet la gestion d'une fenêtre.
	class DE_API Window : public Panel {

		friend Renderer;

		public:
			typedef void (*EventCallback)(Window &window, devent e);
			typedef void (*UpdateCallback)(Window &window);

		private:
			SDL_Window *_window;
			EventCallback _eventCallback;
			UpdateCallback _updateCallback;
			uint16_t _targetMSPerUpdate;
			bool _running;

		public:
			Window(uint16_t targetFPS, const size &size);
			
			/// @brief			Crée une fenêtre avec un titre et une taille.
			/// @param win		La fenêtre à créer.
			/// @param title	Titre de la fenêtre.
			/// @param size		Taille de la fenêtre.
			/// @return			Le code d'erreur.
			static ErrorStatus create(Window &win, const char *title);
			
			/// @brief Détruit la fenêtre ainsi que tous ses composants internes.
			void destroy();
			
			/// @brief Game Loop
			void run();
			
			/// @brief			Récupère et retire l'évènement le plus vieux de la queue.
			/// @return			Un \ref de::devent lorsqu'un évènement s'est produit ou \c nullptr s'il n'y a aucun évènement.
			/// @remark			La valeur retournée par cette méthode doit être \c delete.
			de::devent pollEvent() const;

			static void defaultInputCallback(Window &window, devent e);


			//===== GETTERS =====//

			SDL_Window *getWindow();
			size getSize() const;
			uint32_t getWidth() const;
			uint32_t getHeight() const;


			//===== SETTERS =====//

			void setEventCallback(EventCallback callabck);
			void setUpdateCallback(UpdateCallback callback);

		private:
			void internalEventCallback(devent e);
	};

	

	/*
	=================
	Window::getWindow
	=================
	*/
	inline SDL_Window *Window::getWindow()
	{
		return _window;
	}

	/*
	========================
	Window::setEventCallback
	========================
	*/
	inline void Window::setEventCallback(EventCallback callback)
	{
		_eventCallback = callback;
	}

	/*
	=========================
	Window::setUpdateCallback
	=========================
	*/
	inline void Window::setUpdateCallback(UpdateCallback callback)
	{
		_updateCallback = callback;
	}

}

#endif