#include <DE/graphics/graphic.hpp>
#include <DE/graphics/vertex.hpp>

#include <DE/ecs/entity.hpp>
#include <DE/ecs/component.hpp>

namespace deep
{

	/*
	==========
	rect::rect
	==========
	*/
	rect::rect(const fvec2 &position, float width, float height)
		: pos(position),
		  w(width),
		  h(height)
	{ }

    /*
	================
	polygon::polygon
	================
	*/
    polygon::polygon(vbo_id vbo, vao_id vao)
        : m_VBO(vbo),
          m_VAO(vao)
    { }

	/*
	========================
	graphic::createRectangle
	========================
	*/
	entity graphic::createRectangle(
        const char *name,
        program_id program,
        entity_collection_id collectionID,
        const fvec3 &position,
        float width,
        float height,
        const colora &color,
        bool collidable)
	{
		component_id drawableComponentID       = component_manager::createDrawableComponent(program, vbo_manager::create(name), vao_manager::create(name));
		component_id transformationComponentID = component_manager::createTransformationComponent(position, fvec3(width, height, 1.0f), 0.0f);

		drawable_component *drawableComponent = component_manager::getDrawableComponent(drawableComponentID);
		drawableComponent->texture = texture_manager::getWhiteTexture();
		drawableComponent->textureUnit = 0;
		drawableComponent->renderCallback = drawable_component::classicRenderCallback;

		float vPos[] =
        {
			-0.5f,	-0.5f, color.R, color.G, color.B, color.A, 0.0f, 1.0f,
			 0.5f,	-0.5f, color.R, color.G, color.B, color.A, 1.0f, 1.0f,
			 0.5f,	 0.5f, color.R, color.G, color.B, color.A, 1.0f, 0.0f, 
			-0.5f,	-0.5f, color.R, color.G, color.B, color.A, 0.0f, 1.0f,
			 0.5f,	 0.5f, color.R, color.G, color.B, color.A, 1.0f, 0.0f,
			-0.5f,   0.5f, color.R, color.G, color.B, color.A, 0.0f, 0.0f
		};

		memory_chunk chunk(vPos, sizeof(vPos));

		vbo_manager::bind(drawableComponent->vbo);
		vao_manager::bind(drawableComponent->vao);

		vbo_manager::transmitData(chunk);
		vbo_manager::setVerticesNumber(6);

		vbo_manager::addAttribute(0, gl_attrib_components_number::x2, gl_type::Float, 8 * sizeof(float), 0);
		vbo_manager::addAttribute(1, gl_attrib_components_number::x4, gl_type::Float, 8 * sizeof(float), 2 * sizeof(float));
		vbo_manager::addAttribute(2, gl_attrib_components_number::x2, gl_type::Float, 8 * sizeof(float), 6 * sizeof(float));

		entity entity = entity_manager::createEntity(collectionID);
		entity_manager::attachComponent(entity, drawableComponentID);
		entity_manager::attachComponent(entity, transformationComponentID);

		if(collidable)
        {
			component_id colliderComponentID = component_manager::createColliderComponent();

			collider_component *colliderComponent = component_manager::getColliderComponent(colliderComponentID);
			colliderComponent->contour.pos.x = position.x - width / 2;
			colliderComponent->contour.pos.y = position.y - height / 2;
			colliderComponent->contour.w = width;
			colliderComponent->contour.h = height;

			entity_manager::attachComponent(entity, colliderComponentID);
		}

		return entity;
	}

