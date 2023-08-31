#include <DE/graphics/graphic.hpp>
#include <DE/graphics/vertex.hpp>

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
	========================
	Graphic::createRectangle
	========================
	*/
	Entity Graphic::createRectangle(entity_collection_id collectionID, const fvec3 &position, float width, float height, const colora &color, bool collidable)
	{
		component_id drawableComponentID       = ComponentManager::createDrawableComponent();
		component_id transformationComponentID = ComponentManager::createTransformationComponent(position, fvec3(width, height, 1.0f), 0.0f);

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
	==========================
	Graphic::create3DRectangle
	==========================
	*/
	Entity Graphic::create3DRectangle(entity_collection_id collectionID, const fvec3 &position, float width, float height, float length, const colora &color1, const colora &color2, const colora &color3, const colora &color4, const colora &color5, const colora &color6, bool collidable)
	{
		component_id drawableComponentID       = ComponentManager::createDrawableComponent();
		component_id transformationComponentID = ComponentManager::createTransformationComponent(position, fvec3(width, height, length), 0.0f);

		DrawableComponent *drawableComponent = ComponentManager::getDrawableComponent(drawableComponentID);

		float vPos[] = {
			-0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A,
			 0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A,
			 0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A,
			 0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A,
			-0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A,
			-0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A,

			-0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A,
			 0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A,
			 0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A,
			 0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A,
			-0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A,
			-0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A,

			-0.5f,  0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A,
			-0.5f,  0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A,
			-0.5f, -0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A,
			-0.5f, -0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A,
			-0.5f, -0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A,
			-0.5f,  0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A,

			 0.5f,  0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A,
			 0.5f,  0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A,
			 0.5f, -0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A,
			 0.5f, -0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A,
			 0.5f, -0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A,
			 0.5f,  0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A,

			-0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A,
			 0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A,
			 0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A,
			 0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A,
			-0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A,
			-0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A,

			-0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A,
			 0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A,
			 0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A,
			 0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A,
			-0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A,
			-0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A
		};

		MemoryChunk chunk(vPos, sizeof(vPos));

		drawableComponent->vbo.bind();
		drawableComponent->vao.bind();
		drawableComponent->vbo.transmitData(chunk);
		drawableComponent->vbo.setVerticesNumber(36);
		drawableComponent->vbo.addAttribute(0, GLAttribComponentsNumber::x3, GLType::Float, 7 * sizeof(float), 0);
		drawableComponent->vbo.addAttribute(1, GLAttribComponentsNumber::x4, GLType::Float, 7 * sizeof(float), 3 * sizeof(float));

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
	Entity Graphic::createTriangle(entity_collection_id collectionID, const fvec3 &position, float radius, const colora &color)
	{
		component_id drawableComponentID = ComponentManager::createDrawableComponent();
		component_id transformationComponentID = ComponentManager::createTransformationComponent(position, fvec3(radius, radius, 1.0f), 0.0f);

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
