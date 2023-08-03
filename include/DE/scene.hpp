#ifndef __DEEP_ENGINE_SCENE_HPP__
#define __DEEP_ENGINE_SCENE_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/memory/list.hpp>
#include <DE/ecs/entity.hpp>

namespace de {

	using scene_id = de_id;

	class DE_API Scene {

		private:
			static scene_id m_ActiveScene;

			entity_collection_id m_EntityCollection;	///< ID du gestionnaire d'entités de la scène.

		public:
			/// @brief	Crée une scène.
			/// @return L'ID de la scène nouvellement créée.
			static scene_id createScene();

			/// @brief			Supprime une scène.
			/// @param scene	L'ID de la scène à supprimer.
			static void deleteScene(scene_id scene);

			static void setCurrentScene(scene_id scene);

			/// @brief	Crée une entité dans la scène.
			/// @return L'ID de l'entité nouvellement créé.
			static entity_id createEntity(scene_id scene);

			/// @brief			Détruit une entité appartenant à la scène.
			/// @param entity	L'ID de l'entité à retirer.
			static void destroyEntity(scene_id scene, entity_id entity);

			/// @brief				Attache un composant à l'entité appartenant à la scène.
			/// @param entity		L'ID de l'entité à laquelle ajouter un composant.
			/// @param component	L'ID du composant à ajouter.
			static void attachComponent(scene_id scene, entity_id entity, component_id component);

			//===== GETTERS =====//

			static entity_collection_id getEntityCollection(scene_id scene);
			static scene_id getActiveSceneID();

			
		private:
			Scene();

	};

	/*
	======================
	Scene::setCurrentScene
	======================
	*/
	inline void Scene::setCurrentScene(scene_id scene)
	{
		m_ActiveScene = scene;
	}

	/*
	=====================
	Scene::getActiveScene
	=====================
	*/
	inline scene_id Scene::getActiveSceneID()
	{
		return m_ActiveScene;
	}

}

#endif