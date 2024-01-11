#ifndef __DEEP_ENGINE_SYSTEM_HPP__
#define __DEEP_ENGINE_SYSTEM_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/memory/list.hpp"
#include "DE/ecs/ecs.hpp"
#include "DE/ecs/component.hpp"
#include "DE/renderer.hpp"
#include "DE/drivers/opengl/framerenderbuffer.hpp"
#include "DE/drivers/opengl/renderer.hpp"

namespace deep
{

    class system_manager
    {

        public:
            struct system_item
            {
                system_function function;
                component_manager::component_type componentTypesToInclude;
                component_manager::component_type componentTypesToExclude;

                DE_API system_item(system_function function, component_manager::component_type componentTypesToInclude = component_manager::component_type::none, component_manager::component_type componentTypesToExclude = component_manager::component_type::none);
            };

        public:
            DE_API static system_manager *get_singleton();

            /// @brief			Crée un système.
            /// @param function	La fonction que le système devra exécuter.
            /// @return			L'ID du système nouvellement créé.
            DE_API system_id createSystem(system_function function, component_manager::component_type componentTypesToInclude = component_manager::component_type::none, component_manager::component_type componentTypesToExclude = component_manager::component_type::none);

            /// @brief			Exécute un système spécifique.
            /// @param system	L'ID du système à exécuter.
            DE_API void executeSystem(system_id system);

            /// @brief	Exécute tous les systèmes activés dans leur ordre d'activation.
            ///			1er activé = 1er exécuté.
            DE_API void executeSystems();

            /// @brief			Active le système afin qu'il soit exécuté lors de l'appel à \ref executeSystems.
            /// @remark			Si le système est déjà activé, il ne sera pas activé une 2ème fois.
            /// @param system	L'ID du système à activer.
            DE_API void enableSystem(system_id system);

            /// @brief			Désactive le système afin qu'il ne soit plus exécuté lors de l'appel à \ref executeSystems.
            /// @remark			Si le système est déjà désactivé, il ne sera pas désactivé une 2ème fois.
            /// @param system	L'ID du système à désactiver.
            DE_API void disableSystem(system_id system);

            DE_API void accelerationSystem();

            DE_API void velocitySystem();

            DE_API void colliderSystem();

            DE_API void renderSystem(GL3::gl_renderer &renderer, GL3::framerenderbuffer &frb, scene_id sceneID);

        private:
            system_manager();

            system_id m_NextID;                  ///< L'ID du prochain système à attribuer.
            list<system_id> m_EnabledSystems;    ///< Liste des systèmes actuellement activés.

        public:
            system_manager(const system_manager &) = delete;
            system_manager(const system_manager &&) = delete;
    };

    /*
    ===========================
    system_manager::enableSystem
    ===========================
    */
    inline void system_manager::enableSystem(system_id system)
    {
        // On n'active pas le système s'il est déjà activé
        if(m_EnabledSystems.find(system) != nothing)
            return;

        m_EnabledSystems.add(system);
    }

    /*
    ============================
    system_manager::disableSystem
    ============================
    */
    inline void system_manager::disableSystem(system_id system)
    {
        size_t index = m_EnabledSystems.find(system);

        if(index != nothing)
            m_EnabledSystems.remove(index);
    }

}


#endif