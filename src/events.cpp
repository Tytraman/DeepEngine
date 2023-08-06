#include <DE/events.hpp>
#include <DE/window.hpp>

namespace de {

	struct devent_internal {
		SDL_Event *e;
	};

	devent_s *devent_s::create()
	{
		devent_internal *di = new devent_internal;
		di->e = new SDL_Event;

		return (devent_s *) di;
	}

	devent_s::~devent_s()
	{
		delete ((devent_internal *) this)->e;
	}

	bool devent_s::pollEvent()
	{
		return SDL_PollEvent(((devent_internal *) this)->e);
	}

	uint32_t devent_s::getType() const
	{
		return ((devent_internal *) this)->e->type;
	}

	uint8_t devent_s::getMouseButton() const
	{
		return ((devent_internal *) this)->e->button.button;
	}

	int devent_s::getMouseX() const
	{
		return ((devent_internal *) this)->e->button.x;
	}

	int devent_s::getMouseY() const
	{
		return ((devent_internal *) this)->e->button.y;
	}

	int devent_s::getMouseScrollX() const
	{
		return ((devent_internal *) this)->e->wheel.x;
	}

	int devent_s::getMouseScrollY() const
	{
		return ((devent_internal *) this)->e->wheel.y;
	}

	uint32_t devent_s::getWindowEventType() const
	{
		return ((devent_internal *) this)->e->window.event;
	}

	int32_t devent_s::getKeysym() const
	{
		return ((devent_internal *) this)->e->key.keysym.sym;
	}

	size devent_s::getWindowSize() const
	{
		devent_internal *di = (devent_internal *) this;
		size s;

		s.width = di->e->window.data1;
		s.height = di->e->window.data2;

		return s;
	}

}
