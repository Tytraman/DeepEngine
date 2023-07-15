#ifndef __DEEP_ENGINE_SCENE_HPP__
#define __DEEP_ENGINE_SCENE_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/entity.hpp>

#include <vector>

namespace de {

	class Scene {

		private:
			std::vector<entity> _entities;

		public:
			entity_id addEntity(component_info componentInfo);

	};

}

#endif