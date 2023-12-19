#include <DE/ecs/scene.hpp>
#include <DE/ecs/entity.hpp>
#include <DE/string_utils.hpp>

#include <unordered_map>

namespace deep {

	scene_id scene::m_ActiveScene = badID;
	list<scene_id> scene::m_ScenesToDelete;

	static std::unordered_map<scene_id, scene> m_Scenes;

	/*
	==================
	scene::createScene
	==================
	*/
	scene_id scene::createScene(const char *name)
	{
		scene_id id = (scene_id) m_Scenes.size();

		m_Scenes.emplace(id, scene(name));

		return id;
	}

	/*
	==================
	scene::deleteScene
	==================
	*/
	void scene::deleteScene(scene_id scn)
	{
		scene *s = getScene(scn);
		if(s != nullptr)
			mem::free(s->m_Name);

		m_Scenes.erase(scn);
	}

	/*
	======================
	scene::deleteAllScenes
	======================
	*/
	void scene::deleteAllScenes()
	{
		for(auto &it : m_Scenes)
			mem::free(it.second.m_Name);

		m_Scenes.clear();
	}

	/*
	===================
	scene::deleteScenes
	===================
	*/
	void scene::deleteScenes()
	{
		size_t length = m_ScenesToDelete.count();
		size_t i;
		scene_id scene;

		for(i = 0; i < length; ++i)
        {
            scene = m_ScenesToDelete[i];

			deleteScene(scene);
		}

		m_ScenesToDelete.free();
	}

	/*
	============
	scene::scene
	============
	*/
	scene::scene(const char *name)
		: m_EntityCollection(entity_manager::createEntityCollection()),
		  m_ColliderCallback(nullptr),
		  m_ColliderOutCallback(nullptr),
		  m_Camera(fvec3(0.5f, 0.5f, 0.0f), fvec3(0.0f, 0.0f, -1.0f)),
		  m_Name(string_utils::copy(name))
	{ }

	/*
	================
	scene::addEntity
	================
	*/
	entity scene::createEntity(scene_id scene)
	{
		const auto &s = m_Scenes.find(scene);
		if(s == m_Scenes.end())
			return entity::bad();

		return entity_manager::createEntity(s->second.m_EntityCollection);
	}

	/*
	=================
	scene::enumScenes
	=================
	*/
	void scene::enumScenes(enum_callback callback)
	{
		for(auto &el : m_Scenes)
			callback(el.first, el.second);

		deleteScenes();
	}

	/*
	==========================
	scene::getEntityCollection
	==========================
	*/
	entity_collection_id scene::getEntityCollection(scene_id scn)
	{
		auto it = m_Scenes.find(scn);
		if(it == m_Scenes.end())
			return badID;
		return it->second.m_EntityCollection;
	}

	/*
	===============
	scene::getScene
	===============
	*/
	scene *scene::getScene(scene_id scn)
	{
		auto it = m_Scenes.find(scn);
		if(it == m_Scenes.end())
			return nullptr;
		return &it->second;
	}

}
