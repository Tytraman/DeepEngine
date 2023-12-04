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

		// It�re � travers les syst�mes activ�s dans l'ordre dans lequel ils ont �t� activ�,
		// 1er activ� = 1er ex�cut�
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

		// Si aucune sc�ne n'est active, cela ne sert � rien de continuer la proc�dure.
		if(sceneID == badID)
			return;

		list<entity_id> entities;
		entity_collection_id collectionID = scene::getEntityCollection(sceneID);

		// Query toutes les entit�s poss�dant une v�locit� et une acc�l�ration.
		entity_manager::query(collectionID, VelocityComponentType | AccelerationComponentType, 0, entities);

		size_t length = entities.count();
		size_t i;
		entity_id entity;

		// It�re � travers toutes les entit�s poss�dant une v�locit� et une acc�l�ration.
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

			// Applique l'acc�l�ration � la v�locit�.
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

		// Si aucune sc�ne n'est active, cela ne sert � rien de continuer la proc�dure.
		if(sceneID == badID)
			return;

		list<entity_id> entities;
		entity_collection_id collectionID = scene::getEntityCollection(sceneID);

		// Query toutes les entit�s poss�dant une v�locit� et une transformation.
		entity_manager::query(collectionID, TransformationComponentType | VelocityComponentType, 0, entities);

		size_t length = entities.count();
		size_t i;
		entity_id entity;

		// It�re � travers toutes les entit�s poss�dant une v�locit� et une transformation.
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

			// Applique la v�locit� sur la transformation.
			fvec3 translation = transformationComponent->getTranslation();
			fvec3 velocity = velocityComponent->getVelocity();
			translation += velocity;
			transformationComponent->setTranslation(translation);

			// Si l'entit� poss�de une collision, on met � jour sa position aussi.
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

		// Si aucune sc�ne n'est active, cela ne sert � rien de continuer la proc�dure.
		if(sceneID == badID)
			return;

		scene *scene = scene::getScene(sceneID);

		// scene n'est pas cens�e pouvoir valoir nullptr, mais ne sait-on jamais.
		if(scene == nullptr)
			return;

		collider_in_callback callback = scene->getColliderCallback();
		collider_out_callback outCallback = scene->getColliderOutCallback();
		
		// S'il n'y a pas de callback � appeler alors �a ne sert � rien de trouver les collisions.
		if(callback == nullptr || outCallback == nullptr)
			return;

		list<entity_id> entities;
		entity_collection_id collectionID = scene::getEntityCollection(sceneID);

		// Query toutes les entit�s poss�dant une bo�te de collision.
		entity_manager::query(collectionID, ColliderComponentType, 0, entities);

		size_t length = entities.count();
		size_t i, j;
		entity_id entity1, entity2;

		// S'il n'y a pas plus qu'une seule entit� il est inutile de continuer la proc�dure.
		if(length < 2)
			return;

		// It�re � travers toutes les entit�s poss�dant une bo�te de collision.
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

					// D�termine le rectangle de collision.
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

					// Appelle la fonction callback entre les 2 entit�s qui sont en collision entre elles.

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

        // Le rendu est d'abord stock� dans un framebuffer pour pouvoir faire du post-processing.
        framebuffer_manager::bind(frb.framebuffer());
        texture_id texture = framebuffer_manager::currentTextureID();

        gpu_core::enableDepthTest();

		// Nettoie le framebuffer_manager en le remplissant de la couleur noire.
		renderer.setClearColor( { 0, 0, 0, 255 } );
		renderer.clear();

		// Si aucune sc�ne n'est active, cela ne sert � rien de continuer la proc�dure.
		if(sceneID != badID)
        {
			scene *scene = scene::getScene(sceneID);

			Camera &cam = scene->getCamera();

			static list<entity_id> entities;
			entity_collection_id collection = scene::getEntityCollection(sceneID);

			// R�cup�re toutes les entit�s dessinables et ayant une transformation.
			entity_manager::query(collection, DrawableComponentType | TransformationComponentType, 0, entities);

			component_id drawableComponentID;
			component_id transformationComponentID;

			// Parcourt toutes les entit�s pr�c�demment r�cup�r�es.
			for(auto ent : entities)
            {
				// R�cup�re les composants de l'entit�.
				drawableComponentID       = entity_manager::getComponentID(collection, ent, DrawableComponentType);
				transformationComponentID = entity_manager::getComponentID(collection, ent, TransformationComponentType);

				drawable_component *drawableComponent = component_manager::getDrawableComponent(drawableComponentID);
				transformation_component *transformationComponent = component_manager::getTransformationComponent(transformationComponentID);

				// La fa�on dont un drawable est rendu peut �tre diff�rente selon leur type, donc on utilise une fonction de callback.
				if(drawableComponent->renderCallback != nullptr)
					drawableComponent->renderCallback(renderer, drawableComponent, transformationComponent, window, &cam);
			}

			// Remet le compteur � 0 sans lib�rer la m�moire afin d'�conomiser des performances au d�triment de la m�moire.
			// Solution temporaire avant d'en trouver une meilleure.
			entities.empty();
		}

        // Toute cette partie l� permet de faire du post-processing.

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

		// C'est ce qui permet de mettre � jour les pixels de l'�cran.
		renderer.swapBuffers();
	}

}
