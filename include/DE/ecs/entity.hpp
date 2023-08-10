#ifndef __DEEP_ENGINE_ENTITY_HPP__
#define __DEEP_ENGINE_ENTITY_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/ecs/ecs.hpp>
#include <DE/memory/list.hpp>

namespace de {

	class EntityManager;

	class DE_API Entity {

		private:
			entity_collection_id  m_CollectionID;
			entity_id m_EntityID;

			friend EntityManager;

		public:
			Entity() = delete;

			static Entity bad();

			bool isOK() const;

			entity_collection_id getCollectionID() const;
			entity_id getEntityID() const;

		protected:
			Entity(entity_collection_id collectionID, entity_id entityID);

	};

	inline Entity Entity::bad()
	{
		return Entity(badID, badID);
	}

	inline bool Entity::isOK() const
	{
		return (m_CollectionID != badID && m_EntityID != badID);
	}

	inline scene_id Entity::getCollectionID() const
	{
		return m_CollectionID;
	}

	inline entity_id Entity::getEntityID() const
	{
		return m_EntityID;
	}

	/// @brief	Classe permettant la gestion d'une collection d'entités.
	class DE_API EntityManager {

		public:
			EntityManager() = delete;
			EntityManager(const EntityManager &) = delete;
			EntityManager(const EntityManager &&) = delete;

			static entity_collection_id createEntityCollection();

			/// @brief	Crée une entité.
			/// @return L'entité nouvellement créée.
			static Entity createEntity(entity_collection_id collection);

			/// @brief		Supprime une entité de la liste.
			static void destroyEntity(const Entity &entity);

			/// @brief				Attache un composant à une entité.
			/// @param entity		L'entité à laquelle attacher le composant.
			/// @return				\c true si le composant à bien été attaché.
			static bool attachComponent(const Entity &entity, component_id component);

			/// @brief							Cherche toutes les entités qui respectent les conditions d'inclusions et les ajoute dans la liste destination.
			/// @param collection				ID de la collection d'entités dans laquelle chercher.
			/// @param componentTypesToInclude	Masque des types de composants que les entités doivent posséder afin d'être incluses.
			/// @param componentTypesToExclude	Masque des types de composants que les entités ne doivent pas avoir pour être incluses.
			/// @param dest						Pointeur vers la liste qui stockera les entités répondant aux conditions.
			static void query(entity_collection_id collection, component_type componentTypesToInclude, component_type componentTypesToExclude, List *dest);

			static component_id getComponentID(entity_collection_id collectionID, entity_id entityID, component_type componentType);
			static component_id getComponentID(const Entity &entity, component_type componentType);

	};

	inline component_id EntityManager::getComponentID(const Entity &entity, component_type componentType)
	{
		return getComponentID(entity.m_CollectionID, entity.m_EntityID, componentType);
	}

}

#endif