	/*
	===============================
	graphic::createRectangleTexture
	===============================
	*/
	entity graphic::createRectangleTexture(
        const char *name,
        program_id program,
        entity_collection_id collectionID,
        const fvec3 &position,
        float width,
        float height,
        const colora &color,
        texture_id texture,
        uint8_t textureUnit,
        bool collidable)
	{
		component_id drawableComponentID       = component_manager::createDrawableComponent(program, vbo_manager::create(name), vao_manager::create(name), texture, textureUnit);
		component_id transformationComponentID = component_manager::createTransformationComponent(position, fvec3(width, height, 1.0f), 0.0f);

		drawable_component *drawableComponent = component_manager::getDrawableComponent(drawableComponentID);
		drawableComponent->renderCallback = drawable_component::classicRenderCallback;

		float vPos[] =
        {
			// Positions   // Couleur                          // Coordonnées de texture
			-0.5f,	-0.5f, color.R, color.G, color.B, color.A, 0.0f, 1.0f, // Haut gauche
			 0.5f,	-0.5f, color.R, color.G, color.B, color.A, 1.0f, 1.0f, // Haut droite
			 0.5f,	 0.5f, color.R, color.G, color.B, color.A, 1.0f, 0.0f, // Bas droite
			-0.5f,	-0.5f, color.R, color.G, color.B, color.A, 0.0f, 1.0f, // Haut gauche
			 0.5f,	 0.5f, color.R, color.G, color.B, color.A, 1.0f, 0.0f, // Bas droite
			-0.5f,   0.5f, color.R, color.G, color.B, color.A, 0.0f, 0.0f  // Bas gauche
		};

		memory_chunk chunk(vPos, sizeof(vPos));
        
		vbo_manager::bind(drawableComponent->vbo);
		vao_manager::bind(drawableComponent->vao);

		vbo_manager::transmitData(chunk);
		vbo_manager::setVerticesNumber(6);
		
		vbo_manager::addAttribute(0, gl_attrib_components_number::x2, gl_type::Float, 8 * sizeof(float), 0);
		vbo_manager::addAttribute(1, gl_attrib_components_number::x4, gl_type::Float, 8 * sizeof(float), 2 * sizeof(float));
		vbo_manager::addAttribute(2, gl_attrib_components_number::x2, gl_type::Float, 8 * sizeof(float), 6 * sizeof(float));

		entity entity = entity_manager::createEntity(collectionID);
		entity_manager::attachComponent(entity, drawableComponentID);
		entity_manager::attachComponent(entity, transformationComponentID);

		if(collidable)
        {
			component_id colliderComponentID = component_manager::createColliderComponent();

			collider_component *colliderComponent = component_manager::getColliderComponent(colliderComponentID);
			colliderComponent->contour.pos.x = position.x - width / 2;
			colliderComponent->contour.pos.y = position.y - height / 2;
			colliderComponent->contour.w = width;
			colliderComponent->contour.h = height;

			entity_manager::attachComponent(entity, colliderComponentID);
		}

		return entity;
	}

	/*
	==========================
	graphic::create3DRectangle
	==========================
	*/
	entity graphic::create3DRectangle(
        const char *name,
        program_id program,
        entity_collection_id collectionID,
        const fvec3 &position,
        float width,
        float height,
        float length,
        const colora &color1,
        const colora &color2,
        const colora &color3,
        const colora &color4,
        const colora &color5,
        const colora &color6,
        bool collidable)
	{
		component_id drawableComponentID       = component_manager::createDrawableComponent(program, vbo_manager::create(name), vao_manager::create(name));
		component_id transformationComponentID = component_manager::createTransformationComponent(position, fvec3(width, height, length), 0.0f);

		drawable_component *drawableComponent = component_manager::getDrawableComponent(drawableComponentID);
		drawableComponent->texture = texture_manager::getWhiteTexture();
		drawableComponent->textureUnit = 0;
		drawableComponent->renderCallback = drawable_component::classicRenderCallback;

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

		memory_chunk chunk(vPos, sizeof(vPos));

		vbo_manager::bind(drawableComponent->vbo);
		vao_manager::bind(drawableComponent->vao);

		vbo_manager::transmitData(chunk);
		vbo_manager::setVerticesNumber(36);

		vbo_manager::addAttribute(0, gl_attrib_components_number::x3, gl_type::Float, 9 * sizeof(float), 0);
		vbo_manager::addAttribute(1, gl_attrib_components_number::x4, gl_type::Float, 9 * sizeof(float), 3 * sizeof(float));
		vbo_manager::addAttribute(2, gl_attrib_components_number::x2, gl_type::Float, 9 * sizeof(float), 7 * sizeof(float));

		entity entity = entity_manager::createEntity(collectionID);
		entity_manager::attachComponent(entity, drawableComponentID);
		entity_manager::attachComponent(entity, transformationComponentID);

		if(collidable)
        {
			component_id colliderComponentID = component_manager::createColliderComponent();

			collider_component *colliderComponent = component_manager::getColliderComponent(colliderComponentID);
			colliderComponent->contour.pos.x = position.x - width / 2;
			colliderComponent->contour.pos.y = position.y - height / 2;
			colliderComponent->contour.w = width;
			colliderComponent->contour.h = height;

			entity_manager::attachComponent(entity, colliderComponentID);
		}

		return entity;
	}

