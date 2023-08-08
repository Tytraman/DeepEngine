#ifndef __DEEP_ENGINE_KEY_HPP__
#define __DEEP_ENGINE_KEY_HPP__

#include <DE/def.h>
#include <DE/types.hpp>

#include <stdint.h>

#include <SDL.h>

namespace de {

	namespace key {
		enum DKey : int32_t {
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

			LCtrl  = SDLK_LCTRL,
			LShift = SDLK_LSHIFT
		};
	}

	class DE_API Key {

		private:
			static const Uint8 *m_Keys;

		public:
			static void update();

			static bool isPressed(key::DKey key);

	};

}

#endif