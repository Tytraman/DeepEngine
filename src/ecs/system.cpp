#include <DE/ecs/entity.hpp>
#include <DE/ecs/component.hpp>
#include <DE/ecs/system.hpp>

#include <DE/ecs/scene.hpp>
#include <DE/mat.hpp>

#include <unordered_map>

namespace de {

	system_id SystemManager::m_NextID = 0;
	List SystemManager::m_EnabledSystems(sizeof(system_id));

	static std::unordered_map<system_id, SystemItem> m_Functions;

	static std::unordered_map<entity_id, std::unordered_map<entity_id, Rect>> m_Collisions;

	/*
	=========================
	SystemManager::SystemItem
	=========================
	*/
	SystemItem::SystemItem(SystemFunction _function, component_type _componentTypesToInclude, component_type _componentTypesToExclude)
		: function(_function), componentTypesToInclude(_componentTypesToInclude), componentTypesToExclude(_componentTypesToExclude)
	{ }

	/*
	===========================
	SystemManager::createSystem
	===========================
	*/
	system_id SystemManager::createSystem(SystemFunction function, component_type componentTypesToInclude, component_type componentTypesToExclude)
	{
		system_id id = m_NextID;

		m_Functions.emplace(id, SystemItem(function, componentTypesToInclude, componentTypesToExclude));

		m_NextID = id + 1;

		return id;
	}

	/*
	============================
	SystemManager::executeSystem
	============================
	*/
	void SystemManager::executeSystem(system_id system)
	{
		const auto &it = m_Functions.find(system);
		if(it == m_Functions.end())
			return;

		scene_id scene = Scene::getActiveSceneID();
		if(scene == badID)
			return;

		entity_collection_id collection = Scene::getEntityCollection(scene);

		List entities(sizeof(entity_id));
		EntityManager::query(collection, it->second.componentTypesToInclude, it->second.componentTypesToExclude, &entities);

		it->second.function(&entities);
	}

	/*
	=============================
	SystemManager::executeSystems
	=============================
	*/
	void SystemManager::executeSystems()
	{
		size_t i;
		size_t length = m_EnabledSystems.getNumberOfElements();
		system_id system;

		scene_id scene = Scene::getActiveSceneID();
		if(scene == badID)
			return;

		entity_collection_id collection = Scene::getEntityCollection(scene);

		List entities(sizeof(entity_id));

		// Itère à travers les systèmes activés dans l'ordre dans lequel ils ont été activé,
		// 1er activé = 1er exécuté
		for(i = 0; i < length; ++i) {
			if(m_EnabledSystems.getCopy(i, &system)) {
				const auto &it = m_Functions.find(system);
				if(it != m_Functions.end()) {
					EntityManager::query(collection, it->second.componentTypesToInclude, it->second.componentTypesToExclude, &entities);
					it->second.function(&entities);
					entities.free();
				}
			}
		}
	}

	/*
	=================================
	SystemManager::accelerationSystem
	=================================
	*/
	void SystemManager::accelerationSystem()
	{
		scene_id sceneID = Scene::getActiveSceneID();

		// Si aucune scène n'est active, cela ne sert à rien de continuer la procédure.
		if(sceneID == badID)
			return;

		List entities(sizeof(entity_id));
		entity_collection_id collectionID = Scene::getEntityCollection(sceneID);

		// Query toutes les entités possédant une vélocité et une accélération.
		EntityManager::query(collectionID, VelocityComponentType | AccelerationComponentType, 0, &entities);

		size_t length = entities.getNumberOfElements();
		size_t i;
		entity_id entity;

		// Itère à travers toutes les entités possédant une vélocité et une accélération.
		for(i = 0; i < length; ++i) {
			if(entities.getCopy(i, &entity)) {
				component_id velCpnID = EntityManager::getComponentID(collectionID, entity, VelocityComponentType);
				VelocityComponent *velCpn = ComponentManager::getVelocityComponent(velCpnID);

				if(velCpn == nullptr)
					continue;

				component_id accCpnID = EntityManager::getComponentID(collectionID, entity, AccelerationComponentType);
				AccelerationComponent *accCpn = ComponentManager::getAccelerationComponent(accCpnID);

				if(accCpn == nullptr)
					continue;

				// Applique l'accélération à la vélocité.
				velCpn->setVelocity(velCpn->getVelocity() + accCpn->acceleration);
			}
		}
	}

