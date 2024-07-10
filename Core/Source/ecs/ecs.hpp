#ifndef __DEEP_ENGINE_ECS_HPP__
#define __DEEP_ENGINE_ECS_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "maths/vec.hpp"
#include "graphics/shape.hpp"
#include "core/list.hpp"

namespace deep
{

	/// @brief	Identifie une entit� dans une sc�ne.
	/// @remark	Chaque entit� d'une sc�ne est repr�sent� par un identifiant unique.
	using entity_id            = de_id;
	using entity_collection_id = de_id;

	using component_id         = de_id;                    ///< Identificateur de composant.

	using system_function      = void (*)(list<entity_id> &query);  ///< Pointeur vers une fonction qu'un syst�me doit ex�cuter.
	using system_id            = de_id;                             ///< ID repr�sentant un syst�me.

	using scene_id = de_id;

	using collider_in_callback = void (*)(entity_collection_id collectionID, entity_id entity1, entity_id entity2, const vec2<float> &difference, const rect &collision);
	using collider_out_callback = void (*)(entity_collection_id collectionID, entity_id entity1, entity_id entity2, const vec2<float> &difference);

}

#endif