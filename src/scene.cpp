#include <DE/scene.hpp>

/*
================
Scene::addEntity
================
*/
de::entity_id de::Scene::addEntity(component_info componentInfo)
{
	_entities.push_back( { (entity_id) _entities.size(), componentInfo });
	return _entities.back().id;
}
