#ifndef __DEEP_ENGINE_SCENE_HPP__
#define __DEEP_ENGINE_SCENE_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/ecs/ecs.hpp>
#include <DE/ecs/entity.hpp>
#include <DE/memory/list.hpp>
#include <DE/vec.hpp>
#include <DE/graphics/shape.hpp>
#include <DE/rendering/camera.hpp>

namespace deep
{

    class scene
    {

        using enum_callback = void (*)(scene_id id, scene &scene);

        public:
            DE_API static void shutdown();

            /// @brief	Crée une scène.
            /// @return L'ID de la scène nouvellement créée.
            DE_API static scene_id createScene(const char *name);

            /// @brief			Supprime une scène.
            /// @param scene	L'ID de la scène à supprimer.
            DE_API static void deleteScene(scene_id scn);
            DE_API static void deleteAllScenes();

            DE_API static bool mustBeDeleted(scene_id scn);
            DE_API static void deleteScenes();

            DE_API static void setActiveScene(scene_id scn);

            /// @brief	Crée une entité dans la scène.
            /// @return L'entité nouvellement créé.
            DE_API static entity createEntity(scene_id scn);

            DE_API static void enumScenes(enum_callback callback);


            //===== GETTERS =====//

            /// @brief       Récupère l'ID de la collection d'entités liée à la scène spécifiée.
            /// @param scene L'ID de la scène.
            /// @return      L'ID de la collection d'entités si la scène existe sinon \ref badID.
            DE_API static entity_collection_id getEntityCollection(scene_id scene);

            /// @brief  Récupère l'ID de la scène actuellement active.
            /// @return L'ID de la scène active ou \ref badID si aucune scène n'est active.
            DE_API static scene_id getActiveSceneID();

            /// @brief       Récupère un pointeur vers la structure de la scène spécifié.
            /// @param scene L'ID de la scène.
            /// @return      Le pointeur vers la structure si la scène existe ou \c nullptr si elle n'existe pas.
            DE_API static scene *getScene(scene_id scene);

            DE_API collider_in_callback getColliderCallback() const;
            DE_API collider_out_callback getColliderOutCallback() const;

            DE_API Camera &getCamera();

            DE_API const char *getName() const;

            //===== SETTERS =====//

            DE_API void setColliderCallback(collider_in_callback callback);
            DE_API void setColliderOutCallback(collider_out_callback callback);
            
        private:
            scene(const char *name);
            
            static scene_id m_ActiveScene;
            static list<scene_id> m_ScenesToDelete;

            collider_in_callback m_ColliderCallback;        ///< Callback lorsqu'une collision est détectée entre 2 entités.
            collider_out_callback m_ColliderOutCallback;    ///< Callback lorsqu'une collision n'est plus détectée entre 2 entités.

            entity_collection_id m_EntityCollection;        ///< ID du gestionnaire d'entités de la scène.
            Camera m_Camera;
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
        deleteAllScenes();
    }

    /*
    ====================
    scene::mustBeDeleted
    ====================
    */
    inline bool scene::mustBeDeleted(scene_id scn)
    {
        return m_ScenesToDelete.add(scn);
    }

    /*
    ======================
    scene::setCurrentScene
    ======================
    */
    inline void scene::setActiveScene(scene_id scn)
    {
        m_ActiveScene = scn;
    }

    /*
    =====================
    scene::getActiveScene
    =====================
    */
    inline scene_id scene::getActiveSceneID()
    {
        return m_ActiveScene;
    }

    /*
    ==========================
    scene::getColliderCallback
    ==========================
    */
    inline collider_in_callback scene::getColliderCallback() const
    {
        return m_ColliderCallback;
    }

    /*
    =============================
    scene::getColliderOutCallback
    =============================
    */
    inline collider_out_callback scene::getColliderOutCallback() const
    {
        return m_ColliderOutCallback;
    }

    inline Camera &scene::getCamera()
    {
        return m_Camera;
    }

    /*
    ==============
    scene::getName
    ==============
    */
    inline const char *scene::getName() const
    {
        return m_Name;
    }

    /*
    ==========================
    scene::setColliderCallback
    ==========================
    */
    inline void scene::setColliderCallback(collider_in_callback callback)
    {
        m_ColliderCallback = callback;
    }

    /*
    =============================
    scene::setColliderOutCallback
    =============================
    */
    inline void scene::setColliderOutCallback(collider_out_callback callback)
    {
        m_ColliderOutCallback = callback;
    }

}

#endif