#include <DE/key.hpp>

namespace de
{

	const Uint8 *key::m_Keys = nullptr;

	void key::update()
    {
		m_Keys = SDL_GetKeyboardState(NULL);
	}

	bool key::isPressed(dkey key)
    {
		return m_Keys[SDL_GetScancodeFromKey(static_cast<SDL_Scancode>(key))];
	}
}
