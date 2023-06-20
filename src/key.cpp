#include <DE/key.hpp>

const Uint8 *de::Key::_keys = nullptr;

void de::Key::update() {
	_keys = SDL_GetKeyboardState(NULL);
}
