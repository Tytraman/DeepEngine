#ifndef __DEEP_ENGINE_SYSTEM_HPP__
#define __DEEP_ENGINE_SYSTEM_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/core/list.hpp"
#include "DE/ecs/ecs.hpp"
#include "DE/ecs/component.hpp"
#include "DE/graphics/renderer.hpp"
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

                DE_API system_item(system_function function, component_manager::component_type componentTypesToInclude = component_manager::component_type::None, component_manager::component_type componentTypesToExclude = component_manager::component_type::None);
            };

        public:
            DE_API static system_manager *get_singleton();

            /// @brief			Crée un système.
            /// @param function	La fonction que le système devra exécuter.
            /// @return			L'ID du système nouvellement créé.
            DE_API system_id create_system(system_function function, component_manager::component_type componentTypesToInclude = component_manager::component_type::None, component_manager::component_type componentTypesToExclude = component_manager::component_type::None);

            /// @brief			Exécute un système spécifique.
            /// @param system	L'ID du système à exécuter.
            DE_API void execute_system(system_id system);

            /// @brief	Exécute tous les systèmes activés dans leur ordre d'activation.
            ///			1er activé = 1er exécuté.
            DE_API void execute_systems();

            /// @brief			Active le système afin qu'il soit exécuté lors de l'appel à \ref execute_systems.
            /// @remark			Si le système est déjà activé, il ne sera pas activé une 2ème fois.
            /// @param system	L'ID du système à activer.
            DE_API void enable_system(system_id system);

            /// @brief			Désactive le système afin qu'il ne soit plus exécuté lors de l'appel à \ref execute_systems.
            /// @remark			Si le système est déjà désactivé, il ne sera pas désactivé une 2ème fois.
            /// @param system	L'ID du système à désactiver.
            DE_API void disable_system(system_id system);

            DE_API void execute_acceleration_system();

            DE_API void execute_velocity_system();

            // TODO: refaire cette fonction
            DE_API void execute_collider_system();

            DE_API void execute_render_system(GL3::gl_renderer &renderer, GL3::framerenderbuffer &frb, scene_id sceneID);

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
    system_manager::enable_system
    ===========================
    */
    inline void system_manager::enable_system(system_id system)
    {
        // On n'active pas le système s'il est déjà activé
        if(m_EnabledSystems.find(system) != nothing)
            return;

        m_EnabledSystems.add(system);
    }

    /*
    ============================
    system_manager::disable_system
    ============================
    */
    inline void system_manager::disable_system(system_id system)
    {
        size_t index = m_EnabledSystems.find(system);

        if(index != nothing)
            m_EnabledSystems.remove(index);
    }

}


#endif