	/*
	=================================
	graphic::create3DRectangleTexture
	=================================
	*/
	entity graphic::create3DRectangleTexture(
        const char *name,
        program_id program,
        entity_collection_id collectionID,
        const fvec3 &position,
        float width,
        float height,
        float length,
        const colora &color1,
        const colora &color2,
        const colora &color3,
        const colora &color4,
        const colora &color5,
        const colora &color6,
        texture_id texture,
        uint8_t textureUnit,
        bool collidable)
	{
		component_id drawableComponentID       = component_manager::createDrawableComponent(program, vbo_manager::create(name), vao_manager::create(name), texture, textureUnit);
		component_id transformationComponentID = component_manager::createTransformationComponent(position, fvec3(width, height, length), 0.0f);

		drawable_component *drawableComponent = component_manager::getDrawableComponent(drawableComponentID);
		drawableComponent->renderCallback = drawable_component::classicRenderCallback;

		float vPos[] =
        {

			// Face avant
			-0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 1.0f, 1.0f,  // Haut gauche
			 0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 0.0f, 0.0f,  // Bas droite
			 0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 0.0f, 1.0f,  // Haut droite
			 
			-0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 1.0f, 1.0f,  // Haut gauche
			-0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 1.0f, 0.0f,  // Bas gauche
			 0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 0.0f, 0.0f,  // Bas droite
			
			// Face arrière
			 0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 1.0f, 1.0f,  // Haut gauche
			-0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 0.0f, 0.0f,  // Bas droite
			-0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 0.0f, 1.0f,  // Haut droite

			 0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 1.0f, 1.0f,  // Haut gauche
			 0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 1.0f, 0.0f,  // Bas gauche
			-0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 0.0f, 0.0f,  // Bas droite
			

			// Face droite
			-0.5f,  0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A, 1.0f, 0.0f,  // Bas gauche
			-0.5f,  0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A, 0.0f, 0.0f,  // Bas droite
			-0.5f, -0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A, 0.0f, 1.0f,  // Haut droite

			-0.5f, -0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A, 0.0f, 1.0f,  // Haut droite
			-0.5f, -0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A, 1.0f, 1.0f,  // Haut gauche
			-0.5f,  0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A, 1.0f, 0.0f,  // Bas gauche


			// Face gauche
			 0.5f,  0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A, 0.0f, 0.0f,  // Bas droite
			 0.5f, -0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A, 1.0f, 1.0f,  // Haut gauche
			 0.5f,  0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A, 1.0f, 0.0f,  // Bas gauche

			 0.5f, -0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A, 1.0f, 1.0f,  // Haut gauche
			 0.5f,  0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A, 0.0f, 0.0f,  // Bas droite
			 0.5f, -0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A, 0.0f, 1.0f,  // Haut droite


			// Face bas
			 0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A, 1.0f, 0.0f,  // Bas droite
			-0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A, 0.0f, 1.0f,  // Haut gauche
			 0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A, 1.0f, 1.0f,  // Haut droite

			-0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A, 0.0f, 1.0f,  // Haut gauche
			 0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A, 1.0f, 0.0f,  // Bas droite
			-0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A, 0.0f, 0.0f,  // Bas gauche
			

			// Face haut
			-0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A, 1.0f, 1.0f,  // Haut gauche
			 0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A, 0.0f, 0.0f,  // Bas droite
			 0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A, 0.0f, 1.0f,  // Haut droite

			 0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A, 0.0f, 0.0f,  // Bas droite
			-0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A, 1.0f, 1.0f,  // Haut gauche
			-0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A, 1.0f, 0.0f   // Bas gauche
			
		};

		memory_chunk chunk(vPos, sizeof(vPos));

		vbo_manager::bind(drawableComponent->vbo);
		vao_manager::bind(drawableComponent->vao);

		vbo_manager::transmitData(chunk);
		vbo_manager::setVerticesNumber(36);

		vbo_manager::addAttribute(0, gl_attrib_components_number::x3, gl_type::Float, 9 * sizeof(float), 0);
		vbo_manager::addAttribute(1, gl_attrib_components_number::x4, gl_type::Float, 9 * sizeof(float), 3 * sizeof(float));
		vbo_manager::addAttribute(2, gl_attrib_components_number::x2, gl_type::Float, 9 * sizeof(float), 7 * sizeof(float));

		entity entity = entity_manager::createEntity(collectionID);
		entity_manager::attachComponent(entity, drawableComponentID);
		entity_manager::attachComponent(entity, transformationComponentID);

		if(collidable)
        {
			component_id colliderComponentID = component_manager::createColliderComponent();

			collider_component *colliderComponent = component_manager::getColliderComponent(colliderComponentID);
			colliderComponent->contour.pos.x = position.x - width / 2;
			colliderComponent->contour.pos.y = position.y - height / 2;
			colliderComponent->contour.w = width;
			colliderComponent->contour.h = height;

			entity_manager::attachComponent(entity, colliderComponentID);
		}

		return entity;
	}

