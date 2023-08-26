#include <DE/graphic/graphic.hpp>
#include <DE/graphic/vertex.hpp>

#include <DE/ecs/entity.hpp>
#include <DE/ecs/component.hpp>

namespace de {

	/*
	==========
	Rect::Rect
	==========
	*/
	Rect::Rect(const fvec2 &position, float width, float height)
		: pos(position),
		  w(width),
		  h(height)
	{ }

	/*
	======================
	Scene::attachComponent
	======================
	*/
	Entity Graphic::createRectangle(entity_collection_id collectionID, const fvec2 &position, float width, float height, const colora &color, bool collidable)
	{
		component_id drawableComponentID       = ComponentManager::createDrawableComponent();
		component_id transformationComponentID = ComponentManager::createTransformationComponent(position, fvec2(width, height), 0.0f);

		DrawableComponent *drawableComponent = ComponentManager::getDrawableComponent(drawableComponentID);

		float vPos[] = {
			-0.5f,	-0.5f, color.R, color.G, color.B, color.A,
			 0.5f,	-0.5f, color.R, color.G, color.B, color.A,
			 0.5f,	 0.5f, color.R, color.G, color.B, color.A,
			-0.5f,	-0.5f, color.R, color.G, color.B, color.A,
			 0.5f,	 0.5f, color.R, color.G, color.B, color.A,
			-0.5f,   0.5f, color.R, color.G, color.B, color.A
		};

		MemoryChunk chunk(vPos, sizeof(vPos));

		drawableComponent->vbo.bind();
		drawableComponent->vao.bind();
		drawableComponent->vbo.transmitData(chunk);
		drawableComponent->vbo.setVerticesNumber(6);
		drawableComponent->vbo.addAttribute(0, GLAttribComponentsNumber::x2, GLType::Float, 6 * sizeof(float), 0);
		drawableComponent->vbo.addAttribute(1, GLAttribComponentsNumber::x4, GLType::Float, 6 * sizeof(float), 2 * sizeof(float));

		Entity entity = EntityManager::createEntity(collectionID);
		EntityManager::attachComponent(entity, drawableComponentID);
		EntityManager::attachComponent(entity, transformationComponentID);

		if(collidable) {
			component_id colliderComponentID = ComponentManager::createColliderComponent();

			ColliderComponent *colliderComponent = ComponentManager::getColliderComponent(colliderComponentID);
			colliderComponent->contour.pos.x = position.x - width / 2;
			colliderComponent->contour.pos.y = position.y - height / 2;
			colliderComponent->contour.w = width;
			colliderComponent->contour.h = height;

			EntityManager::attachComponent(entity, colliderComponentID);
		}

		return entity;
	}

	/*
	=====================
	Scene::createTriangle
	=====================
	*/
	Entity Graphic::createTriangle(entity_collection_id collectionID, const fvec2 &position, float radius, const colora &color)
	{
		component_id drawableComponentID = ComponentManager::createDrawableComponent();
		component_id transformationComponentID = ComponentManager::createTransformationComponent(position, fvec2(radius, radius), 0.0f);

		DrawableComponent *drawableComponent = ComponentManager::getDrawableComponent(drawableComponentID);

		float vPos[] = {
			 0.0f, -0.5f, color.R, color.G, color.B, color.A,
			 0.5f,  0.5f, color.R, color.G, color.B, color.A,
			-0.5f,  0.5f, color.R, color.G, color.B, color.A
		};

		MemoryChunk chunk(vPos, sizeof(vPos));

		drawableComponent->vbo.bind();
		drawableComponent->vao.bind();
		drawableComponent->vbo.transmitData(chunk);
		drawableComponent->vbo.setVerticesNumber(3);
		drawableComponent->vbo.addAttribute(0, GLAttribComponentsNumber::x2, GLType::Float, 6 * sizeof(float), 0);
		drawableComponent->vbo.addAttribute(1, GLAttribComponentsNumber::x4, GLType::Float, 6 * sizeof(float), 2 * sizeof(float));

		Entity entity = EntityManager::createEntity(collectionID);
		EntityManager::attachComponent(entity, drawableComponentID);
		EntityManager::attachComponent(entity, transformationComponentID);

		return entity;
	}

}
