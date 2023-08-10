#include <DE/ecs/entity.hpp>
#include <DE/ecs/component.hpp>

#include <unordered_map>

namespace de {

	/// @struct Entity
	/// @brief	Une entité peut contenir plusieurs composants et est représentée par un ID.
	struct EntityItem {
		List components;				///< Contient tous les composants que l'entité possède.
		component_type componentsType;	///< Masque des types de composants attribués à l'entité.

		EntityItem();
	};

	static entity_collection_id m_NextCollectionID = 0;														///< Prochain ID de collection à attribuer.
	static std::unordered_map<entity_collection_id, std::unordered_map<entity_id, EntityItem>> m_Collections;	///< Collection d'entités.
	static std::unordered_map<entity_collection_id, entity_id> m_NextEntityID;								///< Contient le prochain ID d'entité à attribuer dans une collection.

	EntityItem::EntityItem()
		: components(sizeof(component_id)), componentsType(0)
	{ }

	Entity::Entity(entity_collection_id collectionID, entity_id entityID)
		: m_CollectionID(collectionID),
		  m_EntityID(entityID)
	{ }

	/*
	=====================================
	EntityManager::createEntityCollection
	=====================================
	*/
	entity_collection_id EntityManager::createEntityCollection()
	{
		entity_collection_id id = m_NextCollectionID;

		m_Collections[id] = std::unordered_map<entity_id, EntityItem>();

		m_NextCollectionID++;

		return id;
	}

	/*
	===========================
	EntityManager::createEntity
	===========================
	*/
	Entity EntityManager::createEntity(entity_collection_id collection)
	{
		auto &entityCollection = m_Collections[collection];

		entity_id id = m_NextEntityID[collection];
		entityCollection[id].components = List(sizeof(component_id));

		m_NextEntityID[collection]++;

		return Entity(collection, id);
	}

	/*
	============================
	EntityManager::destroyEntity
	============================
	*/
	void EntityManager::destroyEntity(const Entity &entity)
	{
		auto &entityCollection = m_Collections[entity.m_CollectionID];

		// Libère la mémoire utilisée par la liste.
		entityCollection[entity.m_EntityID].components.free();
		entityCollection.erase(entity.m_EntityID);
	}

	/*
	==============================
	EntityManager::attachComponent
	==============================
	*/
	bool EntityManager::attachComponent(const Entity &entity, component_id component)
	{
		const auto &itCollection = m_Collections.find(entity.m_CollectionID);
		if(itCollection == m_Collections.end())
			return false;

		const auto &ent = itCollection->second.find(entity.m_EntityID);
		if(ent == itCollection->second.end())
			return false;

		component_type type = ComponentManager::getType(component);

		// Vérifie que l'entité ne possède pas déjà un composant du même type.
		if((ent->second.componentsType & type) == type)
			return false;

		ent->second.components.addCopy(&component);
		ent->second.componentsType |= type;

		return true;
	}

	/*
	=============================
	EntityManager::getComponentID
	=============================
	*/
	component_id EntityManager::getComponentID(entity_collection_id collectionID, entity_id entityID, component_type componentType)
	{
		const auto &itCollection = m_Collections.find(collectionID);
		if(itCollection == m_Collections.end())
			return badID;

		const auto &ent = itCollection->second.find(entityID);
		if(ent == itCollection->second.end())
			return badID;

		size_t i, length = ent->second.components.getNumberOfElements();
		component_id component;
		List *list = &ent->second.components;
		component_type type;

		// Itère à travers tous les composants de l'entité pour trouver celui qui a le bon type.
		for(i = 0; i < length; ++i) {
			if(list->getCopy(i, &component)) {
				type = ComponentManager::getType(component);
				if(type == componentType)
					return component;
			}
		}

		return badID;
	}

	/*
	====================
	EntityManager::query
	====================
	*/
	void EntityManager::query(entity_collection_id collection, component_type componentTypesToInclude, component_type componentTypesToExclude, List *dest)
	{
		auto &entityCollection = m_Collections[collection];
			
		for(const auto &it : entityCollection) {
			if((it.second.componentsType & componentTypesToInclude) == componentTypesToInclude && (it.second.componentsType & componentTypesToExclude) == 0)
				dest->addCopy((mem_ptr)  &it.first);
		}
	}

}