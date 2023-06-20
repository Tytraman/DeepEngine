#ifndef __DEEP_ENGINE_WINDOW_HPP__
#define __DEEP_ENGINE_WINDOW_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/color.hpp>
#include <DE/error.hpp>
#include <DE/renderer.hpp>
#include <DE/events.hpp>
#include <DE/drawable.hpp>

#include <string>
#include <vector>
#include <memory>

namespace de {

	class DE_API Window {

		friend Renderer;

		public:
			typedef void (*EventCallback)(Window &window, devent e);
			typedef void (*UpdateCallback)(Window &window);

		private:
			Renderer _renderer;
			colora _backColor;
			std::vector<Drawable *> *_drawables;
			SDL_Window *_window;
			EventCallback _eventCallback;
			UpdateCallback _updateCallback;
			uint16 _targetMSPerUpdate;
			bool _running;

		public:
			Window(uint16 targetFPS);
			~Window();
			
			/// @brief			Crée une fenêtre avec un titre et une taille.
			/// @param win		La fenêtre à créer.
			/// @param title	Titre de la fenêtre.
			/// @param size		Taille de la fenêtre.
			/// @return			Le code d'erreur.
			static ErrorStatus create(Window &win, const char *title, size &&size);
			
			/// @brief Détruit la fenêtre ainsi que tous ses composants internes.
			void destroy();
			
			/// @brief Nettoie toute la surface de dessin en la remplissant de la couleur définie.
			void clear();
			
			/// @brief Game Loop
			void run();
			
			/// @brief Fait le rendu graphique de tous les éléments de la fenêtre.
			void render();
			
			/// @brief			Récupère et retire l'évènement le plus vieux de la queue.
			/// @param event	Variable qui stockera le dernier évènement de la queue.
			/// @return			@ref true tant qu'il y a un évènement dans la queue.
			bool pollEvent(devent &event) const;

			static void defaultInputCallback(Window &window, devent e);

			void addDrawable(Drawable *drawable);

			Drawable *getDrawable(size_t index);


			//===== GETTERS =====//

			SDL_Window *getWindow();
			Renderer &getRenderer();
			Renderer *getRendererPtr();
			colora getBackColor() const;
			size getSize() const;


			//===== SETTERS =====//

			void setEventCallback(EventCallback callabck);
			void setUpdateCallback(UpdateCallback callback);
	};

	/*
	==============
	Window::Window
	==============
	*/
	inline Window::Window(uint16 targetMSPerUpdate)
		: _drawables(new std::vector<Drawable *>()), _window(NULL), _running(false), _eventCallback(defaultInputCallback), _updateCallback(nullptr), _targetMSPerUpdate(targetMSPerUpdate)
	{ }

	/*
	===============
	Window::~Window
	===============
	*/
	inline Window::~Window()
	{
		delete _drawables;
	}

	/*
	===================
	Window::addDrawable
	===================
	*/
	inline void Window::addDrawable(Drawable *drawable)
	{
		_drawables->push_back(drawable);
	}

	/*
	===================
	Window::getDrawable
	===================
	*/
	inline Drawable *Window::getDrawable(size_t index)
	{
		return (*_drawables)[index];
	}

	/*
	=============
	Window::clear
	=============
	*/
	inline void Window::clear()
	{
		_renderer.setColor(_backColor);
		_renderer.clear();
	}

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
	===================
	Window::getRenderer
	===================
	*/
	inline Renderer &Window::getRenderer()
	{
		return _renderer;
	}

	/*
	======================
	Window::getRendererPtr
	======================
	*/
	inline Renderer *Window::getRendererPtr()
	{
		return &_renderer;
	}

	/*
	====================
	Window::getBackColor
	====================
	*/
	inline colora Window::getBackColor() const
	{
		return _backColor;
	}

	/*
	===============
	Window::getSize
	===============
	*/
	inline size Window::getSize() const
	{
		size windowSize;
		SDL_GetWindowSize(_window, (int *) & windowSize.width, (int *) & windowSize.height);
		return windowSize;
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