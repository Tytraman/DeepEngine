#include <DE/ecs/system.hpp>
#include <DE/ecs/entity.hpp>
#include <DE/scene.hpp>
#include <DE/mat.hpp>

#include <unordered_map>

namespace de {

	system_id SystemManager::m_NextID = 0;
	List SystemManager::m_EnabledSystems(sizeof(system_id));

	std::unordered_map<system_id, SystemItem> m_Functions;

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
	===========================
	SystemManager::renderSystem
	===========================
	*/
	void SystemManager::renderSystem(Renderer &renderer)
	{
		scene_id sceneID = Scene::getActiveSceneID();

		// Si aucune scène n'est active, cela ne sert à rien de continuer la procédure.
		if(sceneID == badID)
			return;

		Scene *scene = Scene::getScene(sceneID);

		// scene n'est pas censée pouvoir valoir nullptr, mais ne sait-on jamais.
		if(scene == nullptr)
			return;

		List entities(sizeof(entity_id));
		entity_collection_id collection = Scene::getEntityCollection(sceneID);
		fvec2 sceneViewTranslation = scene->getViewTranslation();
		fvec2 sceneViewScale = scene->getViewScale();
		float sceneViewAngle = scene->getViewAngle();

		// Cette matrice permet de passer de l'espace du monde à l'espace de la caméra.
		fmat3x3 view = fmat3x3::translate(fmat3x3(), sceneViewTranslation);
		        view = fmat3x3::rotate(view, sceneViewAngle);
				view = fmat3x3::scale(view, sceneViewScale);

		// Récupère toutes les entités dessinables et ayant une transformation.
		EntityManager::query(collection, DrawableComponentType | TransformationComponentType, 0, &entities);

		size_t index, length = entities.getNumberOfElements();
		entity_id entity;
		component_id drawableComponentID;
		component_id positionComponentID;
		
		// Nettoie l'écran en le remplissant de la couleur noire.
		renderer.setColor(colora(0, 0, 0, 255));
		renderer.clear();
		
		// Itère à travers toutes les entités précédemment récupérées.
		for(index = 0; index < length; ++index) {
			if(entities.getCopy(index, &entity)) {
				DrawableComponent *drawableComponent;
				TransformationComponent *transformationComponent;

				drawableComponentID = EntityManager::getComponentID(collection, entity, DrawableComponentType);
				positionComponentID = EntityManager::getComponentID(collection, entity, TransformationComponentType);

				drawableComponent = ComponentManager::getDrawableComponent(drawableComponentID);
				if(drawableComponent == nullptr)
					continue;

				transformationComponent = ComponentManager::getTransformationComponent(positionComponentID);
				if(transformationComponent == nullptr)
					continue;

				size_t numberOfVertices = drawableComponent->vertices.getNumberOfElements();
				size_t i;
				SDL_Vertex *vertices = (SDL_Vertex *) mem::allocNoTrack(sizeof(SDL_Vertex) * numberOfVertices);
				if(vertices == nullptr)
					continue;

				Vertex vertex;

				// TODO: optimiser le rendu, utiliser OpenGL.
				for(i = 0; i < numberOfVertices; ++i) {
					if(drawableComponent->vertices.getCopy(i, &vertex)) {
						fvec3 vertexPos(vertex.pos.x, vertex.pos.y);

						// Cette matrice permet de passer de l'espace local à l'espce du monde.
						fmat3x3 model = fmat3x3::translate(fmat3x3(), transformationComponent->translation);
						        model = fmat3x3::rotate(model, transformationComponent->rotation);
						        model = fmat3x3::scale(model, transformationComponent->scaling);

						// Applique la transformation sur le vecteur final
						vertexPos = view * model * vertexPos;

						vertices[i].position.x = vertexPos.x;
						vertices[i].position.y = vertexPos.y;
						vertices[i].color.a = vertex.color.A;
						vertices[i].color.r = vertex.color.R;
						vertices[i].color.g = vertex.color.G;
						vertices[i].color.b = vertex.color.B;
					}
				}

				renderer.drawShape(vertices, (int) numberOfVertices);

				mem::freeNoTrack(vertices);
			}
		}

		renderer.swapBuffers();
	}

}
