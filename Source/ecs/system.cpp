#include <DE/ecs/entity.hpp>
#include <DE/ecs/component.hpp>
#include <DE/ecs/system.hpp>
#include <DE/window.hpp>

#include <DE/ecs/scene.hpp>
#include <DE/mat.hpp>

#include <unordered_map>

namespace de
{

	system_id system_manager::m_NextID = 0;
    list<system_id> system_manager::m_EnabledSystems;

	static std::unordered_map<system_id, system_item> m_Functions;

	static std::unordered_map<entity_id, std::unordered_map<entity_id, rect>> m_Collisions;

	/*
	===========================
	system_manager::system_item
	===========================
	*/
	system_item::system_item(system_function _function, component_type _componentTypesToInclude, component_type _componentTypesToExclude)
		: function(_function), componentTypesToInclude(_componentTypesToInclude), componentTypesToExclude(_componentTypesToExclude)
	{ }

	/*
	============================
	system_manager::createSystem
	============================
	*/
	system_id system_manager::createSystem(system_function function, component_type componentTypesToInclude, component_type componentTypesToExclude)
	{
		system_id id = m_NextID;

		m_Functions.emplace(id, system_item(function, componentTypesToInclude, componentTypesToExclude));

		m_NextID = id + 1;

		return id;
	}

	/*
	=============================
	system_manager::executeSystem
	=============================
	*/
	void system_manager::executeSystem(system_id system)
	{
		const auto &it = m_Functions.find(system);
		if(it == m_Functions.end())
			return;

		scene_id scene = scene::getActiveSceneID();
		if(scene == badID)
			return;

		entity_collection_id collection = scene::getEntityCollection(scene);

		list<entity_id> entities;
		entity_manager::query(collection, it->second.componentTypesToInclude, it->second.componentTypesToExclude, entities);

		it->second.function(entities);

        entities.free();
	}

	/*
	=============================
	system_manager::executeSystems
	=============================
	*/
	void system_manager::executeSystems()
	{
		size_t i;
		size_t length = m_EnabledSystems.count();
		system_id system;

		scene_id scene = scene::getActiveSceneID();
		if(scene == badID)
			return;

		entity_collection_id collection = scene::getEntityCollection(scene);

		list<entity_id> entities;

		// Itère à travers les systèmes activés dans l'ordre dans lequel ils ont été activé,
		// 1er activé = 1er exécuté
		for(i = 0; i < length; ++i)
        {
            system = m_EnabledSystems[i];

			const auto &it = m_Functions.find(system);
			if(it != m_Functions.end())
            {
				entity_manager::query(collection, it->second.componentTypesToInclude, it->second.componentTypesToExclude, entities);
				it->second.function(entities);
				entities.free();
			}
		}
	}

	/*
	=================================
	system_manager::accelerationSystem
	=================================
	*/
	void system_manager::accelerationSystem()
	{
		scene_id sceneID = scene::getActiveSceneID();

		// Si aucune scène n'est active, cela ne sert à rien de continuer la procédure.
		if(sceneID == badID)
			return;

		list<entity_id> entities;
		entity_collection_id collectionID = scene::getEntityCollection(sceneID);

		// Query toutes les entités possédant une vélocité et une accélération.
		entity_manager::query(collectionID, VelocityComponentType | AccelerationComponentType, 0, entities);

		size_t length = entities.count();
		size_t i;
		entity_id entity;

		// Itère à travers toutes les entités possédant une vélocité et une accélération.
		for(i = 0; i < length; ++i)
        {
            entity = entities[i];

			component_id velCpnID = entity_manager::getComponentID(collectionID, entity, VelocityComponentType);
			velocity_component *velCpn = component_manager::getVelocityComponent(velCpnID);

			if(velCpn == nullptr)
				continue;

			component_id accCpnID = entity_manager::getComponentID(collectionID, entity, AccelerationComponentType);
			acceleration_component *accCpn = component_manager::getAccelerationComponent(accCpnID);

			if(accCpn == nullptr)
				continue;

			// Applique l'accélération à la vélocité.
			velCpn->setVelocity(velCpn->getVelocity() + accCpn->acceleration);
		}
	}

