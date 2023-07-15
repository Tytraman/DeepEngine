#ifndef __DEEP_ENGINE_HEALTH_HPP__
#define __DEEP_ENGINE_HEALTH_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/component.hpp>

#include <vector>

namespace de {

	struct DE_API HealthComponent {
		uint32_t pv;
		uint32_t max;

		private:
			static std::vector<HealthComponent> _healthComponents;
	};

}

#endif