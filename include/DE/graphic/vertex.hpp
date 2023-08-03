#ifndef __DEEP_ENGINE_VERTEX_HPP__
#define __DEEP_ENGINE_VERTEX_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/vec.hpp>

namespace de {

	struct DE_API Vertex {
		fvec2 pos;
		colora color;
	};

}

#endif