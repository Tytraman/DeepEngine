#include "DE/ecs/scene.hpp"
#include "DE/ecs/entity.hpp"
#include "DE/ecs/component.hpp"
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

        file_object_container *entContainer = container->add_container(ent.get_name().str());

        component_manager *componentManager = component_manager::get_singleton();
        entity_manager *entityManager = entity_manager::get_singleton();

        component_manager::component_type componentTypes = entityManager->get_component_types(ent.get_entity_id(), ent.get_collection_id());

        if((to_utype(componentTypes) & to_utype(component_manager::component_type::Drawable)) > 0)
        {
            /*component_id drawID = entityManager->get_component_id(ent.get_entity_id(), ent.get_collection_id(), component_manager::component_type::drawable);
            drawable_component *comp = componentManager->get_drawable_component(drawID);*/
            /*file_object_container *drawableContainer = */entContainer->add_container("drawable_component");

            // Pour le moment il n'y a rien à sauvegarder.
        }

        if((to_utype(componentTypes) & to_utype(component_manager::component_type::Transformation)) > 0)
        {
            component_id transID = entityManager->get_component_id(ent.get_entity_id(), ent.get_collection_id(), component_manager::component_type::Transformation);
            transformation_component *comp = componentManager->get_transformation_component(transID);

            file_object_container *transContainer = entContainer->add_container("transformation_component");

            string x = string_utils::double_to_str(comp->get_translation().x);
            string y = string_utils::double_to_str(comp->get_translation().y);
            string z = string_utils::double_to_str(comp->get_translation().z);

            transContainer->add_element("", "translation_x", x.str());
            transContainer->add_element("", "translation_y", y.str());
            transContainer->add_element("", "translation_z", z.str());

            x = string_utils::double_to_str(comp->get_scaling().x);
            y = string_utils::double_to_str(comp->get_scaling().y);
            z = string_utils::double_to_str(comp->get_scaling().z);

            transContainer->add_element("", "scaling_x", x.str());
            transContainer->add_element("", "scaling_y", y.str());
            transContainer->add_element("", "scaling_z", z.str());

            x = string_utils::double_to_str(comp->get_rotation_X());
            y = string_utils::double_to_str(comp->get_rotation_Y());
            z = string_utils::double_to_str(comp->get_rotation_Z());

            transContainer->add_element("", "rotation_x", x.str());
            transContainer->add_element("", "rotation_y", y.str());
            transContainer->add_element("", "rotation_z", z.str());
        }

        if((to_utype(componentTypes) & to_utype(component_manager::component_type::Collider)) > 0)
        {
            component_id colID = entityManager->get_component_id(ent.get_entity_id(), ent.get_collection_id(), component_manager::component_type::Collider);
            collider_component *comp = componentManager->get_collider_component(colID);

            file_object_container *colContainer = entContainer->add_container("collider_component");

            vec3<float> colPos = comp->position;
            vec3<float> colSize = comp->size;

            string x = string_utils::double_to_str(colPos.x);
            string y = string_utils::double_to_str(colPos.y);
            string z = string_utils::double_to_str(colPos.z);

            colContainer->add_element("", "collider_position_x", x.str());
            colContainer->add_element("", "collider_position_y", y.str());
            colContainer->add_element("", "collider_position_z", z.str());

            x = string_utils::double_to_str(colSize.x);
            y = string_utils::double_to_str(colSize.y);
            z = string_utils::double_to_str(colSize.z);

            colContainer->add_element("", "collider_size_x", x.str());
            colContainer->add_element("", "collider_size_y", y.str());
            colContainer->add_element("", "collider_size_z", z.str());
        }

        if((to_utype(componentTypes) & to_utype(component_manager::component_type::Velocity)) > 0)
        {
            component_id veloID = entityManager->get_component_id(ent.get_entity_id(), ent.get_collection_id(), component_manager::component_type::Velocity);
            velocity_component *comp = componentManager->get_velocity_component(veloID);

            file_object_container *veloContainer = entContainer->add_container("velocity_component");

            vec3<float> vel = comp->get_velocity();

            string x = string_utils::double_to_str(vel.x);
            string y = string_utils::double_to_str(vel.y);
            string z = string_utils::double_to_str(vel.z);

            veloContainer->add_element("", "velocity_x", x.str());
            veloContainer->add_element("", "velocity_y", y.str());
            veloContainer->add_element("", "velocity_z", z.str());
        }
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
