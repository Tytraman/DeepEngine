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
			MouseMotion     = SDL_MOUSEMOTION,
			MouseButtonDown = SDL_MOUSEBUTTONDOWN,
			MouseButtonUp   = SDL_MOUSEBUTTONUP,
			MouseWheel      = SDL_MOUSEWHEEL
		};

	}

	enum events : uint32_t {
		Quit			= SDL_QUIT,
		KeyDown			= SDL_KEYDOWN,
		KeyUp			= SDL_KEYUP,
		WindowResized	= SDL_WINDOWEVENT_RESIZED
	};

	enum MouseButton {
		Left   = SDL_BUTTON_LEFT,
		Middle = SDL_BUTTON_MIDDLE,
		Right  = SDL_BUTTON_RIGHT
	};

	enum dkey : int32_t {
		Backspace = SDLK_BACKSPACE,
		Esc = SDLK_ESCAPE,

		A = SDLK_a,
		B = SDLK_b,
		C = SDLK_c,
		D = SDLK_d,
		E = SDLK_e,
		F = SDLK_f,
		G = SDLK_g,
		H = SDLK_h,
		I = SDLK_i,
		J = SDLK_j,
		K = SDLK_k,
		L = SDLK_l,
		M = SDLK_m,
		N = SDLK_n,
		O = SDLK_o,
		P = SDLK_p,
		Q = SDLK_q,
		R = SDLK_r,
		S = SDLK_s,
		T = SDLK_t,
		U = SDLK_u,
		V = SDLK_v,
		W = SDLK_w,
		X = SDLK_x,
		Y = SDLK_y,
		Z = SDLK_z,

		RightArrow = SDLK_RIGHT,
		LeftArrow = SDLK_LEFT,
		DownArrow = SDLK_DOWN,
		UpArrow = SDLK_UP
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

		uint8_t getMouseButton() const;
		int getMouseX() const;
		int getMouseY() const;

		int getMouseScrollX() const;
		int getMouseScrollY() const;

		uint32_t getWindowEventType() const;

		int32_t getKeysym() const;
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