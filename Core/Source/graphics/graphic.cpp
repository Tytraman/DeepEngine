#include "DE/graphics/graphic.hpp"
#include "DE/graphics/vertex.hpp"
#include "DE/ecs/entity.hpp"
#include "DE/ecs/component.hpp"
#include "DE/drivers/opengl/vbo.hpp"
#include "DE/drivers/opengl/vao.hpp"
#include "DE/drivers/opengl/texture.hpp"

namespace deep
{

    graphic::cube_texture_points::cube_texture_points()
        : frontTopLeft(1.0f, 1.0f),
          frontBottomRight(0.0f, 0.0f),
          frontTopRight(0.0f, 1.0f),
          frontBottomLeft(1.0f, 0.0f),

          backTopLeft(1.0f, 1.0f),
          backBottomRight(0.0f, 0.0f),
          backTopRight(0.0f, 1.0f),
          backBottomLeft(1.0f, 0.0f),

          rightBottomLeft(1.0f, 0.0f),
          rightBottomRight(0.0f, 0.0f),
          rightTopRight(0.0f, 1.0f),
          rightTopLeft(1.0f, 1.0f),

          leftBottomRight(0.0f, 0.0f),
          leftTopLeft(1.0f, 1.0f),
          leftBottomLeft(1.0f, 0.0f),
          leftTopRight(0.0f, 1.0f),

          bottomBottomRight(1.0f, 0.0f),
          bottomTopLeft(0.0f, 1.0f),
          bottomTopRight(1.0f, 1.0f),
          bottomBottomLeft(0.0f, 0.0f),

          topTopLeft(1.0f, 1.0f),
          topBottomRight(0.0f, 0.0f),
          topTopRight(0.0f, 1.0f),
          topBottomLeft(1.0f, 0.0f)
    { }

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
    polygon::polygon(GL3::gl_id vbo, GL3::gl_id vao)
        : m_VBO(vbo),
          m_VAO(vao)
    { }

    /*
    ========================
    graphic::createRectangle
    ========================
    */
    entity_manager::entity graphic::createRectangle(
        const char *name,
        GL3::gl_id program,
        entity_collection_id collectionID,
        const fvec3 &position,
        float width,
        float height,
        const colora &color,
        bool collidable)
    {
        component_manager *componentManager = component_manager::get_singleton();
        entity_manager *entityManager = entity_manager::get_singleton();
        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();
        GL3::texture_manager *textureManager = GL3::texture_manager::get_singleton();

        component_id drawableComponentID       = componentManager->create_drawable_component(program, vboManager->create(name), vaoManager->create(name));
        component_id transformationComponentID = componentManager->create_transformation_component(position, fvec3(width, height, 1.0f), 0.0f);

        drawable_component *drawableComponent = componentManager->get_drawable_component(drawableComponentID);
        drawableComponent->texture = textureManager->get_white_texture();
        drawableComponent->textureUnit = 0;
        drawableComponent->renderCallback = drawable_component::classic_render_callback;

        float vPos[] =
        {
            -0.5f,	-0.5f, static_cast<float>(color.R), static_cast<float>(color.G), static_cast<float>(color.B), static_cast<float>(color.A), 0.0f, 1.0f,
             0.5f,	-0.5f, static_cast<float>(color.R), static_cast<float>(color.G), static_cast<float>(color.B), static_cast<float>(color.A), 1.0f, 1.0f,
             0.5f,	 0.5f, static_cast<float>(color.R), static_cast<float>(color.G), static_cast<float>(color.B), static_cast<float>(color.A), 1.0f, 0.0f, 
            -0.5f,	-0.5f, static_cast<float>(color.R), static_cast<float>(color.G), static_cast<float>(color.B), static_cast<float>(color.A), 0.0f, 1.0f,
             0.5f,	 0.5f, static_cast<float>(color.R), static_cast<float>(color.G), static_cast<float>(color.B), static_cast<float>(color.A), 1.0f, 0.0f,
            -0.5f,   0.5f, static_cast<float>(color.R), static_cast<float>(color.G), static_cast<float>(color.B), static_cast<float>(color.A), 0.0f, 0.0f
        };

        memory_chunk chunk(vPos, sizeof(vPos));

        vboManager->bind(drawableComponent->vbo);
        vaoManager->bind(drawableComponent->vao);
        vaoManager->attach_vbo(drawableComponent->vao, drawableComponent->vbo);

        vboManager->transmit_data(chunk);
        vboManager->set_vertices_number(6);

        vboManager->add_attribute(0, GL3::gl_attrib_components_number::x2, GL3::gl_type::Float, 8 * sizeof(float), 0);
        vboManager->add_attribute(1, GL3::gl_attrib_components_number::x4, GL3::gl_type::Float, 8 * sizeof(float), 2 * sizeof(float));
        vboManager->add_attribute(2, GL3::gl_attrib_components_number::x2, GL3::gl_type::Float, 8 * sizeof(float), 6 * sizeof(float));

        entity_manager::entity entity = entityManager->create_entity(name, collectionID);
        entityManager->attach_component(name, collectionID, drawableComponentID);
        entityManager->attach_component(name, collectionID, transformationComponentID);

        if(collidable)
        {
            component_id colliderComponentID = componentManager->create_collider_component();

            collider_component *colliderComponent = componentManager->get_collider_component(colliderComponentID);
            colliderComponent->contour.pos.x = position.x - width / 2;
            colliderComponent->contour.pos.y = position.y - height / 2;
            colliderComponent->contour.w = width;
            colliderComponent->contour.h = height;

            entityManager->attach_component(name, collectionID, colliderComponentID);
        }

        return entity;
    }

