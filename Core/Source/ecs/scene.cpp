#include "DE/ecs/scene.hpp"
#include "DE/ecs/entity.hpp"
#include "DE/string_utils.hpp"
#include "DE/memory/hash_table.hpp"

namespace deep
{

    scene_id scene::m_ActiveScene = badID;
    list<scene_id> scene::m_ScenesToDelete;

    static hash_table<scene> m_Scenes;

    /*
    ===================
    scene::create_scene
    ===================
    */
    scene_id scene::create_scene(const char *name)
    {
        scene_id id = (scene_id) m_Scenes.size();

        m_Scenes.insert(id, scene(name));

        return id;
    }

    /*
    ===================
    scene::delete_scene
    ===================
    */
    void scene::delete_scene(scene_id scn)
    {
        scene *s = get_scene(scn);
        if(s != nullptr)
        {
            mem::free(s->m_Name);
        }

        m_Scenes.remove(scn);
    }

    /*
    ========================
    scene::delete_all_scenes
    ========================
    */
    void scene::delete_all_scenes()
    {
        for(auto &it : m_Scenes)
        {
            mem::free(it.value.m_Name);
        }

        m_Scenes.clear();
    }

    /*
    ====================
    scene::delete_scenes
    ====================
    */
    void scene::delete_scenes()
    {
        size_t length = m_ScenesToDelete.count();
        size_t i;
        scene_id scene;

        for(i = 0; i < length; ++i)
        {
            scene = m_ScenesToDelete[i];

            delete_scene(scene);
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
          m_Camera(vec3<float>(0.5f, 0.5f, 0.0f), vec3<float>(0.0f, 0.0f, -1.0f)),
          m_Name(string_utils::copy(name))
    {
        entity_manager *entityManager = entity_manager::get_singleton();
        m_EntityCollection = entityManager->create_entity_collection();
    }

    /*
    ==================
    scene::enum_scenes
    ==================
    */
    void scene::enum_scenes(enum_callback callback)
    {
        for(auto &el : m_Scenes)
        {
            callback(static_cast<scene_id>(el.key), el.value);
        }

        delete_scenes();
    }

    void __deep_export_scene_entity_callback(entity_manager::entity &ent, mem_ptr args)
    {
        file_object_container *container = static_cast<file_object_container *>(args);

        /*file_object_container *entContainer = */container->add_container(ent.get_name().str());
    }

    /*
    ===================
    scene::export_scene
    ===================
    */
    bool scene::export_scene(scene_id scn, file_object &dest)
    {
        hash_entry<deep::scene> *s = m_Scenes[scn];
        if(s == nullptr)
        {
            return false;
        }

        entity_manager *entityManager = entity_manager::get_singleton();

        file_object_container *container = dest.add_container("scenes");
        container = container->add_container(s->value.m_Name);

        entityManager->enum_entities(s->value.m_EntityCollection, __deep_export_scene_entity_callback, container);

        return true;
    }

    /*
    ============================
    scene::get_entity_collection
    ============================
    */
    entity_collection_id scene::get_entity_collection(scene_id scn)
    {
        hash_entry<deep::scene> *s = m_Scenes[scn];
        if(s == nullptr)
        {
            return badID;
        }

        return s->value.m_EntityCollection;
    }

    /*
    ================
    scene::get_scene
    ================
    */
    scene *scene::get_scene(scene_id scn)
    {
        hash_entry<deep::scene> *s = m_Scenes[scn];
        if(s == nullptr)
        {
            return nullptr;
        }

        return &s->value;
    }

}
