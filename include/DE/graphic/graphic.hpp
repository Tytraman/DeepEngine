#ifndef __DEEP_ENGINE_GRAPHIC_HPP__
#define __DEEP_ENGINE_GRAPHIC_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/scene.hpp>
#include <DE/color.hpp>
#include <DE/ecs/entity.hpp>

namespace de {

	class DE_API Graphic {

		public:
			static entity_id createRectangle(scene_id scene, const fvec2 &position, float width, float height, const colora &color);
			static entity_id createTriangle(scene_id scene, const fvec2 &position, float radius, const colora &color);

	};

}

#endif