#ifndef __DEEP_ENGINE_ECS_HPP__
#define __DEEP_ENGINE_ECS_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "maths/vec.hpp"
#include "graphics/shape.hpp"
#include "core/list.hpp"

namespace deep
{

	/// @brief	Identifie une entité dans une scène.
	/// @remark	Chaque entité d'une scène est représenté par un identifiant unique.
	using entity_id            = de_id;
	using entity_collection_id = de_id;

	using component_id         = de_id;                    ///< Identificateur de composant.

	using system_function      = void (*)(list<entity_id> &query);  ///< Pointeur vers une fonction qu'un système doit exécuter.
	using system_id            = de_id;                             ///< ID représentant un système.

	using scene_id = de_id;

	using collider_in_callback = void (*)(entity_collection_id collectionID, entity_id entity1, entity_id entity2, const vec2<float> &difference, const rect &collision);
	using collider_out_callback = void (*)(entity_collection_id collectionID, entity_id entity1, entity_id entity2, const vec2<float> &difference);

}

#endif