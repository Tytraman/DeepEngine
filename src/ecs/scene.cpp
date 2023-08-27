#include <DE/ecs/scene.hpp>
#include <DE/ecs/entity.hpp>
#include <DE/string_utils.hpp>

#include <unordered_map>

namespace de {

	scene_id Scene::m_ActiveScene = badID;
	List Scene::m_ScenesToDelete(sizeof(scene_id));

	static std::unordered_map<scene_id, Scene> m_Scenes;

	/*
	==================
	Scene::createScene
	==================
	*/
	scene_id Scene::createScene(const char *name)
	{
		scene_id id = (scene_id) m_Scenes.size();

		m_Scenes.emplace(id, Scene(name));

		return id;
	}

	/*
	==================
	Scene::deleteScene
	==================
	*/
	void Scene::deleteScene(scene_id scene)
	{
		Scene * s = getScene(scene);
		if(s != nullptr)
			mem::free(s->m_Name);

		m_Scenes.erase(scene);
	}

	/*
	======================
	Scene::deleteAllScenes
	======================
	*/
	void Scene::deleteAllScenes()
	{
		for(auto &it : m_Scenes)
			mem::free(it.second.m_Name);

		m_Scenes.clear();
	}

	/*
	===================
	Scene::deleteScenes
	===================
	*/
	void Scene::deleteScenes()
	{
		size_t length = m_ScenesToDelete.getNumberOfElements();
		size_t i;
		scene_id scene;

		for(i = 0; i < length; ++i) {
			if(!m_ScenesToDelete.getCopy(i, &scene))
			   continue;

			deleteScene(scene);
		}

		m_ScenesToDelete.free();
	}

	/*
	============
	Scene::Scene
	============
	*/
	Scene::Scene(const char *name)
		: m_EntityCollection(EntityManager::createEntityCollection()),
		  m_ColliderCallback(nullptr),
		  m_ColliderOutCallback(nullptr),
		  m_Camera(fvec3(0.5f, 0.5f, 0.0f), fvec3(0.0f, 0.0f, -1.0f)),
		  m_Name(StringUtils::copy(name))
	{ }

	/*
	================
	Scene::addEntity
	================
	*/
	Entity Scene::createEntity(scene_id scene)
	{
		const auto &s = m_Scenes.find(scene);
		if(s == m_Scenes.end())
			return Entity::bad();

		return EntityManager::createEntity(s->second.m_EntityCollection);
	}

	/*
	=================
	Scene::enumScenes
	=================
	*/
	void Scene::enumScenes(EnumCallback callback)
	{
		for(auto &el : m_Scenes)
			callback(el.first, el.second);

		deleteScenes();
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