    /*
    ===============================
    graphic::createRectangleTexture
    ===============================
    */
    entity_manager::entity graphic::createRectangleTexture(
        const char *name,
        GL3::gl_id program,
        entity_collection_id collectionID,
        const fvec3 &position,
        float width,
        float height,
        const colora &color,
        GL3::gl_id texture,
        uint8_t textureUnit,
        bool collidable)
    {
        component_manager *componentManager = component_manager::get_singleton();
        entity_manager *entityManager = entity_manager::get_singleton();
        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();

        component_id drawableComponentID       = componentManager->create_drawable_component(program, vboManager->create(name), vaoManager->create(name), texture, textureUnit);
        component_id transformationComponentID = componentManager->create_transformation_component(position, fvec3(width, height, 1.0f), 0.0f);

        drawable_component *drawableComponent = componentManager->get_drawable_component(drawableComponentID);
        drawableComponent->renderCallback = drawable_component::classic_render_callback;

        float vPos[] =
        {
            // Positions   // Couleur                          // Coordonnées de texture
            -0.5f,	-0.5f, static_cast<float>(color.R), static_cast<float>(color.G), static_cast<float>(color.B), static_cast<float>(color.A), 0.0f, 1.0f, // Haut gauche
             0.5f,	-0.5f, static_cast<float>(color.R), static_cast<float>(color.G), static_cast<float>(color.B), static_cast<float>(color.A), 1.0f, 1.0f, // Haut droite
             0.5f,	 0.5f, static_cast<float>(color.R), static_cast<float>(color.G), static_cast<float>(color.B), static_cast<float>(color.A), 1.0f, 0.0f, // Bas droite
            -0.5f,	-0.5f, static_cast<float>(color.R), static_cast<float>(color.G), static_cast<float>(color.B), static_cast<float>(color.A), 0.0f, 1.0f, // Haut gauche
             0.5f,	 0.5f, static_cast<float>(color.R), static_cast<float>(color.G), static_cast<float>(color.B), static_cast<float>(color.A), 1.0f, 0.0f, // Bas droite
            -0.5f,   0.5f, static_cast<float>(color.R), static_cast<float>(color.G), static_cast<float>(color.B), static_cast<float>(color.A), 0.0f, 0.0f  // Bas gauche
        };

        memory_chunk chunk(vPos, sizeof(vPos));
        
        vboManager->bind(drawableComponent->vbo);
        vaoManager->bind(drawableComponent->vao);
        vaoManager->attach_vbo(drawableComponent->vao, drawableComponent->vbo);

        vboManager->transmit_data(chunk);
        vboManager->set_vertices_number(6);
        
        vboManager->add_attribute(0, GL3::gl_attrib_components_number::x2, GL3::gl_type::Float, 8 * sizeof(float), 0);
        vboManager->add_attribute(1, GL3::gl_attrib_components_number::x4, GL3::gl_type::Float, 8 * sizeof(float), 2 * sizeof(float));
        vboManager->add_attribute(2, GL3::gl_attrib_components_number::x2, GL3::gl_type::Float, 8 * sizeof(float), 6 * sizeof(float));

        entity_manager::entity entity = entityManager->create_entity(name, collectionID);
        entityManager->attach_component(name, collectionID, drawableComponentID);
        entityManager->attach_component(name, collectionID, transformationComponentID);

        if(collidable)
        {
            component_id colliderComponentID = componentManager->create_collider_component();

            collider_component *colliderComponent = componentManager->get_collider_component(colliderComponentID);
            colliderComponent->contour.pos.x = position.x - width / 2;
            colliderComponent->contour.pos.y = position.y - height / 2;
            colliderComponent->contour.w = width;
            colliderComponent->contour.h = height;

            entityManager->attach_component(name, collectionID, colliderComponentID);
        }

        return entity;
    }

