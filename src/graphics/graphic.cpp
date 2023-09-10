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
	Entity Graphic::createRectangle(gl_program program, entity_collection_id collectionID, const fvec3 &position, float width, float height, const colora &color, bool collidable)
	{
		component_id drawableComponentID       = ComponentManager::createDrawableComponent(program, GLVBO::create(), GLVAO::create());
		component_id transformationComponentID = ComponentManager::createTransformationComponent(position, fvec3(width, height, 1.0f), 0.0f);

		DrawableComponent *drawableComponent = ComponentManager::getDrawableComponent(drawableComponentID);
		drawableComponent->texture = GLTexture::getWhiteTexture();
		drawableComponent->textureUnit = 0;
		drawableComponent->renderCallback = DrawableComponent::classicRenderCallback;

		float vPos[] = {
			-0.5f,	-0.5f, color.R, color.G, color.B, color.A, 0.0f, 1.0f,
			 0.5f,	-0.5f, color.R, color.G, color.B, color.A, 1.0f, 1.0f,
			 0.5f,	 0.5f, color.R, color.G, color.B, color.A, 1.0f, 0.0f, 
			-0.5f,	-0.5f, color.R, color.G, color.B, color.A, 0.0f, 1.0f,
			 0.5f,	 0.5f, color.R, color.G, color.B, color.A, 1.0f, 0.0f,
			-0.5f,   0.5f, color.R, color.G, color.B, color.A, 0.0f, 0.0f
		};

		MemoryChunk chunk(vPos, sizeof(vPos));

		GLVBO::bind(drawableComponent->vbo);
		GLVAO::bind(drawableComponent->vao);

		GLVBO::transmitData(chunk);
		GLVBO::setVerticesNumber(6);

		GLVBO::addAttribute(0, GLAttribComponentsNumber::x2, GLType::Float, 8 * sizeof(float), 0);
		GLVBO::addAttribute(1, GLAttribComponentsNumber::x4, GLType::Float, 8 * sizeof(float), 2 * sizeof(float));
		GLVBO::addAttribute(2, GLAttribComponentsNumber::x2, GLType::Float, 8 * sizeof(float), 6 * sizeof(float));

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
	===============================
	Graphic::createRectangleTexture
	===============================
	*/
	Entity Graphic::createRectangleTexture(gl_program program, entity_collection_id collectionID, const fvec3 &position, float width, float height, const colora &color, gl_texture texture, uint8_t textureUnit, bool collidable)
	{
		component_id drawableComponentID       = ComponentManager::createDrawableComponent(program, GLVBO::create(), GLVAO::create(), texture, textureUnit);
		component_id transformationComponentID = ComponentManager::createTransformationComponent(position, fvec3(width, height, 1.0f), 0.0f);

		DrawableComponent *drawableComponent = ComponentManager::getDrawableComponent(drawableComponentID);
		drawableComponent->renderCallback = DrawableComponent::classicRenderCallback;

		float vPos[] = {
			// Positions   // Couleur                          // Coordonnées de texture
			-0.5f,	-0.5f, color.R, color.G, color.B, color.A, 0.0f, 1.0f, // Haut gauche
			 0.5f,	-0.5f, color.R, color.G, color.B, color.A, 1.0f, 1.0f, // Haut droite
			 0.5f,	 0.5f, color.R, color.G, color.B, color.A, 1.0f, 0.0f, // Bas droite
			-0.5f,	-0.5f, color.R, color.G, color.B, color.A, 0.0f, 1.0f, // Haut gauche
			 0.5f,	 0.5f, color.R, color.G, color.B, color.A, 1.0f, 0.0f, // Bas droite
			-0.5f,   0.5f, color.R, color.G, color.B, color.A, 0.0f, 0.0f  // Bas gauche
		};

		MemoryChunk chunk(vPos, sizeof(vPos));

		GLVBO::bind(drawableComponent->vbo);
		GLVAO::bind(drawableComponent->vao);

		GLVBO::transmitData(chunk);
		GLVBO::setVerticesNumber(6);
		
		GLVBO::addAttribute(0, GLAttribComponentsNumber::x2, GLType::Float, 8 * sizeof(float), 0);
		GLVBO::addAttribute(1, GLAttribComponentsNumber::x4, GLType::Float, 8 * sizeof(float), 2 * sizeof(float));
		GLVBO::addAttribute(2, GLAttribComponentsNumber::x2, GLType::Float, 8 * sizeof(float), 6 * sizeof(float));

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
	Entity Graphic::create3DRectangle(gl_program program, entity_collection_id collectionID, const fvec3 &position, float width, float height, float length, const colora &color1, const colora &color2, const colora &color3, const colora &color4, const colora &color5, const colora &color6, bool collidable)
	{
		component_id drawableComponentID       = ComponentManager::createDrawableComponent(program, GLVBO::create(), GLVAO::create());
		component_id transformationComponentID = ComponentManager::createTransformationComponent(position, fvec3(width, height, length), 0.0f);

		DrawableComponent *drawableComponent = ComponentManager::getDrawableComponent(drawableComponentID);
		drawableComponent->texture = GLTexture::getWhiteTexture();
		drawableComponent->textureUnit = 0;
		drawableComponent->renderCallback = DrawableComponent::classicRenderCallback;

		float vPos[] = {

			// Face avant
			-0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 0.0f, 1.0f,  // Haut gauche
			 0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 1.0f, 0.0f,  // Bas droite
			 0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 1.0f, 1.0f,  // Haut droite
			 
			-0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 1.0f, 0.0f,  // Haut gauche
			-0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 0.0f, 1.0f,  // Bas gauche
			 0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 0.0f, 0.0f,  // Bas droite
			
			// Face arrière
			 0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 0.0f, 1.0f,  // Haut gauche
			-0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 1.0f, 0.0f,  // Bas droite
			-0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 1.0f, 1.0f,  // Haut droite

			 0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 1.0f, 0.0f,  // Haut gauche
			 0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 0.0f, 1.0f,  // Bas gauche
			-0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 0.0f, 0.0f,  // Bas droite
			

			// Face droite
			-0.5f,  0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A, 0.0f, 1.0f,  // Bas gauche
			-0.5f,  0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A, 1.0f, 1.0f,  // Bas droite
			-0.5f, -0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A, 1.0f, 0.0f,  // Haut droite

			-0.5f, -0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A, 1.0f, 0.0f,  // Haut droite
			-0.5f, -0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A, 0.0f, 0.0f,  // Haut gauche
			-0.5f,  0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A, 0.0f, 1.0f,  // Bas gauche


			// Face gauche
			 0.5f,  0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A, 0.0f, 1.0f,  // Bas droite
			 0.5f, -0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A, 1.0f, 0.0f,  // Haut gauche
			 0.5f,  0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A, 1.0f, 1.0f,  // Bas gauche

			 0.5f, -0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A, 1.0f, 0.0f,  // Haut gauche
			 0.5f,  0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A, 0.0f, 1.0f,  // Bas droite
			 0.5f, -0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A, 0.0f, 0.0f,  // Haut droite


			// Face bas
			0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A, 1.0f, 0.0f,  // Bas droite
			-0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A, 0.0f, 1.0f,  // Haut gauche
			 0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A, 1.0f, 1.0f,  // Haut droite

			 -0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A, 0.0f, 1.0f,  // Haut gauche
			 0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A, 1.0f, 0.0f,  // Bas droite
			-0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A, 0.0f, 0.0f,  // Bas gauche
			

			// Face haut
			-0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A, 0.0f, 1.0f,  // Haut gauche
			 0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A, 1.0f, 0.0f,  // Bas droite
			 0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A, 1.0f, 1.0f,  // Haut droite

			 0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A, 1.0f, 0.0f,  // Bas droite
			-0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A, 0.0f, 1.0f,   // Haut gauche
			-0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A, 0.0f, 0.0f  // Bas gauche
			
		};

		MemoryChunk chunk(vPos, sizeof(vPos));

		GLVBO::bind(drawableComponent->vbo);
		GLVAO::bind(drawableComponent->vao);

		GLVBO::transmitData(chunk);
		GLVBO::setVerticesNumber(36);

		GLVBO::addAttribute(0, GLAttribComponentsNumber::x3, GLType::Float, 9 * sizeof(float), 0);
		GLVBO::addAttribute(1, GLAttribComponentsNumber::x4, GLType::Float, 9 * sizeof(float), 3 * sizeof(float));
		GLVBO::addAttribute(2, GLAttribComponentsNumber::x2, GLType::Float, 9 * sizeof(float), 7 * sizeof(float));

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
	=================================
	Graphic::create3DRectangleTexture
	=================================
	*/
	Entity Graphic::create3DRectangleTexture(gl_program program, entity_collection_id collectionID, const fvec3 &position, float width, float height, float length, const colora &color1, const colora &color2, const colora &color3, const colora &color4, const colora &color5, const colora &color6, gl_texture texture, uint8_t textureUnit, bool collidable)
	{
		component_id drawableComponentID       = ComponentManager::createDrawableComponent(program, GLVBO::create(), GLVAO::create(), texture, textureUnit);
		component_id transformationComponentID = ComponentManager::createTransformationComponent(position, fvec3(width, height, length), 0.0f);

		DrawableComponent *drawableComponent = ComponentManager::getDrawableComponent(drawableComponentID);
		drawableComponent->renderCallback = DrawableComponent::classicRenderCallback;

		float vPos[] = {

			// Face avant
			-0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 0.0f, 1.0f,  // Haut gauche
			 0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 1.0f, 0.0f,  // Bas droite
			 0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 1.0f, 1.0f,  // Haut droite
			 
			-0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 1.0f, 0.0f,  // Haut gauche
			-0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 0.0f, 1.0f,  // Bas gauche
			 0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 0.0f, 0.0f,  // Bas droite
			
			// Face arrière
			 0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 0.0f, 1.0f,  // Haut gauche
			-0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 1.0f, 0.0f,  // Bas droite
			-0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 1.0f, 1.0f,  // Haut droite

			 0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 1.0f, 0.0f,  // Haut gauche
			 0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 0.0f, 1.0f,  // Bas gauche
			-0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 0.0f, 0.0f,  // Bas droite
			

			// Face droite
			-0.5f,  0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A, 0.0f, 1.0f,  // Bas gauche
			-0.5f,  0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A, 1.0f, 1.0f,  // Bas droite
			-0.5f, -0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A, 1.0f, 0.0f,  // Haut droite

			-0.5f, -0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A, 1.0f, 0.0f,  // Haut droite
			-0.5f, -0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A, 0.0f, 0.0f,  // Haut gauche
			-0.5f,  0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A, 0.0f, 1.0f,  // Bas gauche


			// Face gauche
			 0.5f,  0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A, 0.0f, 1.0f,  // Bas droite
			 0.5f, -0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A, 1.0f, 0.0f,  // Haut gauche
			 0.5f,  0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A, 1.0f, 1.0f,  // Bas gauche

			 0.5f, -0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A, 1.0f, 0.0f,  // Haut gauche
			 0.5f,  0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A, 0.0f, 1.0f,  // Bas droite
			 0.5f, -0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A, 0.0f, 0.0f,  // Haut droite


			// Face bas
			0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A, 1.0f, 0.0f,  // Bas droite
			-0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A, 0.0f, 1.0f,  // Haut gauche
			 0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A, 1.0f, 1.0f,  // Haut droite

			 -0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A, 0.0f, 1.0f,  // Haut gauche
			 0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A, 1.0f, 0.0f,  // Bas droite
			-0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A, 0.0f, 0.0f,  // Bas gauche
			

			// Face haut
			-0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A, 0.0f, 1.0f,  // Haut gauche
			 0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A, 1.0f, 0.0f,  // Bas droite
			 0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A, 1.0f, 1.0f,  // Haut droite

			 0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A, 1.0f, 0.0f,  // Bas droite
			-0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A, 0.0f, 1.0f,   // Haut gauche
			-0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A, 0.0f, 0.0f  // Bas gauche
			
		};

		MemoryChunk chunk(vPos, sizeof(vPos));

		GLVBO::bind(drawableComponent->vbo);
		GLVAO::bind(drawableComponent->vao);

		GLVBO::transmitData(chunk);
		GLVBO::setVerticesNumber(36);

		GLVBO::addAttribute(0, GLAttribComponentsNumber::x3, GLType::Float, 9 * sizeof(float), 0);
		GLVBO::addAttribute(1, GLAttribComponentsNumber::x4, GLType::Float, 9 * sizeof(float), 3 * sizeof(float));
		GLVBO::addAttribute(2, GLAttribComponentsNumber::x2, GLType::Float, 9 * sizeof(float), 7 * sizeof(float));

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

	Entity Graphic::createCubemap(
		gl_program program,
		entity_collection_id collectionID,
		const fvec3 &position,
		float width,
		float height,
		float length,
		gl_texture texture,
		uint8_t textureUnit,
		bool collidable
	)
	{
		component_id drawableComponentID       = ComponentManager::createDrawableComponent(program, GLVBO::create(), GLVAO::create(), texture, textureUnit);
		component_id transformationComponentID = ComponentManager::createTransformationComponent(position, fvec3(width, height, length), 0.0f);

		DrawableComponent *drawableComponent = ComponentManager::getDrawableComponent(drawableComponentID);
		drawableComponent->renderCallback = DrawableComponent::skyboxRenderCallback;

		float vPos[] = {
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			

			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 -1.0f,  1.0f, -1.0f,
			 


			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,


			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,


			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,


			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,


			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f

		};

		MemoryChunk chunk(vPos, sizeof(vPos));

		GLVBO::bind(drawableComponent->vbo);
		GLVAO::bind(drawableComponent->vao);

		GLVBO::transmitData(chunk);
		GLVBO::setVerticesNumber(36);

		GLVBO::addAttribute(0, GLAttribComponentsNumber::x3, GLType::Float, 3 * sizeof(float), 0);

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

}