	/*
	=============================
	system_manager::velocitySystem
	=============================
	*/
	void system_manager::velocitySystem()
	{
		scene_id sceneID = scene::getActiveSceneID();

		// Si aucune scène n'est active, cela ne sert à rien de continuer la procédure.
		if(sceneID == badID)
			return;

		list<entity_id> entities;
		entity_collection_id collectionID = scene::getEntityCollection(sceneID);

		// Query toutes les entités possédant une vélocité et une transformation.
		entity_manager::query(collectionID, TransformationComponentType | VelocityComponentType, 0, entities);

		size_t length = entities.count();
		size_t i;
		entity_id entity;

		// Itère à travers toutes les entités possédant une vélocité et une transformation.
		for(i = 0; i < length; ++i)
        {
            entity = entities[i];

			component_id transformationComponentID = entity_manager::getComponentID(collectionID, entity, TransformationComponentType);

			transformation_component *transformationComponent = component_manager::getTransformationComponent(transformationComponentID);

			if(transformationComponent == nullptr)
				continue;

			component_id velocityComponentID = entity_manager::getComponentID(collectionID, entity, VelocityComponentType);

			velocity_component *velocityComponent = component_manager::getVelocityComponent(velocityComponentID);

			if(velocityComponent == nullptr)
				continue;

			// Applique la vélocité sur la transformation.
			fvec3 translation = transformationComponent->getTranslation();
			fvec3 velocity = velocityComponent->getVelocity();
			translation += velocity;
			transformationComponent->setTranslation(translation);

			// Si l'entité possède une collision, on met à jour sa position aussi.
			component_id colliderComponentID = entity_manager::getComponentID(collectionID, entity, ColliderComponentType);
			if(colliderComponentID != badID)
            {
				collider_component *colliderComponent = component_manager::getColliderComponent(colliderComponentID);
				colliderComponent->contour.pos += velocity;
			}
		}
	}

	/*
	=============================
	system_manager::colliderSystem
	=============================
	*/
	void system_manager::colliderSystem()
	{
		scene_id sceneID = scene::getActiveSceneID();

		// Si aucune scène n'est active, cela ne sert à rien de continuer la procédure.
		if(sceneID == badID)
			return;

		scene *scene = scene::getScene(sceneID);

		// scene n'est pas censée pouvoir valoir nullptr, mais ne sait-on jamais.
		if(scene == nullptr)
			return;

		collider_in_callback callback = scene->getColliderCallback();
		collider_out_callback outCallback = scene->getColliderOutCallback();
		
		// S'il n'y a pas de callback à appeler alors ça ne sert à rien de trouver les collisions.
		if(callback == nullptr || outCallback == nullptr)
			return;

		list<entity_id> entities;
		entity_collection_id collectionID = scene::getEntityCollection(sceneID);

		// Query toutes les entités possédant une boîte de collision.
		entity_manager::query(collectionID, ColliderComponentType, 0, entities);

		size_t length = entities.count();
		size_t i, j;
		entity_id entity1, entity2;

		// S'il n'y a pas plus qu'une seule entité il est inutile de continuer la procédure.
		if(length < 2)
			return;

		// Itère à travers toutes les entités possédant une boîte de collision.
		for(i = 0; i < length - 1; ++i)
        {
            entity1 = entities[i];

			component_id colliderComponentID1 = entity_manager::getComponentID(collectionID, entity1, ColliderComponentType);

			collider_component *colliderComponent1 = component_manager::getColliderComponent(colliderComponentID1);

			const fvec2 &col1 = colliderComponent1->contour.pos;
			const float &w1   = colliderComponent1->contour.w;
			const float &h1   = colliderComponent1->contour.h;

			if(colliderComponent1 == nullptr)
				continue;

			fvec2 entity1Middle((col1.x + col1.x + w1) / 2, (col1.y + col1.y + h1) / 2);

			for(j = i + 1; j < length; ++j)
            {
                entity2 = entities[j];

				component_id colliderComponentID2 = entity_manager::getComponentID(collectionID, entity2, ColliderComponentType);
				collider_component *colliderComponent2 = component_manager::getColliderComponent(colliderComponentID2);

				if(colliderComponent2 == nullptr)
					continue;

				const fvec2 &col2 = colliderComponent2->contour.pos;
				const float &w2   = colliderComponent2->contour.w;
				const float &h2   = colliderComponent2->contour.h;

				fvec2 entity2Middle((col2.x + col2.x + w2) / 2, (col2.y + col2.y + h2) / 2);

				if(colliderComponent2->contour.isInside(colliderComponent1->contour))
                {
					rect collision(fvec2(), 0.0f, 0.0f);

					// Détermine le rectangle de collision.
					if(col1.y > col2.y) {
						collision.pos.y = col1.y;
						collision.h = col2.y + h2 - col1.y;
					}
                    else
                    {
						collision.pos.y = col2.y;
						collision.h = col1.y + h1 - col2.y;
					}

					if(col1.x > col2.x)
                    {
						collision.pos.x = col1.x;
						collision.w = col2.x + w2 - col1.x;
					}
                    else
                    {
						collision.pos.x = col2.x;
						collision.w = col1.x + w1 - col2.x;
					}

					m_Collisions[entity1][entity2] = collision;

					// Appelle la fonction callback entre les 2 entités qui sont en collision entre elles.

					callback(collectionID, entity1, entity2, entity2Middle - entity1Middle, collision);
				}
                else
                {
					const auto &it = m_Collisions.find(entity1);
					if(it == m_Collisions.end())
						continue;

					if(it->second.erase(entity2) > 0)
						outCallback(collectionID, entity1, entity2, entity2Middle - entity1Middle);
				}
			}
		}
	}