	/*
	=============================
	SystemManager::velocitySystem
	=============================
	*/
	void SystemManager::velocitySystem()
	{
		scene_id sceneID = Scene::getActiveSceneID();

		// Si aucune scène n'est active, cela ne sert à rien de continuer la procédure.
		if(sceneID == badID)
			return;

		List entities(sizeof(entity_id));
		entity_collection_id collectionID = Scene::getEntityCollection(sceneID);

		// Query toutes les entités possédant une vélocité et une transformation.
		EntityManager::query(collectionID, TransformationComponentType | VelocityComponentType, 0, &entities);

		size_t length = entities.getNumberOfElements();
		size_t i;
		entity_id entity;

		// Itère à travers toutes les entités possédant une vélocité et une transformation.
		for(i = 0; i < length; ++i) {
			if(entities.getCopy(i, &entity)) {
				component_id transformationComponentID = EntityManager::getComponentID(collectionID, entity, TransformationComponentType);

				TransformationComponent *transformationComponent = ComponentManager::getTransformationComponent(transformationComponentID);

				if(transformationComponent == nullptr)
					continue;

				component_id velocityComponentID = EntityManager::getComponentID(collectionID, entity, VelocityComponentType);

				VelocityComponent *velocityComponent = ComponentManager::getVelocityComponent(velocityComponentID);

				if(velocityComponent == nullptr)
					continue;

				// Applique la vélocité sur la transformation.
				fvec3 translation = transformationComponent->getTranslation();
				fvec3 velocity = velocityComponent->getVelocity();
				translation += velocity;
				transformationComponent->setTranslation(translation);

				// Si l'entité possède une collision, on met à jour sa position aussi.
				component_id colliderComponentID = EntityManager::getComponentID(collectionID, entity, ColliderComponentType);
				if(colliderComponentID != badID) {
					ColliderComponent *colliderComponent = ComponentManager::getColliderComponent(colliderComponentID);
					colliderComponent->contour.pos += velocity;
				}
			}
		}
	}

