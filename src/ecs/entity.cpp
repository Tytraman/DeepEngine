#include <DE/ecs/entity.hpp>
#include <DE/ecs/component.hpp>

namespace de {

	/// @struct Entity
	/// @brief	Une entité peut contenir plusieurs composants et est représentée par un ID.
	struct EntityItem {
		List components;               ///< Contient tous les composants que l'entité possède.
		component_type componentsType; ///< Masque des types de composants attribués à l'entité.

		EntityItem();
	};

	static entity_collection_id m_NextCollectionID = 0;                                                          ///< Prochain ID de collection à attribuer.

	HashTable EntityManager::m_Collections(sizeof(HashTable), 50);
	HashTable EntityManager::m_NextEntityID(sizeof(entity_id), 50);
	HashTable EntityManager::m_EntitiesToDelete(sizeof(List), 50);

	/*
	======================
	EntityItem::EntityItem
	======================
	*/
	EntityItem::EntityItem()
		: components(sizeof(component_id)), componentsType(0)
	{ }

	/*
	==============
	Entity::Entity
	==============
	*/
	Entity::Entity(entity_collection_id collectionID, entity_id entityID)
		: m_CollectionID(collectionID),
		  m_EntityID(entityID)
	{ }

	/*
	===========================
	EntityManager::enumEntities
	===========================
	*/
	void EntityManager::enumEntities(entity_collection_id collection, EntityEnumCallback callback)
	{
		HashTable *col = (HashTable *) m_Collections.getPtr(collection);
		if(col == nullptr)
			return;

		for(mem_ptr ptr : *col)
			callback(collection, *((entity_id *) ptr));

		deleteEntities();
	}

	/*
	==================================
	EntityManager::getNumberOfEntities
	==================================
	*/
	size_t EntityManager::getNumberOfEntities(entity_collection_id collection)
	{
		HashTable *col = (HashTable *) m_Collections.getPtr(collection);
		if(col == nullptr)
			return 0;

		return ((HashTable *) col)->getNumberOfElements();
	}

	/*
	=====================================
	EntityManager::createEntityCollection
	=====================================
	*/
	entity_collection_id EntityManager::createEntityCollection()
	{
		entity_collection_id id = m_NextCollectionID;
		entity_id entID = 0;
		HashTable table(sizeof(EntityItem), 3000);

		m_NextEntityID.insertCopy(id, &entID);
		m_Collections.insertCopy(id, &table);

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
		HashTable *col = (HashTable *) m_Collections.getPtr(collection);

		entity_id *id = (entity_id *) m_NextEntityID.getPtr(collection);
		entity_id  di = *id;
		(*id)++;

		EntityItem item;
		EntityItem *ptr = (EntityItem *) col->insertCopy(di, &item);

		return Entity(collection, di);
	}

	/*
	============================
	EntityManager::destroyEntity
	============================
	*/
	void EntityManager::destroyEntity(const Entity &entity)
	{
		mem_ptr col = m_Collections.getPtr(entity.m_CollectionID);

		// Libère la mémoire utilisée par la liste.
		((EntityItem *) ((HashTable *) col)->getPtr(entity.m_EntityID))->components.free();

		((HashTable *) col)->destroy(entity.m_EntityID);
	}

	/*
	============================
	EntityManager::destroyEntity
	============================
	*/
	void EntityManager::destroyEntity(entity_collection_id collection, entity_id entity)
	{
		HashTable *col = (HashTable *) m_Collections.getPtr(collection);
		EntityItem *item = (EntityItem *) col->getPtr(entity);

		item->components.free();
		col->destroy(entity);
	}

	/*
	=================================
	EntityManager::destroyAllEntities
	=================================
	*/
	void EntityManager::destroyAllEntities(entity_collection_id collection)
	{
		HashTable *col = (HashTable *) m_Collections.getPtr(collection);
		EntityItem *item;

		if(col == nullptr)
			return;

		for(mem_ptr ptr : *col) {
			item = (EntityItem *) ((uint64_t *) ptr + 1);
			item->components.free();
		}

		m_Collections.destroy(collection);
	}

	/*
	============================
	EntityManager::mustBeDeleted
	============================
	*/
	bool EntityManager::mustBeDeleted(entity_collection_id collection, entity_id entity)
	{
		List *list = (List *) m_EntitiesToDelete.getPtr(collection);
		if(list == nullptr) {
			List l(sizeof(entity_id));
			list = (List *) m_EntitiesToDelete.insertCopy(collection, &l);
		}
		if(list->addCopy(&entity))
			return false;

		return true;
	}

	/*
	=============================
	EntityManager::deleteEntities
	=============================
	*/
	void EntityManager::deleteEntities()
	{
		List *list;
		for(mem_ptr col : m_EntitiesToDelete) {
			list = (List *) ((uint64_t *) col + 1);

			for(mem_ptr entPtr : *list)
				destroyEntity(*((uint64_t *) col), *((entity_id *) entPtr));

			list->free();
		}
	}

	/*
	==============================
	EntityManager::attachComponent
	==============================
	*/
	bool EntityManager::attachComponent(const Entity &entity, component_id component)
	{
		HashTable *col = (HashTable *) m_Collections.getPtr(entity.m_CollectionID);
		EntityItem *item = (EntityItem *) col->getPtr(entity.m_EntityID);

		component_type type = ComponentManager::getType(component);

		// Vérifie que l'entité ne possède pas déjà un composant du même type.
		if((item->componentsType & type) == type)
			return false;

		item->components.addCopy(&component);
		item->componentsType |= type;

		return true;
	}

	/*
	=============================
	EntityManager::getComponentID
	=============================
	*/
	component_id EntityManager::getComponentID(entity_collection_id collection, entity_id entity, component_type componentType)
	{
		HashTable *col = (HashTable *) m_Collections.getPtr(collection);
		EntityItem *item = (EntityItem *) col->getPtr(entity);

		component_id *component;
		List *list = &item->components;
		component_type type;

		// Itère à travers tous les composants de l'entité pour trouver celui qui a le bon type.
		for(mem_ptr ptr : *list) {
			component = (component_id *) ptr;

			type = ComponentManager::getType(*component);
			if(type == componentType)
				return *component;
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
		HashTable *col = (HashTable *) m_Collections.getPtr(collection);
		EntityItem *item;

		if(col == nullptr)
			return;
			
		for(mem_ptr ptr : *col) {
			item = (EntityItem *) ((uint64_t *) ptr + 1);
			if((item->componentsType & componentTypesToInclude) == componentTypesToInclude && (item->componentsType & componentTypesToExclude) == 0)
				dest->addCopy((uint64_t *) ptr);
		}
	}

}