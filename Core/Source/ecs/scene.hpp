#ifndef __DEEP_ENGINE_SCENE_HPP__
#define __DEEP_ENGINE_SCENE_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "ecs/ecs.hpp"
#include "ecs/entity.hpp"
#include "core/templates/list.hpp"
#include "maths/vec.hpp"
#include "graphics/shape.hpp"
#include "core/camera.hpp"
#include "file/file_object.hpp"

namespace deep
{

    class scene
    {

        using enum_callback = void (*)(scene_id id, scene &scene);

        public:
            DE_API static void shutdown();

            /// @brief	Crée une scène.
            /// @return L'ID de la scène nouvellement créée.
            DE_API static scene_id create_scene(const char *name);

            /// @brief			Supprime une scène.
            /// @param scene	L'ID de la scène à supprimer.
            DE_API static void delete_scene(scene_id scn);
            DE_API static void delete_all_scenes();

            DE_API static bool must_be_deleted(scene_id scn);
            DE_API static void delete_scenes();

            DE_API static void set_active_scene(scene_id scn);

            /// @brief	Crée une entité dans la scène.
            /// @return L'entité nouvellement créé.
            //DE_API static entity create_entity(scene_id scn);

            DE_API static void enum_scenes(enum_callback callback);

            DE_API static bool export_scene(scene_id scn, file_object &dest);


            //===== GETTERS =====//

            /// @brief       Récupère l'ID de la collection d'entités liée à la scène spécifiée.
            /// @param scene L'ID de la scène.
            /// @return      L'ID de la collection d'entités si la scène existe sinon \ref badID.
            DE_API static entity_collection_id get_entity_collection(scene_id scene);

            /// @brief  Récupère l'ID de la scène actuellement active.
            /// @return L'ID de la scène active ou \ref badID si aucune scène n'est active.
            DE_API static scene_id get_active_scene_id();

            /// @brief       Récupère un pointeur vers la structure de la scène spécifié.
            /// @param scene L'ID de la scène.
            /// @return      Le pointeur vers la structure si la scène existe ou \c nullptr si elle n'existe pas.
            DE_API static scene *get_scene(scene_id scene);

            DE_API collider_in_callback get_collider_callback() const;
            DE_API collider_out_callback get_collider_out_callback() const;

            DE_API camera &get_attached_camera();

            DE_API const char *get_name() const;

            //===== SETTERS =====//

            DE_API void set_collider_callback(collider_in_callback callback);
            DE_API void set_collider_out_callback(collider_out_callback callback);
            
        private:
            scene(const char *name);
            
            static scene_id m_ActiveScene;
            static list<scene_id> m_ScenesToDelete;

            collider_in_callback m_ColliderCallback;        ///< Callback lorsqu'une collision est détectée entre 2 entités.
            collider_out_callback m_ColliderOutCallback;    ///< Callback lorsqu'une collision n'est plus détectée entre 2 entités.

            entity_collection_id m_EntityCollection;        ///< ID du gestionnaire d'entités de la scène.
            camera m_Camera;
            char *m_Name;                                   ///< Nom de la scène.

    };

    /*
    ===============
    scene::shutdown
    ===============
    */
    inline void scene::shutdown()
    {
        m_ScenesToDelete.free();
        delete_all_scenes();
    }

    /*
    ====================
    scene::must_be_deleted
    ====================
    */
    inline bool scene::must_be_deleted(scene_id scn)
    {
        return m_ScenesToDelete.add(scn);
    }

    /*
    ======================
    scene::setCurrentScene
    ======================
    */
    inline void scene::set_active_scene(scene_id scn)
    {
        m_ActiveScene = scn;
    }

    /*
    =====================
    scene::getActiveScene
    =====================
    */
    inline scene_id scene::get_active_scene_id()
    {
        return m_ActiveScene;
    }

    /*
    ==========================
    scene::get_collider_callback
    ==========================
    */
    inline collider_in_callback scene::get_collider_callback() const
    {
        return m_ColliderCallback;
    }

    /*
    =============================
    scene::get_collider_out_callback
    =============================
    */
    inline collider_out_callback scene::get_collider_out_callback() const
    {
        return m_ColliderOutCallback;
    }

    inline camera &scene::get_attached_camera()
    {
        return m_Camera;
    }

    /*
    ==============
    scene::get_name
    ==============
    */
    inline const char *scene::get_name() const
    {
        return m_Name;
    }

    /*
    ==========================
    scene::set_collider_callback
    ==========================
    */
    inline void scene::set_collider_callback(collider_in_callback callback)
    {
        m_ColliderCallback = callback;
    }

    /*
    =============================
    scene::set_collider_out_callback
    =============================
    */
    inline void scene::set_collider_out_callback(collider_out_callback callback)
    {
        m_ColliderOutCallback = callback;
    }

}

#endif