	/*
	=============================
	SystemManager::colliderSystem
	=============================
	*/
	void SystemManager::colliderSystem()
	{
		scene_id sceneID = Scene::getActiveSceneID();

		// Si aucune scène n'est active, cela ne sert à rien de continuer la procédure.
		if(sceneID == badID)
			return;

		Scene *scene = Scene::getScene(sceneID);

		// scene n'est pas censée pouvoir valoir nullptr, mais ne sait-on jamais.
		if(scene == nullptr)
			return;

		ColliderCallback callback = scene->getColliderCallback();
		ColliderOutCallback outCallback = scene->getColliderOutCallback();
		
		// S'il n'y a pas de callback à appeler alors ça ne sert à rien de trouver les collisions.
		if(callback == nullptr || outCallback == nullptr)
			return;

		List entities(sizeof(entity_id));
		entity_collection_id collectionID = Scene::getEntityCollection(sceneID);

		// Query toutes les entités possédant une boîte de collision.
		EntityManager::query(collectionID, ColliderComponentType, 0, &entities);

		size_t length = entities.getNumberOfElements();
		size_t i, j;
		entity_id entity1, entity2;

		// S'il n'y a pas plus qu'une seule entité il est inutile de continuer la procédure.
		if(length < 2)
			return;

		// Itère à travers toutes les entités possédant une boîte de collision.
		for(i = 0; i < length - 1; ++i) {
			if(entities.getCopy(i, &entity1)) {
				component_id colliderComponentID1 = EntityManager::getComponentID(collectionID, entity1, ColliderComponentType);

				ColliderComponent *colliderComponent1 = ComponentManager::getColliderComponent(colliderComponentID1);

				const fvec2 &col1 = colliderComponent1->contour.pos;
				const float &w1   = colliderComponent1->contour.w;
				const float &h1   = colliderComponent1->contour.h;

				if(colliderComponent1 == nullptr)
					continue;

				fvec2 entity1Middle((col1.x + col1.x + w1) / 2, (col1.y + col1.y + h1) / 2);

				for(j = i + 1; j < length; ++j) {
					if(entities.getCopy(j, &entity2)) {
						component_id colliderComponentID2 = EntityManager::getComponentID(collectionID, entity2, ColliderComponentType);
						ColliderComponent *colliderComponent2 = ComponentManager::getColliderComponent(colliderComponentID2);

						if(colliderComponent2 == nullptr)
							continue;

						const fvec2 &col2 = colliderComponent2->contour.pos;
						const float &w2   = colliderComponent2->contour.w;
						const float &h2   = colliderComponent2->contour.h;

						fvec2 entity2Middle((col2.x + col2.x + w2) / 2, (col2.y + col2.y + h2) / 2);

						if(colliderComponent2->contour.isInside(colliderComponent1->contour)) {
							Rect collision(fvec2(), 0.0f, 0.0f);

							// Détermine le rectangle de collision.
							if(col1.y > col2.y) {
								collision.pos.y = col1.y;
								collision.h = col2.y + h2 - col1.y;
							}else {
								collision.pos.y = col2.y;
								collision.h = col1.y + h1 - col2.y;
							}

							if(col1.x > col2.x) {
								collision.pos.x = col1.x;
								collision.w = col2.x + w2 - col1.x;
							}else {
								collision.pos.x = col2.x;
								collision.w = col1.x + w1 - col2.x;
							}

							m_Collisions[entity1][entity2] = collision;

							// Appelle la fonction callback entre les 2 entités qui sont en collision entre elles.

							callback(collectionID, entity1, entity2, entity2Middle - entity1Middle, collision);

							int a = 10;
						}else {
							const auto &it = m_Collisions.find(entity1);
							if(it == m_Collisions.end())
								continue;

							if(it->second.erase(entity2) > 0)
								outCallback(collectionID, entity1, entity2, entity2Middle - entity1Middle);
						}
					}
				}
			}
		}
	}

	/*
	===========================
	SystemManager::renderSystem
	===========================
	*/
//	void SystemManager::renderSystem(Renderer &renderer, scene_id sceneID)
//	{
//		// Nettoie l'écran en le remplissant de la couleur noire.
//		renderer.setColor(colora(0, 0, 0, 255));
//		renderer.clear();
//
//		// Si aucune scène n'est active, cela ne sert à rien de continuer la procédure.
//		if(sceneID != badID) {
//			Scene *scene = Scene::getScene(sceneID);
//
//			// scene n'est pas censée pouvoir valoir nullptr, mais ne sait-on jamais.
//			if(scene == nullptr)
//				goto skip;
//
//			List entities(sizeof(entity_id));
//			entity_collection_id collection = Scene::getEntityCollection(sceneID);
//			fvec2 sceneViewTranslation = scene->getViewTranslation();
//			fvec2 sceneViewScale = scene->getViewScale();
//			float sceneViewAngle = scene->getViewAngle();
//
//			// Cette matrice permet de passer de l'espace du monde à l'espace de la caméra.
//			fmat3x3 view = fmat3x3::translate(fmat3x3(), sceneViewTranslation);
//					view = fmat3x3::rotate(view, sceneViewAngle);
//					view = fmat3x3::scale(view, sceneViewScale);
//
//			// Récupère toutes les entités dessinables et ayant une transformation.
//			EntityManager::query(collection, DrawableComponentType | TransformationComponentType, 0, &entities);
//
//			size_t index, length = entities.getNumberOfElements();
//			entity_id entity;
//			component_id drawableComponentID;
//			component_id positionComponentID;
//		
//			// Itère à travers toutes les entités précédemment récupérées.
//			for(index = 0; index < length; ++index) {
//				if(entities.getCopy(index, &entity)) {
//					DrawableComponent *drawableComponent;
//					TransformationComponent *transformationComponent;
//
//					drawableComponentID = EntityManager::getComponentID(collection, entity, DrawableComponentType);
//					positionComponentID = EntityManager::getComponentID(collection, entity, TransformationComponentType);
//
//					drawableComponent = ComponentManager::getDrawableComponent(drawableComponentID);
//					if(drawableComponent == nullptr)
//						continue;
//
//					transformationComponent = ComponentManager::getTransformationComponent(positionComponentID);
//					if(transformationComponent == nullptr)
//						continue;
//
//					size_t numberOfVertices = drawableComponent->vertices.getNumberOfElements();
//					size_t i;
//					SDL_Vertex *vertices = (SDL_Vertex *) mem::allocNoTrack(sizeof(SDL_Vertex) * numberOfVertices);
//					if(vertices == nullptr)
//						continue;
//
//					Vertex vertex;
//
//					// TODO: optimiser le rendu, utiliser OpenGL.
//					for(i = 0; i < numberOfVertices; ++i) {
//						if(drawableComponent->vertices.getCopy(i, &vertex)) {
//							fvec3 vertexPos(vertex.pos.x, vertex.pos.y);
//
//							// Cette matrice permet de passer de l'espace local à l'espce du monde.
//							fmat3x3 model = fmat3x3::translate(fmat3x3(), transformationComponent->m_Translation);
//									model = fmat3x3::rotate(model, transformationComponent->m_Rotation);
//									model = fmat3x3::scale(model, transformationComponent->m_Scaling);
//
//							// Applique la transformation sur le vecteur final
//							vertexPos = view * model * vertexPos;
//
//							vertices[i].position.x = vertexPos.x;
//							vertices[i].position.y = vertexPos.y;
//							vertices[i].color.a = vertex.color.A;
//							vertices[i].color.r = vertex.color.R;
//							vertices[i].color.g = vertex.color.G;
//							vertices[i].color.b = vertex.color.B;
//						}
//					}
//
//					renderer.drawShape(vertices, (int) numberOfVertices);
//
//					mem::freeNoTrack(vertices);
//
//					transformationComponent->m_LastMovement.x = 0.0f;
//					transformationComponent->m_LastMovement.y = 0.0f;
//				}
//			}
//		}
//
//skip:
//		renderer.swapBuffers();
//	}

