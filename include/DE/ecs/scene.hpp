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

namespace de {

	class DE_API Scene {

		using EnumCallback = void (*)(scene_id id, Scene &scene);

		public:
			static void shutdown();

			/// @brief	Crée une scène.
			/// @return L'ID de la scène nouvellement créée.
			static scene_id createScene(const char *name);

			/// @brief			Supprime une scène.
			/// @param scene	L'ID de la scène à supprimer.
			static void deleteScene(scene_id scene);
			static void deleteAllScenes();

			static bool mustBeDeleted(scene_id scene);
			static void deleteScenes();

			static void setActiveScene(scene_id scene);

			/// @brief	Crée une entité dans la scène.
			/// @return L'entité nouvellement créé.
			static Entity createEntity(scene_id scene);

			static void enumScenes(EnumCallback callback);



			//===== GETTERS =====//

			/// @brief       Récupère l'ID de la collection d'entités liée à la scène spécifiée.
			/// @param scene L'ID de la scène.
			/// @return      L'ID de la collection d'entités si la scène existe sinon \ref badID.
			static entity_collection_id getEntityCollection(scene_id scene);

			/// @brief  Récupère l'ID de la scène actuellement active.
			/// @return L'ID de la scène active ou \ref badID si aucune scène n'est active.
			static scene_id getActiveSceneID();

			/// @brief       Récupère un pointeur vers la structure de la scène spécifié.
			/// @param scene L'ID de la scène.
			/// @return      Le pointeur vers la structure si la scène existe ou \c nullptr si elle n'existe pas.
			static Scene *getScene(scene_id scene);

			ColliderCallback getColliderCallback() const;
			ColliderOutCallback getColliderOutCallback() const;

			Camera &getCamera();

			const char *getName() const;

			//===== SETTERS =====//

			void setColliderCallback(ColliderCallback callback);
			void setColliderOutCallback(ColliderOutCallback callback);

			
		private:
			Scene(const char *name);
			
			static scene_id m_ActiveScene;
			static List     m_ScenesToDelete;

			ColliderCallback m_ColliderCallback;          ///< Callback lorsqu'une collision est détectée entre 2 entités.
			ColliderOutCallback m_ColliderOutCallback;    ///< Callback lorsqu'une collision n'est plus détectée entre 2 entités.

			entity_collection_id m_EntityCollection;      ///< ID du gestionnaire d'entités de la scène.
			Camera m_Camera;
			char *m_Name;							      ///< Nom de la scène.

	};

	/*
	===============
	Scene::shutdown
	===============
	*/
	inline void Scene::shutdown()
	{
		m_ScenesToDelete.free();
		deleteAllScenes();
	}

	/*
	====================
	Scene::mustBeDeleted
	====================
	*/
	inline bool Scene::mustBeDeleted(scene_id scene)
	{
		return m_ScenesToDelete.addCopy(&scene);
	}

	/*
	======================
	Scene::setCurrentScene
	======================
	*/
	inline void Scene::setActiveScene(scene_id scene)
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

	/*
	==========================
	Scene::getColliderCallback
	==========================
	*/
	inline ColliderCallback Scene::getColliderCallback() const
	{
		return m_ColliderCallback;
	}

	/*
	=============================
	Scene::getColliderOutCallback
	=============================
	*/
	inline ColliderOutCallback Scene::getColliderOutCallback() const
	{
		return m_ColliderOutCallback;
	}

	inline Camera &Scene::getCamera()
	{
		return m_Camera;
	}

	/*
	==============
	Scene::getName
	==============
	*/
	inline const char *Scene::getName() const
	{
		return m_Name;
	}

	/*
	==========================
	Scene::setColliderCallback
	==========================
	*/
	inline void Scene::setColliderCallback(ColliderCallback callback)
	{
		m_ColliderCallback = callback;
	}

	/*
	=============================
	Scene::setColliderOutCallback
	=============================
	*/
	inline void Scene::setColliderOutCallback(ColliderOutCallback callback)
	{
		m_ColliderOutCallback = callback;
	}

}

#endif