	/*
	============================
	system_manager::renderSystem
	============================
	*/
	void system_manager::renderSystem(gl_renderer &renderer, framerenderbuffer &frb, scene_id sceneID)
	{
		window *window = renderer.getWindow();

        // Le rendu est d'abord stocké dans un framebuffer pour pouvoir faire du post-processing.
        framebuffer_manager::bind(frb.framebuffer());
        texture_id texture = framebuffer_manager::currentTextureID();

        gpu_core::enableDepthTest();

		// Nettoie le framebuffer_manager en le remplissant de la couleur noire.
		renderer.setClearColor( { 0, 0, 0, 255 } );
		renderer.clear();

		// Si aucune scène n'est active, cela ne sert à rien de continuer la procédure.
		if(sceneID != badID)
        {
			scene *scene = scene::getScene(sceneID);

			Camera &cam = scene->getCamera();

			static list<entity_id> entities;
			entity_collection_id collection = scene::getEntityCollection(sceneID);

			// Récupère toutes les entités dessinables et ayant une transformation.
			entity_manager::query(collection, DrawableComponentType | TransformationComponentType, 0, entities);

			component_id drawableComponentID;
			component_id transformationComponentID;

			// Parcourt toutes les entités précédemment récupérées.
			for(auto ent : entities)
            {
				// Récupère les composants de l'entité.
				drawableComponentID       = entity_manager::getComponentID(collection, ent, DrawableComponentType);
				transformationComponentID = entity_manager::getComponentID(collection, ent, TransformationComponentType);

				drawable_component *drawableComponent = component_manager::getDrawableComponent(drawableComponentID);
				transformation_component *transformationComponent = component_manager::getTransformationComponent(transformationComponentID);

				// La façon dont un drawable est rendu peut être différente selon leur type, donc on utilise une fonction de callback.
				if(drawableComponent->renderCallback != nullptr)
					drawableComponent->renderCallback(renderer, drawableComponent, transformationComponent, window, &cam);
			}

			// Remet le compteur à 0 sans libérer la mémoire afin d'économiser des performances au détriment de la mémoire.
			// Solution temporaire avant d'en trouver une meilleure.
			entities.empty();
		}

        // Toute cette partie là permet de faire du post-processing.

        framebuffer_manager::bindDefault();
        gpu_core::disableDepthTest();

        renderer.setClearColor( { 0, 0, 0, 0 } );
		renderer.clear();

        if(program_manager::use("post_processing"))
        {
            if(vao_manager::bind(frb.vao()))
            {
                texture_manager::bind(texture, 0);

                renderer.draw(vbo_manager::getVerticesNumber(frb.vbo()));
            }
        }

		// C'est ce qui permet de mettre à jour les pixels de l'écran.
		renderer.swapBuffers();
	}

}
