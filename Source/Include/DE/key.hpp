#ifndef __DEEP_ENGINE_KEY_HPP__
#define __DEEP_ENGINE_KEY_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>

#include <stdint.h>

#include <SDL.h>

namespace deep
{
	
	enum class dkey
    {
		LeftArrow = SDLK_LEFT,
		RightArrow = SDLK_RIGHT,
		UpArrow = SDLK_UP,
		DownArrow = SDLK_DOWN,

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

		Esc    = SDLK_ESCAPE,
		LCtrl  = SDLK_LCTRL,
		LShift = SDLK_LSHIFT,
		Space  = SDLK_SPACE,
		Insert = SDLK_INSERT,

        F2  = SDLK_F2,
		F11 = SDLK_F11,
		F12 = SDLK_F12

	};

	class DE_API key
    {

		public:
			static void update();

			static bool isPressed(dkey key);

        private:
			static const Uint8 *m_Keys;

	};

}

#endif