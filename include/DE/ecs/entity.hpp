#ifndef __DEEP_ENGINE_ENTITY_HPP__
#define __DEEP_ENGINE_ENTITY_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/ecs/component.hpp>
#include <DE/memory/list.hpp>

namespace de {

	/// @brief	Identifie une entité dans une scène.
	/// @remark	Chaque entité d'une scène est représenté par un identifiant unique.
	using entity_id = de_id;
	
	using entity_collection_id = de_id;

	/// @brief	Classe permettant la gestion d'une collection d'entités.
	class DE_API EntityManager {

		public:
			EntityManager() = delete;
			EntityManager(const EntityManager &) = delete;
			EntityManager(const EntityManager &&) = delete;

			static entity_collection_id createEntityCollection();

			/// @brief	Crée une entité.
			/// @return L'ID de l'entité.
			static entity_id createEntity(entity_collection_id collection);

			/// @brief		Supprime une entité de la liste.
			/// @param id	L'ID de l'entité à supprimer.
			static void destroyEntity(entity_collection_id collection, entity_id entity);

			/// @brief				Attache un composant à une entité.
			/// @param entity		L'entité à laquelle attacher le composant.
			/// @param component	Composant à attacher.
			/// @return				\c true si le composant à bien été attaché.
			static bool attachComponent(entity_collection_id collection, entity_id entity, component_id component);

			/// @brief							Cherche toutes les entités qui respectent les conditions d'inclusions et les ajoute dans la liste destination.
			/// @param collection				ID de la collection d'entités dans laquelle chercher.
			/// @param componentTypesToInclude	Masque des types de composants que les entités doivent posséder afin d'être incluses.
			/// @param componentTypesToExclude	Masque des types de composants que les entités ne doivent pas avoir pour être incluses.
			/// @param dest						Pointeur vers la liste qui stockera les entités répondant aux conditions.
			static void query(entity_collection_id collection, component_type componentTypesToInclude, component_type componentTypesToExclude, List *dest);

			static component_id getComponentID(entity_collection_id collection, entity_id entity, component_type componentType);

	};

}

#endif