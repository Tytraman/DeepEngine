#include <DE/entity.hpp>

#include <cassert>

/*
============================
EntityManager::EntityManager
============================
*/
de::EntityManager::EntityManager(entity_id maxEntities)
	: _maxEntities(maxEntities), _entityCount(0)
{
	entity_id id;
	for(id = 0; id < maxEntities; ++id)
		_availableEntities.push(id);
}

/*
===========================
EntityManager::createEntity
===========================
*/
de::entity_id de::EntityManager::createEntity()
{
	assert(_entityCount < _maxEntities && "There are too many entities.");

	entity_id id = _availableEntities.front();
	_availableEntities.pop();

	_entityCount++;

	return id;
}

/*
============================
EntityManager::destroyEntity
============================
*/
bool de::EntityManager::destroyEntity(entity_id id)
{
	if(id >= _maxEntities)
		return false;


}
