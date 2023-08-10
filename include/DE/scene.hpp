#ifndef __DEEP_ENGINE_SCENE_HPP__
#define __DEEP_ENGINE_SCENE_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/ecs/ecs.hpp>
#include <DE/ecs/entity.hpp>
#include <DE/memory/list.hpp>
#include <DE/vec.hpp>
#include <DE/graphic/shape.hpp>

namespace de {

	class DE_API Scene {

		private:
			static scene_id m_ActiveScene;

			ColliderCallback m_ColliderCallback;     ///< Callback lorsqu'une collision est détectée entre 2 entités.
			entity_collection_id m_EntityCollection; ///< ID du gestionnaire d'entités de la scène.
			fvec2 m_ViewTranslation;                 ///< La position de la vue relative à l'origine du plan.
			fvec2 m_ViewScale;                       ///< Le zoom / déformation de la vue.
			float m_ViewAngle;                       ///< L'angle de rotation de la vue relative à l'origine du plan.

		public:
			/// @brief	Crée une scène.
			/// @return L'ID de la scène nouvellement créée.
			static scene_id createScene();

			/// @brief			Supprime une scène.
			/// @param scene	L'ID de la scène à supprimer.
			static void deleteScene(scene_id scene);

			static void setCurrentScene(scene_id scene);

			/// @brief	Crée une entité dans la scène.
			/// @return L'entité nouvellement créé.
			static Entity createEntity(scene_id scene);

			/// @brief				Attache un composant à l'entité appartenant à la scène.
			/// @param entity		L'ID de l'entité à laquelle ajouter un composant.
			/// @param component	L'ID du composant à ajouter.
			//== static void attachComponent(scene_id scene, entity_id entity, component_id component);

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

			/// @brief  Récupère la position de la vue de la scène.
			/// @return Le vecteur décrivant la translation de la vue.
			fvec2 getViewTranslation() const;

			/// @brief  Récupère le zoom / déformation de la vue de la scène.
			/// @return Le vecteur décrivant le zoom / déformation de la vue.
			fvec2 getViewScale() const;

			/// @brief  Récupère l'angle de rotation de la vue de la scène.
			/// @return L'angle de rotation de la vue de la scène.
			float getViewAngle() const;

			//===== SETTERS =====//

			void setColliderCallback(ColliderCallback callback);
			void setViewTranslation(const fvec2 &vec);
			void setViewScale(const fvec2 &vec);
			void setViewAngle(float angle);

			
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
	=========================
	Scene::getViewTranslation
	=========================
	*/
	inline fvec2 Scene::getViewTranslation() const
	{
		return m_ViewTranslation;
	}

	/*
	===================
	Scene::getViewScale
	===================
	*/
	inline fvec2 Scene::getViewScale() const
	{
		return m_ViewScale;
	}

	/*
	===================
	Scene::getViewAngle
	===================
	*/
	inline float Scene::getViewAngle() const
	{
		return m_ViewAngle;
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
	=========================
	Scene::setViewTranslation
	=========================
	*/
	inline void Scene::setViewTranslation(const fvec2 &vec)
	{
		m_ViewTranslation = vec;
	}

	/*
	===================
	Scene::setViewScale
	===================
	*/
	inline void Scene::setViewScale(const fvec2 &vec)
	{
		m_ViewScale = vec;
	}

	/*
	===================
	Scene::setViewAngle
	===================
	*/
	inline void Scene::setViewAngle(float angle)
	{
		m_ViewAngle = angle;
	}

}

#endif