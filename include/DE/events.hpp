#ifndef __DEEP_ENGINE_EVENTS_HPP__
#define __DEEP_ENGINE_EVENTS_HPP__

#include <DE/def.h>
#include <DE/types.hpp>

#include <stdint.h>

#include <SDL.h>

namespace de {

	class Window;

	namespace EventType {

		enum : uint32_t {
			Window          = SDL_WINDOWEVENT,
			KeyDown			= SDL_KEYDOWN,
			KeyUp			= SDL_KEYUP,
			MouseMotion     = SDL_MOUSEMOTION,
			MouseButtonDown = SDL_MOUSEBUTTONDOWN,
			MouseButtonUp   = SDL_MOUSEBUTTONUP,
			MouseWheel      = SDL_MOUSEWHEEL,
			Quit			= SDL_QUIT
		};

	}

	enum events : uint32_t {
		WindowResized	= SDL_WINDOWEVENT_RESIZED
	};

	enum MouseButton {
		Left   = SDL_BUTTON_LEFT,
		Middle = SDL_BUTTON_MIDDLE,
		Right  = SDL_BUTTON_RIGHT
	};
	
	/// @struct devent_s
	/// @brief Permet la gestion des évènements système et des fenêtres.
	struct DE_API devent_s {

		static devent_s *create();

		devent_s() = delete;
		devent_s(devent_s &) = delete;
		devent_s(devent_s &&) = delete;
		~devent_s();

		bool pollEvent();

		uint32_t getType() const;

		int32_t getKeysym() const;

		uint8_t getMouseButton() const;
		int getMouseX() const;
		int getMouseY() const;

		int getMouseScrollX() const;
		int getMouseScrollY() const;

		uint32_t getWindowEventType() const;

		size getWindowSize() const;
		

	};

	/// @typedef devent
	/// @brief Pointeur opaque vers la structure \ref devent_s.
	/// 
	/// Les évènements sont gérés en interne par la SDL et afin de rester
	/// uniforme avec DeepEngine, les fonctions et les types de la SDL ne sont
	/// pas exposer.
	typedef struct devent_s *devent;

}

#endif