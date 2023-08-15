#ifndef __DEEP_ENGINE_SYSTEM_HPP__
#define __DEEP_ENGINE_SYSTEM_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/memory/list.hpp>
#include <DE/ecs/ecs.hpp>
#include <DE/renderer.hpp>

namespace de {

	struct DE_API SystemItem {
		SystemFunction function;					///< Fonction que le système doit exécuter.
		component_type componentTypesToInclude;		///< Masque indiquant les types de composants à inclure dans le query.
		component_type componentTypesToExclude;		///< Masque indiquant les types de composants à ne pas inclure dans le query.

		SystemItem(SystemFunction function, component_type componentTypesToInclude = 0, component_type componentTypesToExclude = 0);
	};

	class DE_API SystemManager {

		private:
			static system_id m_NextID;		///< L'ID du prochain système à attribuer.
			static List m_EnabledSystems;	///< Liste des systèmes actuellement activés.

		public:
			SystemManager() = delete;
			SystemManager(const SystemManager &) = delete;
			SystemManager(const SystemManager &&) = delete;

			/// @brief			Crée un système.
			/// @param function	La fonction que le système devra exécuter.
			/// @return			L'ID du système nouvellement créé.
			static system_id createSystem(SystemFunction function, component_type componentTypesToInclude = 0, component_type componentTypesToExclude = 0);

			/// @brief			Exécute un système spécifique.
			/// @param system	L'ID du système à exécuter.
			static void executeSystem(system_id system);

			/// @brief	Exécute tous les systèmes activés dans leur ordre d'activation.
			///			1er activé = 1er exécuté.
			static void executeSystems();

			/// @brief			Active le système afin qu'il soit exécuté lors de l'appel à \ref executeSystems.
			/// @remark			Si le système est déjà activé, il ne sera pas activé une 2ème fois.
			/// @param system	L'ID du système à activer.
			static void enableSystem(system_id system);

			/// @brief			Désactive le système afin qu'il ne soit plus exécuté lors de l'appel à \ref executeSystems.
			/// @remark			Si le système est déjà désactivé, il ne sera pas désactivé une 2ème fois.
			/// @param system	L'ID du système à désactiver.
			static void disableSystem(system_id system);

			static void accelerationSystem();

			static void velocitySystem();

			static void colliderSystem();

			static void renderSystem(Renderer &renderer, scene_id sceneID);
	};

	/*
	===========================
	SystemManager::enableSystem
	===========================
	*/
	inline void SystemManager::enableSystem(system_id system)
	{
		// On n'active pas le système s'il est déjà activé
		if(m_EnabledSystems.find(&system) != nothing)
			return;

		m_EnabledSystems.addCopy(&system);
	}

	/*
	============================
	SystemManager::disableSystem
	============================
	*/
	inline void SystemManager::disableSystem(system_id system)
	{
		size_t index = m_EnabledSystems.find(&system);

		if(index != nothing)
			m_EnabledSystems.remove(index);
	}

}


#endif