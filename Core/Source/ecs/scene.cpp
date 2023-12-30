#include <DE/ecs/scene.hpp>
#include <DE/ecs/entity.hpp>
#include <DE/string_utils.hpp>
#include <DE/memory/hash_table.hpp>

namespace deep
{

    scene_id scene::m_ActiveScene = badID;
    list<scene_id> scene::m_ScenesToDelete;

    static hash_table<scene> m_Scenes;

    /*
    ==================
    scene::createScene
    ==================
    */
    scene_id scene::createScene(const char *name)
    {
        scene_id id = (scene_id) m_Scenes.size();

        m_Scenes.insert(id, scene(name));

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

        m_Scenes.remove(scn);
    }

    /*
    ======================
    scene::deleteAllScenes
    ======================
    */
    void scene::deleteAllScenes()
    {
        for(auto &it : m_Scenes)
            mem::free(it.value.m_Name);

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
        : m_ColliderCallback(nullptr),
          m_ColliderOutCallback(nullptr),
          m_Camera(fvec3(0.5f, 0.5f, 0.0f), fvec3(0.0f, 0.0f, -1.0f)),
          m_Name(string_utils::copy(name))
    {
        entity_manager *entityManager = entity_manager::get_singleton();
        m_EntityCollection = entityManager->createEntityCollection();
    }

    /*
    ================
    scene::addEntity
    ================
    */
    entity scene::createEntity(scene_id scene)
    {
        hash_entry<deep::scene> *s = m_Scenes[scene];

        if(s == nullptr)
            return entity::bad();

        entity_manager *entityManager = entity_manager::get_singleton();

        return entityManager->createEntity(s->value.m_EntityCollection);
    }

    /*
    =================
    scene::enumScenes
    =================
    */
    void scene::enumScenes(enum_callback callback)
    {
        for(auto &el : m_Scenes)
            callback(static_cast<scene_id>(el.key), el.value);

        deleteScenes();
    }

    /*
    ==========================
    scene::getEntityCollection
    ==========================
    */
    entity_collection_id scene::getEntityCollection(scene_id scn)
    {
        hash_entry<deep::scene> *s = m_Scenes[scn];
        if(s == nullptr)
            return badID;

        return s->value.m_EntityCollection;
    }

    /*
    ===============
    scene::getScene
    ===============
    */
    scene *scene::getScene(scene_id scn)
    {
        hash_entry<deep::scene> *s = m_Scenes[scn];
        if(s == nullptr)
            return nullptr;

        return &s->value;
    }

}