    polygon graphic::createCube(
        const char *name,
        const colora &color1,
	    const colora &color2,
	    const colora &color3,
	    const colora &color4,
	    const colora &color5,
	    const colora &color6
    )
    {
        float vPos[] =
        {

			// Face avant
			-0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 1.0f, 1.0f,  // Haut gauche
			 0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 0.0f, 0.0f, // Bas droite
			 0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 0.0f, 1.0f, // Haut droite
			 
			-0.5f, -0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 1.0f, 1.0f, // Haut gauche
			-0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 1.0f, 0.0f, // Bas gauche
			 0.5f,  0.5f, -0.5f, color1.R, color1.G, color1.B, color1.A, 0.0f, 0.0f, // Bas droite
			
			// Face arrière
			 0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 1.0f, 1.0f, // Haut gauche
			-0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 0.0f, 0.0f, // Bas droite
			-0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 0.0f, 1.0f, // Haut droite

			 0.5f, -0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 1.0f, 1.0f, // Haut gauche
			 0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 1.0f, 0.0f, // Bas gauche
			-0.5f,  0.5f,  0.5f, color2.R, color2.G, color2.B, color2.A, 0.0f, 0.0f, // Bas droite
			

			// Face droite
			-0.5f,  0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A, 1.0f, 0.0f, // Bas gauche
			-0.5f,  0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A, 0.0f, 0.0f, // Bas droite
			-0.5f, -0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A, 0.0f, 1.0f, // Haut droite

			-0.5f, -0.5f, -0.5f, color3.R, color3.G, color3.B, color3.A, 0.0f, 1.0f, // Haut droite
			-0.5f, -0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A, 1.0f, 1.0f, // Haut gauche
			-0.5f,  0.5f,  0.5f, color3.R, color3.G, color3.B, color3.A, 1.0f, 0.0f, // Bas gauche


			// Face gauche
			 0.5f,  0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A, 0.0f, 0.0f, // Bas droite
			 0.5f, -0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A, 1.0f, 1.0f, // Haut gauche
			 0.5f,  0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A, 1.0f, 0.0f, // Bas gauche

			 0.5f, -0.5f, -0.5f, color4.R, color4.G, color4.B, color4.A, 1.0f, 1.0f, // Haut gauche
			 0.5f,  0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A, 0.0f, 0.0f, // Bas droite
			 0.5f, -0.5f,  0.5f, color4.R, color4.G, color4.B, color4.A, 0.0f, 1.0f, // Haut droite


			// Face bas
			 0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A, 1.0f, 0.0f, // Bas droite
			-0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A, 0.0f, 1.0f, // Haut gauche
			 0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A, 1.0f, 1.0f, // Haut droite

			-0.5f, -0.5f, -0.5f, color5.R, color5.G, color5.B, color5.A, 0.0f, 1.0f, // Haut gauche
			 0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A, 1.0f, 0.0f, // Bas droite
			-0.5f, -0.5f,  0.5f, color5.R, color5.G, color5.B, color5.A, 0.0f, 0.0f, // Bas gauche
			

			// Face haut
			-0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A, 1.0f, 1.0f, // Haut gauche
			 0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A, 0.0f, 0.0f, // Bas droite
			 0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A, 0.0f, 1.0f, // Haut droite

			 0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A, 0.0f, 0.0f, // Bas droite
			-0.5f,  0.5f, -0.5f, color6.R, color6.G, color6.B, color6.A, 1.0f, 1.0f, // Haut gauche
			-0.5f,  0.5f,  0.5f, color6.R, color6.G, color6.B, color6.A, 1.0f, 0.0f // Bas gauche
			
		};

        polygon pol(vbo_manager::create(name), vao_manager::create(name));
        memory_chunk chunk(vPos, sizeof(vPos));

        vbo_manager::bind(pol.vbo());
		vao_manager::bind(pol.vao());

		vbo_manager::transmitData(chunk);
		vbo_manager::setVerticesNumber(36);

		vbo_manager::addAttribute(0, gl_attrib_components_number::x3, gl_type::Float, 9 * sizeof(float), 0);
		vbo_manager::addAttribute(1, gl_attrib_components_number::x4, gl_type::Float, 9 * sizeof(float), 3 * sizeof(float));
		vbo_manager::addAttribute(2, gl_attrib_components_number::x2, gl_type::Float, 9 * sizeof(float), 7 * sizeof(float));

        vao_manager::bindDefault();

        return pol;
    }