    /*
    ==========================
    graphic::create3DRectangle
    ==========================
    */
    entity_manager::entity graphic::create3DRectangle(
        const char *name,
        GL3::gl_id program,
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
        component_manager *componentManager = component_manager::get_singleton();
        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();
        GL3::texture_manager *textureManager = GL3::texture_manager::get_singleton();

        component_id drawableComponentID       = componentManager->create_drawable_component(program, vboManager->create(name), vaoManager->create(name));
        component_id transformationComponentID = componentManager->create_transformation_component(position, fvec3(width, height, length), 0.0f);

        drawable_component *drawableComponent = componentManager->get_drawable_component(drawableComponentID);
        drawableComponent->texture = textureManager->get_white_texture();
        drawableComponent->textureUnit = 0;
        drawableComponent->renderCallback = drawable_component::classic_render_callback;

        float vPos[] = {

            // Face avant
            -0.5f, -0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), 0.0f, 1.0f,  // Haut gauche
             0.5f,  0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), 1.0f, 0.0f,  // Bas droite
             0.5f, -0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), 1.0f, 1.0f,  // Haut droite
             
            -0.5f, -0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), 1.0f, 0.0f,  // Haut gauche
            -0.5f,  0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), 0.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), 0.0f, 0.0f,  // Bas droite
            
            // Face arrière
             0.5f, -0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), 0.0f, 1.0f,  // Haut gauche
            -0.5f,  0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), 1.0f, 0.0f,  // Bas droite
            -0.5f, -0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), 1.0f, 1.0f,  // Haut droite

             0.5f, -0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), 1.0f, 0.0f,  // Haut gauche
             0.5f,  0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), 0.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), 0.0f, 0.0f,  // Bas droite
            

            // Face droite
            -0.5f,  0.5f,  0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), 0.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f, -0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), 1.0f, 1.0f,  // Bas droite
            -0.5f, -0.5f, -0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), 1.0f, 0.0f,  // Haut droite

            -0.5f, -0.5f, -0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), 1.0f, 0.0f,  // Haut droite
            -0.5f, -0.5f,  0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), 0.0f, 0.0f,  // Haut gauche
            -0.5f,  0.5f,  0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), 0.0f, 1.0f,  // Bas gauche


            // Face gauche
             0.5f,  0.5f,  0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), 0.0f, 1.0f,  // Bas droite
             0.5f, -0.5f, -0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), 1.0f, 0.0f,  // Haut gauche
             0.5f,  0.5f, -0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), 1.0f, 1.0f,  // Bas gauche

             0.5f, -0.5f, -0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), 1.0f, 0.0f,  // Haut gauche
             0.5f,  0.5f,  0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), 0.0f, 1.0f,  // Bas droite
             0.5f, -0.5f,  0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), 0.0f, 0.0f,  // Haut droite


             // Face bas
             0.5f, -0.5f,  0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), 1.0f, 0.0f,  // Bas droite
            -0.5f, -0.5f, -0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), 0.0f, 1.0f,  // Haut gauche
             0.5f, -0.5f, -0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), 1.0f, 1.0f,  // Haut droite

            -0.5f, -0.5f, -0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), 0.0f, 1.0f,  // Haut gauche
             0.5f, -0.5f,  0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), 1.0f, 0.0f,  // Bas droite
            -0.5f, -0.5f,  0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), 0.0f, 0.0f,  // Bas gauche
            

            // Face haut
            -0.5f,  0.5f, -0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), 0.0f, 1.0f,  // Haut gauche
             0.5f,  0.5f,  0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), 1.0f, 0.0f,  // Bas droite
             0.5f,  0.5f, -0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), 1.0f, 1.0f,  // Haut droite

             0.5f,  0.5f,  0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), 1.0f, 0.0f,  // Bas droite
            -0.5f,  0.5f, -0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), 0.0f, 1.0f,   // Haut gauche
            -0.5f,  0.5f,  0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), 0.0f, 0.0f  // Bas gauche
            
        };

        memory_chunk chunk(vPos, sizeof(vPos));

        vboManager->bind(drawableComponent->vbo);
        vaoManager->bind(drawableComponent->vao);
        vaoManager->attach_vbo(drawableComponent->vao, drawableComponent->vbo);

        vboManager->transmit_data(chunk);
        vboManager->set_vertices_number(36);

        vboManager->add_attribute(0, GL3::gl_attrib_components_number::x3, GL3::gl_type::Float, 9 * sizeof(float), 0);
        vboManager->add_attribute(1, GL3::gl_attrib_components_number::x4, GL3::gl_type::Float, 9 * sizeof(float), 3 * sizeof(float));
        vboManager->add_attribute(2, GL3::gl_attrib_components_number::x2, GL3::gl_type::Float, 9 * sizeof(float), 7 * sizeof(float));

        entity_manager *entityManager = entity_manager::get_singleton();

        entity_manager::entity entity = entityManager->create_entity(name, collectionID);
        entityManager->attach_component(name, collectionID, drawableComponentID);
        entityManager->attach_component(name, collectionID, transformationComponentID);

        if(collidable)
        {
            component_id colliderComponentID = componentManager->create_collider_component();

            collider_component *colliderComponent = componentManager->get_collider_component(colliderComponentID);
            colliderComponent->contour.pos.x = position.x - width / 2;
            colliderComponent->contour.pos.y = position.y - height / 2;
            colliderComponent->contour.w = width;
            colliderComponent->contour.h = height;

            entityManager->attach_component(name, collectionID, colliderComponentID);
        }

        return entity;
    }

    /*
    ====================================
    graphic::create_3D_rectangle_texture
    ====================================
    */
    entity_manager::entity graphic::create_3D_rectangle_texture(
        const char *name,
        GL3::gl_id program,
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
        GL3::gl_id texture,
        uint8_t textureUnit,
        bool collidable)
    {
        component_manager *componentManager = component_manager::get_singleton();
        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();

        component_id drawableComponentID       = componentManager->create_drawable_component(program, vboManager->create(name), vaoManager->create(name), texture, textureUnit);
        component_id transformationComponentID = componentManager->create_transformation_component(position, fvec3(width, height, length), 0.0f);

        drawable_component *drawableComponent = componentManager->get_drawable_component(drawableComponentID);
        drawableComponent->renderCallback = drawable_component::classic_render_callback;

        float vPos[] =
        {

            // Face avant
            -0.5f, -0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), 1.0f, 1.0f,  // Haut gauche
             0.5f,  0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), 0.0f, 0.0f,  // Bas droite
             0.5f, -0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), 0.0f, 1.0f,  // Haut droite
             
            -0.5f, -0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), 1.0f, 1.0f,  // Haut gauche
            -0.5f,  0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), 1.0f, 0.0f,  // Bas gauche
             0.5f,  0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), 0.0f, 0.0f,  // Bas droite
            
            // Face arrière
             0.5f, -0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), 1.0f, 1.0f,  // Haut gauche
            -0.5f,  0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), 0.0f, 0.0f,  // Bas droite
            -0.5f, -0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), 0.0f, 1.0f,  // Haut droite

             0.5f, -0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), 1.0f, 1.0f,  // Haut gauche
             0.5f,  0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), 1.0f, 0.0f,  // Bas gauche
            -0.5f,  0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), 0.0f, 0.0f,  // Bas droite
            

            // Face droite
            -0.5f,  0.5f,  0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), 1.0f, 0.0f,  // Bas gauche
            -0.5f,  0.5f, -0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), 0.0f, 0.0f,  // Bas droite
            -0.5f, -0.5f, -0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), 0.0f, 1.0f,  // Haut droite

            -0.5f, -0.5f, -0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), 0.0f, 1.0f,  // Haut droite
            -0.5f, -0.5f,  0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), 1.0f, 1.0f,  // Haut gauche
            -0.5f,  0.5f,  0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), 1.0f, 0.0f,  // Bas gauche


            // Face gauche
             0.5f,  0.5f,  0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), 0.0f, 0.0f,  // Bas droite
             0.5f, -0.5f, -0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), 1.0f, 1.0f,  // Haut gauche
             0.5f,  0.5f, -0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), 1.0f, 0.0f,  // Bas gauche

             0.5f, -0.5f, -0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), 1.0f, 1.0f,  // Haut gauche
             0.5f,  0.5f,  0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), 0.0f, 0.0f,  // Bas droite
             0.5f, -0.5f,  0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), 0.0f, 1.0f,  // Haut droite


            // Face bas
             0.5f, -0.5f,  0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), 1.0f, 0.0f,  // Bas droite
            -0.5f, -0.5f, -0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), 0.0f, 1.0f,  // Haut gauche
             0.5f, -0.5f, -0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), 1.0f, 1.0f,  // Haut droite

            -0.5f, -0.5f, -0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), 0.0f, 1.0f,  // Haut gauche
             0.5f, -0.5f,  0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), 1.0f, 0.0f,  // Bas droite
            -0.5f, -0.5f,  0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), 0.0f, 0.0f,  // Bas gauche
            

            // Face haut
            -0.5f,  0.5f, -0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), 1.0f, 1.0f,  // Haut gauche
             0.5f,  0.5f,  0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), 0.0f, 0.0f,  // Bas droite
             0.5f,  0.5f, -0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), 0.0f, 1.0f,  // Haut droite

             0.5f,  0.5f,  0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), 0.0f, 0.0f,  // Bas droite
            -0.5f,  0.5f, -0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), 1.0f, 1.0f,  // Haut gauche
            -0.5f,  0.5f,  0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), 1.0f, 0.0f   // Bas gauche
            
        };

        memory_chunk chunk(vPos, sizeof(vPos));

        vboManager->bind(drawableComponent->vbo);
        vaoManager->bind(drawableComponent->vao);
        vaoManager->attach_vbo(drawableComponent->vao, drawableComponent->vbo);

        vboManager->transmit_data(chunk);
        vboManager->set_vertices_number(36);

        vboManager->add_attribute(0, GL3::gl_attrib_components_number::x3, GL3::gl_type::Float, 9 * sizeof(float), 0);
        vboManager->add_attribute(1, GL3::gl_attrib_components_number::x4, GL3::gl_type::Float, 9 * sizeof(float), 3 * sizeof(float));
        vboManager->add_attribute(2, GL3::gl_attrib_components_number::x2, GL3::gl_type::Float, 9 * sizeof(float), 7 * sizeof(float));

        entity_manager *entityManager = entity_manager::get_singleton();

        entity_manager::entity entity = entityManager->create_entity(name, collectionID);
        entityManager->attach_component(name, collectionID, drawableComponentID);
        entityManager->attach_component(name, collectionID, transformationComponentID);

        if(collidable)
        {
            component_id colliderComponentID = componentManager->create_collider_component();

            collider_component *colliderComponent = componentManager->get_collider_component(colliderComponentID);
            colliderComponent->contour.pos.x = position.x - width / 2;
            colliderComponent->contour.pos.y = position.y - height / 2;
            colliderComponent->contour.w = width;
            colliderComponent->contour.h = height;

            entityManager->attach_component(name, collectionID, colliderComponentID);
        }

        return entity;
    }

    polygon graphic::create_cube(
        const char *name,
        const colora &color1,
        const colora &color2,
        const colora &color3,
        const colora &color4,
        const colora &color5,
        const colora &color6,
        const cube_texture_points &texturePoints
    )
    {
        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();

        float vPos[] =
        {

            // Face avant
            -0.5f, -0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), texturePoints.frontBottomLeft.x, texturePoints.frontBottomLeft.y,      // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), texturePoints.frontTopRight.x, texturePoints.frontTopRight.y,          // 0.0f, 0.0f,  // Haut droite
             0.5f, -0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), texturePoints.frontBottomRight.x, texturePoints.frontBottomRight.y,    // 0.0f, 1.0f,  // Bas droite
             
            -0.5f, -0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), texturePoints.frontBottomLeft.x, texturePoints.frontBottomLeft.y,      // 1.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), texturePoints.frontTopLeft.x, texturePoints.frontTopLeft.y,            // 1.0f, 0.0f,  // Haut gauche
             0.5f,  0.5f, -0.5f, static_cast<float>(color1.R), static_cast<float>(color1.G), static_cast<float>(color1.B), static_cast<float>(color1.A), texturePoints.frontTopRight.x, texturePoints.frontTopRight.y,          // 0.0f, 0.0f,  // Haut droite
            

            // Face arrière
             0.5f, -0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), texturePoints.backBottomLeft.x, texturePoints.backBottomLeft.y,        // 1.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), texturePoints.backTopRight.x, texturePoints.backTopRight.y,            // 0.0f, 0.0f,  // Haut droite
            -0.5f, -0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), texturePoints.backBottomRight.x, texturePoints.backBottomRight.y,      // 0.0f, 1.0f,  // Bas droite

             0.5f, -0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), texturePoints.backBottomLeft.x, texturePoints.backBottomLeft.y,        // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), texturePoints.backTopLeft.x, texturePoints.backTopLeft.y,              // 1.0f, 0.0f,  // Haut gauche
            -0.5f,  0.5f,  0.5f, static_cast<float>(color2.R), static_cast<float>(color2.G), static_cast<float>(color2.B), static_cast<float>(color2.A), texturePoints.backTopRight.x, texturePoints.backTopRight.y,            // 0.0f, 0.0f,  // Haut droite
            

            // Face droite
            -0.5f,  0.5f,  0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), texturePoints.rightTopLeft.x, texturePoints.rightTopLeft.y,            // 1.0f, 0.0f,  // Haut gauche
            -0.5f,  0.5f, -0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), texturePoints.rightTopRight.x, texturePoints.rightTopRight.y,          // 0.0f, 0.0f,  // Haut droite
            -0.5f, -0.5f, -0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), texturePoints.rightBottomRight.x, texturePoints.rightBottomRight.y,    // 0.0f, 1.0f,  // Bas droite

            -0.5f, -0.5f, -0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), texturePoints.rightBottomRight.x, texturePoints.rightBottomRight.y,    // 0.0f, 1.0f,  // Bas droite
            -0.5f, -0.5f,  0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), texturePoints.rightBottomLeft.x, texturePoints.rightBottomLeft.y,      // 1.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f,  0.5f, static_cast<float>(color3.R), static_cast<float>(color3.G), static_cast<float>(color3.B), static_cast<float>(color3.A), texturePoints.rightTopLeft.x, texturePoints.rightTopLeft.y,            // 1.0f, 0.0f,  // Haut gauche


            // Face gauche
             0.5f,  0.5f,  0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), texturePoints.leftTopRight.x, texturePoints.leftTopRight.y,            // 0.0f, 0.0f,  // Haut droite
             0.5f, -0.5f, -0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), texturePoints.leftBottomLeft.x, texturePoints.leftBottomLeft.y,        // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f, -0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), texturePoints.leftTopLeft.x, texturePoints.leftTopLeft.y,              // 1.0f, 0.0f,  // Haut gauche

             0.5f, -0.5f, -0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), texturePoints.leftBottomLeft.x, texturePoints.leftBottomLeft.y,        // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f,  0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), texturePoints.leftTopRight.x, texturePoints.leftTopRight.y,            // 0.0f, 0.0f,  // Haut droite
             0.5f, -0.5f,  0.5f, static_cast<float>(color4.R), static_cast<float>(color4.G), static_cast<float>(color4.B), static_cast<float>(color4.A), texturePoints.leftBottomRight.x, texturePoints.leftBottomRight.y,      // 0.0f, 1.0f,  // Bas droite


            // Face bas
             0.5f, -0.5f,  0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), texturePoints.bottomTopRight.x, texturePoints.bottomTopRight.y,        // 1.0f, 0.0f,  // Haut droite
            -0.5f, -0.5f, -0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), texturePoints.bottomBottomLeft.x, texturePoints.bottomBottomLeft.y,    // 0.0f, 1.0f,  // Bas gauche
             0.5f, -0.5f, -0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), texturePoints.bottomBottomRight.x, texturePoints.bottomBottomRight.y,  // 1.0f, 1.0f,  // Bas droite

            -0.5f, -0.5f, -0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), texturePoints.bottomBottomLeft.x, texturePoints.bottomBottomLeft.y,    // 0.0f, 1.0f,  // Bas gauche
             0.5f, -0.5f,  0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), texturePoints.bottomTopRight.x, texturePoints.bottomTopRight.y,        // 1.0f, 0.0f,  // Haut droite
            -0.5f, -0.5f,  0.5f, static_cast<float>(color5.R), static_cast<float>(color5.G), static_cast<float>(color5.B), static_cast<float>(color5.A), texturePoints.bottomTopLeft.x, texturePoints.bottomTopLeft.y,          // 0.0f, 0.0f,  // Top gauche
            

            // Face haut
            -0.5f,  0.5f, -0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), texturePoints.topBottomLeft.x, texturePoints.topBottomLeft.y,          // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f,  0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), texturePoints.topTopRight.x, texturePoints.topTopRight.y,              // 0.0f, 0.0f,  // Haut droite
             0.5f,  0.5f, -0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), texturePoints.topBottomRight.x, texturePoints.topBottomRight.y,        // 0.0f, 1.0f,  // Bas droite

             0.5f,  0.5f,  0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), texturePoints.topTopRight.x, texturePoints.topTopRight.y,              // 0.0f, 0.0f,  // Haut droite
            -0.5f,  0.5f, -0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), texturePoints.topBottomLeft.x, texturePoints.topBottomLeft.y,          // 1.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f,  0.5f, static_cast<float>(color6.R), static_cast<float>(color6.G), static_cast<float>(color6.B), static_cast<float>(color6.A), texturePoints.topTopLeft.x, texturePoints.topTopLeft.y                 // 1.0f, 0.0f   // Haut gauche
            
        };

        polygon pol(vboManager->create(name), vaoManager->create(name));
        memory_chunk chunk(vPos, sizeof(vPos));

        vboManager->bind(pol.vbo());
        vaoManager->bind(pol.vao());
        vaoManager->attach_vbo(pol.vao(), pol.vbo());

        vboManager->transmit_data(chunk);
        vboManager->set_vertices_number(36);

        vboManager->add_attribute(0, GL3::gl_attrib_components_number::x3, GL3::gl_type::Float, 9 * sizeof(float), 0);
        vboManager->add_attribute(1, GL3::gl_attrib_components_number::x4, GL3::gl_type::Float, 9 * sizeof(float), 3 * sizeof(float));
        vboManager->add_attribute(2, GL3::gl_attrib_components_number::x2, GL3::gl_type::Float, 9 * sizeof(float), 7 * sizeof(float));

        vaoManager->bind_default();

        return pol;
    }

    polygon graphic::create_cube(const char *name)
    {
        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();

        float vPos[] =
        {

            // Face avant
            -0.5f, -0.5f, -0.5f, // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f, -0.5f, // 0.0f, 0.0f,  // Haut droite
             0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,  // Bas droite
             
            -0.5f, -0.5f, -0.5f, // 1.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f, -0.5f, // 1.0f, 0.0f,  // Haut gauche
             0.5f,  0.5f, -0.5f, // 0.0f, 0.0f,  // Haut droite
            

            // Face arrière
             0.5f, -0.5f,  0.5f, // 1.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f,  0.5f, // 0.0f, 0.0f,  // Haut droite
            -0.5f, -0.5f,  0.5f, // 0.0f, 1.0f,  // Bas droite

             0.5f, -0.5f,  0.5f, // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,  // Haut gauche
            -0.5f,  0.5f,  0.5f, // 0.0f, 0.0f,  // Haut droite
            

            // Face droite
            -0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,  // Haut gauche
            -0.5f,  0.5f, -0.5f, // 0.0f, 0.0f,  // Haut droite
            -0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,  // Bas droite

            -0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,  // Bas droite
            -0.5f, -0.5f,  0.5f, // 1.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,  // Haut gauche


            // Face gauche
             0.5f,  0.5f,  0.5f, // 0.0f, 0.0f,  // Haut droite
             0.5f, -0.5f, -0.5f, // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f, -0.5f, // 1.0f, 0.0f,  // Haut gauche

             0.5f, -0.5f, -0.5f, // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f,  0.5f, // 0.0f, 0.0f,  // Haut droite
             0.5f, -0.5f,  0.5f, // 0.0f, 1.0f,  // Bas droite


            // Face bas
             0.5f, -0.5f,  0.5f, // 1.0f, 0.0f,  // Haut droite
            -0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,  // Bas gauche
             0.5f, -0.5f, -0.5f, // 1.0f, 1.0f,  // Bas droite

            -0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,  // Bas gauche
             0.5f, -0.5f,  0.5f, // 1.0f, 0.0f,  // Haut droite
            -0.5f, -0.5f,  0.5f, // 0.0f, 0.0f,  // Top gauche
            

            // Face haut
            -0.5f,  0.5f, -0.5f, // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f,  0.5f, // 0.0f, 0.0f,  // Haut droite
             0.5f,  0.5f, -0.5f, // 0.0f, 1.0f,  // Bas droite

             0.5f,  0.5f,  0.5f, // 0.0f, 0.0f,  // Haut droite
            -0.5f,  0.5f, -0.5f, // 1.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f,  0.5f // 1.0f, 0.0f   // Haut gauche
            
        };

        polygon pol(vboManager->create(name), vaoManager->create(name));
        memory_chunk chunk(vPos, sizeof(vPos));

        vboManager->bind(pol.vbo());
        vaoManager->bind(pol.vao());
        vaoManager->attach_vbo(pol.vao(), pol.vbo());

        vboManager->transmit_data(chunk);
        vboManager->set_vertices_number(36);

        vboManager->add_attribute(0, GL3::gl_attrib_components_number::x3, GL3::gl_type::Float, 3 * sizeof(float), 0);

        vaoManager->bind_default();

        return pol;
    }

    entity_manager::entity graphic::create_cubemap(
        const char *name,
        GL3::gl_id program,
        entity_collection_id collectionID,
        const fvec3 &position,
        float width,
        float height,
        float length,
        GL3::gl_id texture,
        uint8_t textureUnit,
        bool collidable
    )
    {
        component_manager *componentManager = component_manager::get_singleton();
        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();

        component_id drawableComponentID       = componentManager->create_drawable_component(program, vboManager->create(name), vaoManager->create(name), texture, textureUnit);
        component_id transformationComponentID = componentManager->create_transformation_component(position, fvec3(width, height, length), 0.0f);

        drawable_component *drawableComponent = componentManager->get_drawable_component(drawableComponentID);
        drawableComponent->renderCallback = drawable_component::skybox_render_callback;

        float vPos[] =
        {
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

        vboManager->bind(drawableComponent->vbo);
        vaoManager->bind(drawableComponent->vao);
        vaoManager->attach_vbo(drawableComponent->vao, drawableComponent->vbo);

        vboManager->transmit_data(chunk);
        vboManager->set_vertices_number(36);

        vboManager->add_attribute(0, GL3::gl_attrib_components_number::x3, GL3::gl_type::Float, 3 * sizeof(float), 0);

        entity_manager *entityManager = entity_manager::get_singleton();

        entity_manager::entity entity = entityManager->create_entity(name, collectionID);
        entityManager->attach_component(name, collectionID, drawableComponentID);
        entityManager->attach_component(name, collectionID, transformationComponentID);

        if(collidable)
        {
            component_id colliderComponentID = componentManager->create_collider_component();

            collider_component *colliderComponent = componentManager->get_collider_component(colliderComponentID);
            colliderComponent->contour.pos.x = position.x - width / 2;
            colliderComponent->contour.pos.y = position.y - height / 2;
            colliderComponent->contour.w = width;
            colliderComponent->contour.h = height;

            entityManager->attach_component(name, collectionID, colliderComponentID);
        }

        return entity;
    }

}