	void SystemManager::renderSystem(OpenGLRenderer &renderer, scene_id sceneID)
	{
		// Nettoie l'écran en le remplissant de la couleur noire.
		renderer.setClearColor( { 0, 0, 0, 255 } );
		renderer.clear();

		// Si aucune scène n'est active, cela ne sert à rien de continuer la procédure.
		if(sceneID != badID) {
			Scene *scene = Scene::getScene(sceneID);

			const Camera &cam = scene->getCamera();

			List entities(sizeof(entity_id));
			entity_collection_id collection = Scene::getEntityCollection(sceneID);

			// Récupère toutes les entités dessinables et ayant une transformation.
			EntityManager::query(collection, DrawableComponentType | TransformationComponentType, 0, &entities);

			size_t length = entities.getNumberOfElements();
			entity_id entity;
			component_id drawableComponentID;
			component_id transformationComponentID;
			size_t i;

			GLUniform uniModel;

			unsigned int programID = GLProgram::currentlyBound();

			for(i = 0; i < length; ++i) {
				if(entities.getCopy(i, &entity)) {
					drawableComponentID       = EntityManager::getComponentID(collection, entity, DrawableComponentType);
					transformationComponentID = EntityManager::getComponentID(collection, entity, TransformationComponentType);

					DrawableComponent *drawableComponent = ComponentManager::getDrawableComponent(drawableComponentID);
					TransformationComponent *transformationComponent = ComponentManager::getTransformationComponent(transformationComponentID);

					if(uniModel.find(programID, "model")) {
						fmat4x4 model = fmat4x4::translate(fmat4x4(), transformationComponent->m_Translation);
								model = fmat4x4::rotate(model, transformationComponent->m_Rotation);
								model = fmat4x4::scale(model, transformationComponent->m_Scaling);

						uniModel.send(model);

						if(uniModel.find(programID, "view"))
							uniModel.send(cam.lookAt());
					}

					drawableComponent->vao.bind();

					renderer.draw(drawableComponent->vbo.verticesNumber());
				}
			}
		}

		renderer.swapBuffers();
	}

}
