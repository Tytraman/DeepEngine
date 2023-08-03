#include <DE/ecs/entity.hpp>

#include <unordered_map>

namespace de {

	/// @struct Entity
	/// @brief	Une entité peut contenir plusieurs composants et est représentée par un ID.
	struct Entity {
		List components;				///< Contient tous les composants que l'entité possède.
		component_type componentsType;	///< Masque des types de composants attribués à l'entité.

		Entity();
	};

	static entity_collection_id m_NextCollectionID = 0;														///< Prochain ID de collection à attribuer.
	static std::unordered_map<entity_collection_id, std::unordered_map<entity_id, Entity>> m_Collections;	///< Collection d'entités.
	static std::unordered_map<entity_collection_id, entity_id> m_NextEntityID;								///< Contient le prochain ID d'entité à attribuer dans une collection.

	Entity::Entity()
		: components(sizeof(component_id)), componentsType(0)
	{ }

	/*
	=====================================
	EntityManager::createEntityCollection
	=====================================
	*/
	entity_collection_id EntityManager::createEntityCollection()
	{
		entity_collection_id id = m_NextCollectionID;

		m_Collections[id] = std::unordered_map<entity_id, Entity>();

		m_NextCollectionID++;

		return id;
	}

	/*
	===========================
	EntityManager::createEntity
	===========================
	*/
	entity_id EntityManager::createEntity(entity_collection_id collection)
	{
		auto &entityCollection = m_Collections[collection];

		entity_id id = m_NextEntityID[collection];
		entityCollection[id].components = List(sizeof(component_id));

		m_NextEntityID[collection]++;

		return id;
	}

	/*
	============================
	EntityManager::destroyEntity
	============================
	*/
	void EntityManager::destroyEntity(entity_collection_id collection, entity_id entity)
	{
		auto &entityCollection = m_Collections[collection];

		// Libère la mémoire utilisée par la liste.
		entityCollection[entity].components.free();
		entityCollection.erase(entity);
	}

	/*
	==============================
	EntityManager::attachComponent
	==============================
	*/
	bool EntityManager::attachComponent(entity_collection_id collection, entity_id entity, component_id component)
	{
		const auto &itCollection = m_Collections.find(collection);
		if(itCollection == m_Collections.end())
			return false;

		const auto &ent = itCollection->second.find(entity);
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
	component_id EntityManager::getComponentID(entity_collection_id collection, entity_id entity, component_type componentType)
	{
		const auto &itCollection = m_Collections.find(collection);
		if(itCollection == m_Collections.end())
			return badID;

		const auto &ent = itCollection->second.find(entity);
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