	entity graphic::createCubemap(
        const char *name,
		program_id program,
		entity_collection_id collectionID,
		const fvec3 &position,
		float width,
		float height,
		float length,
		texture_id texture,
		uint8_t textureUnit,
		bool collidable
	)
	{
		component_id drawableComponentID       = component_manager::createDrawableComponent(program, vbo_manager::create(name), vao_manager::create(name), texture, textureUnit);
		component_id transformationComponentID = component_manager::createTransformationComponent(position, fvec3(width, height, length), 0.0f);

		drawable_component *drawableComponent = component_manager::getDrawableComponent(drawableComponentID);
		drawableComponent->renderCallback = drawable_component::skyboxRenderCallback;

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

		memory_chunk chunk(vPos, sizeof(vPos));

		vbo_manager::bind(drawableComponent->vbo);
		vao_manager::bind(drawableComponent->vao);

		vbo_manager::transmitData(chunk);
		vbo_manager::setVerticesNumber(36);

		vbo_manager::addAttribute(0, gl_attrib_components_number::x3, gl_type::Float, 3 * sizeof(float), 0);

		entity entity = entity_manager::createEntity(collectionID);
		entity_manager::attachComponent(entity, drawableComponentID);
		entity_manager::attachComponent(entity, transformationComponentID);

		if(collidable)
        {
			component_id colliderComponentID = component_manager::createColliderComponent();

			collider_component *colliderComponent = component_manager::getColliderComponent(colliderComponentID);
			colliderComponent->contour.pos.x = position.x - width / 2;
			colliderComponent->contour.pos.y = position.y - height / 2;
			colliderComponent->contour.w = width;
			colliderComponent->contour.h = height;

			entity_manager::attachComponent(entity, colliderComponentID);
		}

		return entity;
	}

}
