#ifndef __DEEP_ENGINE_ENTITY_HPP__
#define __DEEP_ENGINE_ENTITY_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/component.hpp>

#include <queue>

namespace de {

	/// @brief	Identifie une entité dans une scène.
	/// @remark	Chaque entité d'une scène est représenté par un identifiant unique.
	typedef uint32_t entity_id;

	struct entity {
		entity_id id;
		component_info componentInfo;
	};

	/// @brief	Classe permettant la gestion d'une collection d'entités.
	class EntityManager {

		private:
			entity_id _maxEntities;						///< Indique le nombre d'entités maximum disponibles.
			entity_id _entityCount;						///< Comptabilise le nombre d'entités actuellement stockées.
			std::queue<entity_id> _availableEntities;	///< File contenant tous les ID d'entités encore disponibles.

		public:
			EntityManager() = delete;
			EntityManager(entity_id maxEntities);

			/// @brief	Crée une entité.
			/// @return L'ID de l'entité.
			entity_id createEntity();

			bool destroyEntity(entity_id id);
	};

}

#endif