#include <DE/scene.hpp>

#include <unordered_map>

namespace de {

	scene_id Scene::m_ActiveScene = badID;

	static std::unordered_map<scene_id, Scene> m_Scenes;

	/*
	==================
	Scene::createScene
	==================
	*/
	scene_id Scene::createScene()
	{
		scene_id id = (scene_id) m_Scenes.size();

		m_Scenes.emplace(id, Scene());

		return id;
	}

	/*
	==================
	Scene::deleteScene
	==================
	*/
	void Scene::deleteScene(scene_id scene)
	{
		m_Scenes.erase(scene);
	}

	/*
	============
	Scene::Scene
	============
	*/
	Scene::Scene()
		: m_EntityCollection(EntityManager::createEntityCollection()),
		  m_ViewTranslation(0.0f, 0.0f),
		  m_ViewScale(1.0f, 1.0f),
		  m_ViewAngle(0.0f)
	{ }

	/*
	================
	Scene::addEntity
	================
	*/
	entity_id Scene::createEntity(scene_id scene)
	{
		const auto &s = m_Scenes.find(scene);
		if(s == m_Scenes.end())
			return badID;

		return EntityManager::createEntity(s->second.m_EntityCollection);
	}

	/*
	====================
	Scene::destroyEntity
	====================
	*/
	void Scene::destroyEntity(scene_id scene, entity_id entity)
	{
		const auto &s = m_Scenes.find(scene);
		if(s == m_Scenes.end())
			return;

		EntityManager::destroyEntity(s->second.m_EntityCollection, entity);
	}

	/*
	======================
	Scene::attachComponent
	======================
	*/
	void Scene::attachComponent(scene_id scene, entity_id entity, component_id component)
	{
		const auto &s = m_Scenes.find(scene);
		if(s == m_Scenes.end())
			return;

		EntityManager::attachComponent(s->second.m_EntityCollection, entity, component);
	}

	/*
	==========================
	Scene::getEntityCollection
	==========================
	*/
	entity_collection_id Scene::getEntityCollection(scene_id scene)
	{
		auto it = m_Scenes.find(scene);
		if(it == m_Scenes.end())
			return badID;
		return it->second.m_EntityCollection;
	}

	/*
	===============
	Scene::getScene
	===============
	*/
	Scene *Scene::getScene(scene_id scene)
	{
		auto it = m_Scenes.find(scene);
		if(it == m_Scenes.end())
			return nullptr;
		return &it->second;
	}

}
