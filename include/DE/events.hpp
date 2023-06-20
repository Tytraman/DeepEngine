#ifndef __DEEP_ENGINE_EVENTS_HPP__
#define __DEEP_ENGINE_EVENTS_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <SDL.h>

namespace de {

	enum events : uint32 {
		Quit	= SDL_QUIT,
		KeyDown = SDL_KEYDOWN,
		KeyUp	= SDL_KEYUP
	};

	enum dkey : sint32 {
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

	struct devent {
		sint32 ev;
		keycode key;
	};

}

#endif