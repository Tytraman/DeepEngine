#ifndef __DEEP_ENGINE_COMPONENT_HPP__
#define __DEEP_ENGINE_COMPONENT_HPP__

#include <DE/def.h>
#include <DE/types.hpp>

#include <vector>

namespace de {

	typedef uint32_t component_id;
	typedef uint16_t component_mask;

	struct component_info {
		component_id id;
		component_mask mask;
	};

}

#endif