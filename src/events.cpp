#include <DE/events.hpp>
#include <DE/window.hpp>

struct devent_internal {
	SDL_Event *e;
};

de::devent_s *de::devent_s::create()
{
	devent_internal *di = new devent_internal;
	di->e = new SDL_Event;

	return (devent_s *) di;
}

de::devent_s::~devent_s()
{
	delete ((devent_internal *) this)->e;
}

bool de::devent_s::pollEvent()
{
	return SDL_PollEvent(((devent_internal *) this)->e);
}

uint32_t de::devent_s::getType() const
{
	return ((devent_internal *) this)->e->type;
}

uint32_t de::devent_s::getWindowEventType() const
{
	return ((devent_internal *) this)->e->window.event;
}

int32_t de::devent_s::getKeysym() const
{
	return ((devent_internal *) this)->e->key.keysym.sym;
}

de::size de::devent_s::getWindowSize() const
{
	devent_internal *di = (devent_internal *) this;
	size s;

	s.width = di->e->window.data1;
	s.height = di->e->window.data2;

	return s;
}
