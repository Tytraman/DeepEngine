#ifndef __DEEP_ENGINE_KEY_HPP__
#define __DEEP_ENGINE_KEY_HPP__

#include <DE/def.h>
#include <DE/types.hpp>

#include <stdint.h>

#include <SDL.h>

namespace de {

	namespace key {
		enum DKey : int32_t {
			LeftArrow = SDL_SCANCODE_LEFT,
			RightArrow = SDL_SCANCODE_RIGHT,
			UpArrow = SDL_SCANCODE_UP,
			DownArrow = SDL_SCANCODE_DOWN
		};
	}

	class DE_API Key {

		private:
			static const Uint8 *_keys;

		public:
			static void update();

			static bool isPressed(key::DKey key);

	};

	inline bool Key::isPressed(key::DKey key) {
		return _keys[key];
	}

}

#endif