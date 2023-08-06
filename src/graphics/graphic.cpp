#include <DE/graphic/graphic.hpp>
#include <DE/graphic/vertex.hpp>

namespace de {

	/*
	======================
	Scene::attachComponent
	======================
	*/
	entity_id Graphic::createRectangle(scene_id scene, const fvec2 &position, float width, float height, const colora &color)
	{
		entity_collection_id collectionID = Scene::getEntityCollection(scene);

		component_id drawableComponentID = ComponentManager::createDrawableComponent();
		component_id transformationComponentID = ComponentManager::createTransformationComponent(position, fvec2(width, height), 0.0f);

		DrawableComponent *drawableComponent = ComponentManager::getDrawableComponent(drawableComponentID);

		Vertex v1;
		v1.pos.x = -0.5f;
		v1.pos.y = -0.5f;
		v1.color = color;

		Vertex v2;
		v2.pos.x = 0.5f;
		v2.pos.y = -0.5f;
		v2.color = color;

		Vertex v3;
		v3.pos.x = 0.5f;
		v3.pos.y = 0.5f;
		v3.color = color;

		Vertex v4;
		v4.pos.x = -0.5f;
		v4.pos.y = 0.5f;
		v4.color = color;

		drawableComponent->vertices.addCopy(&v1);
		drawableComponent->vertices.addCopy(&v2);
		drawableComponent->vertices.addCopy(&v3);
		drawableComponent->vertices.addCopy(&v1);
		drawableComponent->vertices.addCopy(&v3);
		drawableComponent->vertices.addCopy(&v4);

		entity_id entityID = EntityManager::createEntity(collectionID);
		EntityManager::attachComponent(collectionID, entityID, drawableComponentID);
		EntityManager::attachComponent(collectionID, entityID, transformationComponentID);

		return entityID;
	}

	/*
	=====================
	Scene::createTriangle
	=====================
	*/
	entity_id Graphic::createTriangle(scene_id scene, const fvec2 &position, float radius, const colora &color)
	{
		entity_collection_id collectionID = Scene::getEntityCollection(scene);

		component_id drawableComponentID = ComponentManager::createDrawableComponent();
		component_id transformationComponentID = ComponentManager::createTransformationComponent(position, fvec2(radius, radius), 0.0f);

		DrawableComponent *drawableComponent = ComponentManager::getDrawableComponent(drawableComponentID);

		Vertex v1;
		v1.pos.x = 0.0f;
		v1.pos.y = -0.5f;
		v1.color = color;

		Vertex v2;
		v2.pos.x = 0.5f;
		v2.pos.y = 0.5f;
		v2.color = color;

		Vertex v3;
		v3.pos.x = -0.5f;
		v3.pos.y = 0.5f;
		v3.color = color;

		drawableComponent->vertices.addCopy(&v1);
		drawableComponent->vertices.addCopy(&v2);
		drawableComponent->vertices.addCopy(&v3);

		entity_id entityID = EntityManager::createEntity(collectionID);
		EntityManager::attachComponent(collectionID, entityID, drawableComponentID);
		EntityManager::attachComponent(collectionID, entityID, transformationComponentID);

		return entityID;
	}

}
