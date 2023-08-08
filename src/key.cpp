#include <DE/key.hpp>

namespace de {
	const Uint8 *Key::m_Keys = nullptr;

	void Key::update() {
		m_Keys = SDL_GetKeyboardState(NULL);
	}

	bool Key::isPressed(key::DKey key) {
		return m_Keys[SDL_GetScancodeFromKey(key)];
	}
}
