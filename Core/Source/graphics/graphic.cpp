#include "DE/graphics/graphic.hpp"
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
    ====================================
    graphic::create_3D_rectangle_texture
    ====================================
    */
    entity_manager::entity graphic::create_3D_rectangle_texture(
        const char *name,
        entity_collection_id collectionID,
        const vec3<float> &position,
        float width,
        float height,
        float length,
        const colora &color1,
        const colora &color2,
        const colora &color3,
        const colora &color4,
        const colora &color5,
        const colora &color6,
        bool /*collidable*/)
    {
        component_manager *componentManager = component_manager::get_singleton();
        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();

        component_id drawableComponentID       = componentManager->create_drawable_component(vboManager->create(name), vaoManager->create(name), nullptr);
        component_id transformationComponentID = componentManager->create_transformation_component(position, vec3<float>(width, height, length), 0.0f);

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

        // TODO: refaire ça
        /*if(collidable)
        {
            component_id colliderComponentID = componentManager->create_collider_component();

            collider_component *colliderComponent = componentManager->get_collider_component(colliderComponentID);
            colliderComponent->contour.pos.x = position.x - width / 2;
            colliderComponent->contour.pos.y = position.y - height / 2;
            colliderComponent->contour.w = width;
            colliderComponent->contour.h = height;

            entityManager->attach_component(name, collectionID, colliderComponentID);
        }*/

        return entity;
    }

    polygon graphic::create_cube(
        const char *name,
        const vec4<float> &color1,
        const vec4<float> &color2,
        const vec4<float> &color3,
        const vec4<float> &color4,
        const vec4<float> &color5,
        const vec4<float> &color6,
        const cube_texture_points &texturePoints
    )
    {
        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();

        float vPos[] =
        {

            // Face avant        // Normales
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, static_cast<float>(color1.x), static_cast<float>(color1.y), static_cast<float>(color1.z), static_cast<float>(color1.w), texturePoints.frontBottomLeft.x, texturePoints.frontBottomLeft.y,      // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, static_cast<float>(color1.x), static_cast<float>(color1.y), static_cast<float>(color1.z), static_cast<float>(color1.w), texturePoints.frontTopRight.x, texturePoints.frontTopRight.y,          // 0.0f, 0.0f,  // Haut droite
             0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, static_cast<float>(color1.x), static_cast<float>(color1.y), static_cast<float>(color1.z), static_cast<float>(color1.w), texturePoints.frontBottomRight.x, texturePoints.frontBottomRight.y,    // 0.0f, 1.0f,  // Bas droite
                                 
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, static_cast<float>(color1.x), static_cast<float>(color1.y), static_cast<float>(color1.z), static_cast<float>(color1.w), texturePoints.frontBottomLeft.x, texturePoints.frontBottomLeft.y,      // 1.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, static_cast<float>(color1.x), static_cast<float>(color1.y), static_cast<float>(color1.z), static_cast<float>(color1.w), texturePoints.frontTopLeft.x, texturePoints.frontTopLeft.y,            // 1.0f, 0.0f,  // Haut gauche
             0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, static_cast<float>(color1.x), static_cast<float>(color1.y), static_cast<float>(color1.z), static_cast<float>(color1.w), texturePoints.frontTopRight.x, texturePoints.frontTopRight.y,          // 0.0f, 0.0f,  // Haut droite
                                 
                                 
            // Face arrière      
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, static_cast<float>(color2.x), static_cast<float>(color2.y), static_cast<float>(color2.z), static_cast<float>(color2.w), texturePoints.backBottomLeft.x, texturePoints.backBottomLeft.y,        // 1.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, static_cast<float>(color2.x), static_cast<float>(color2.y), static_cast<float>(color2.z), static_cast<float>(color2.w), texturePoints.backTopRight.x, texturePoints.backTopRight.y,            // 0.0f, 0.0f,  // Haut droite
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, static_cast<float>(color2.x), static_cast<float>(color2.y), static_cast<float>(color2.z), static_cast<float>(color2.w), texturePoints.backBottomRight.x, texturePoints.backBottomRight.y,      // 0.0f, 1.0f,  // Bas droite
                                 
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, static_cast<float>(color2.x), static_cast<float>(color2.y), static_cast<float>(color2.z), static_cast<float>(color2.w), texturePoints.backBottomLeft.x, texturePoints.backBottomLeft.y,        // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, static_cast<float>(color2.x), static_cast<float>(color2.y), static_cast<float>(color2.z), static_cast<float>(color2.w), texturePoints.backTopLeft.x, texturePoints.backTopLeft.y,              // 1.0f, 0.0f,  // Haut gauche
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, static_cast<float>(color2.x), static_cast<float>(color2.y), static_cast<float>(color2.z), static_cast<float>(color2.w), texturePoints.backTopRight.x, texturePoints.backTopRight.y,            // 0.0f, 0.0f,  // Haut droite
                                 
                                 
            // Face droite       
            -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, static_cast<float>(color3.x), static_cast<float>(color3.y), static_cast<float>(color3.z), static_cast<float>(color3.w), texturePoints.rightTopLeft.x, texturePoints.rightTopLeft.y,            // 1.0f, 0.0f,  // Haut gauche
            -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, static_cast<float>(color3.x), static_cast<float>(color3.y), static_cast<float>(color3.z), static_cast<float>(color3.w), texturePoints.rightTopRight.x, texturePoints.rightTopRight.y,          // 0.0f, 0.0f,  // Haut droite
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, static_cast<float>(color3.x), static_cast<float>(color3.y), static_cast<float>(color3.z), static_cast<float>(color3.w), texturePoints.rightBottomRight.x, texturePoints.rightBottomRight.y,    // 0.0f, 1.0f,  // Bas droite
                                 
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0, static_cast<float>(color3.x), static_cast<float>(color3.y), static_cast<float>(color3.z), static_cast<float>(color3.w), texturePoints.rightBottomRight.x, texturePoints.rightBottomRight.y,    // 0.0f, 1.0f,  // Bas droite
            -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0, static_cast<float>(color3.x), static_cast<float>(color3.y), static_cast<float>(color3.z), static_cast<float>(color3.w), texturePoints.rightBottomLeft.x, texturePoints.rightBottomLeft.y,      // 1.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0, static_cast<float>(color3.x), static_cast<float>(color3.y), static_cast<float>(color3.z), static_cast<float>(color3.w), texturePoints.rightTopLeft.x, texturePoints.rightTopLeft.y,            // 1.0f, 0.0f,  // Haut gauche
                                 
                                 
            // Face gauche       
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, static_cast<float>(color4.x), static_cast<float>(color4.y), static_cast<float>(color4.z), static_cast<float>(color4.w), texturePoints.leftTopRight.x, texturePoints.leftTopRight.y,            // 0.0f, 0.0f,  // Haut droite
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, static_cast<float>(color4.x), static_cast<float>(color4.y), static_cast<float>(color4.z), static_cast<float>(color4.w), texturePoints.leftBottomLeft.x, texturePoints.leftBottomLeft.y,        // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, static_cast<float>(color4.x), static_cast<float>(color4.y), static_cast<float>(color4.z), static_cast<float>(color4.w), texturePoints.leftTopLeft.x, texturePoints.leftTopLeft.y,              // 1.0f, 0.0f,  // Haut gauche
                                 
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0,  static_cast<float>(color4.x), static_cast<float>(color4.y), static_cast<float>(color4.z), static_cast<float>(color4.w), texturePoints.leftBottomLeft.x, texturePoints.leftBottomLeft.y,        // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0,  static_cast<float>(color4.x), static_cast<float>(color4.y), static_cast<float>(color4.z), static_cast<float>(color4.w), texturePoints.leftTopRight.x, texturePoints.leftTopRight.y,            // 0.0f, 0.0f,  // Haut droite
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0,  static_cast<float>(color4.x), static_cast<float>(color4.y), static_cast<float>(color4.z), static_cast<float>(color4.w), texturePoints.leftBottomRight.x, texturePoints.leftBottomRight.y,      // 0.0f, 1.0f,  // Bas droite
                                 
                                 
            // Face bas          
             0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, static_cast<float>(color5.x), static_cast<float>(color5.y), static_cast<float>(color5.z), static_cast<float>(color5.w), texturePoints.bottomTopRight.x, texturePoints.bottomTopRight.y,        // 1.0f, 0.0f,  // Haut droite
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, static_cast<float>(color5.x), static_cast<float>(color5.y), static_cast<float>(color5.z), static_cast<float>(color5.w), texturePoints.bottomBottomLeft.x, texturePoints.bottomBottomLeft.y,    // 0.0f, 1.0f,  // Bas gauche
             0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, static_cast<float>(color5.x), static_cast<float>(color5.y), static_cast<float>(color5.z), static_cast<float>(color5.w), texturePoints.bottomBottomRight.x, texturePoints.bottomBottomRight.y,  // 1.0f, 1.0f,  // Bas droite
                                 
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, static_cast<float>(color5.x), static_cast<float>(color5.y), static_cast<float>(color5.z), static_cast<float>(color5.w), texturePoints.bottomBottomLeft.x, texturePoints.bottomBottomLeft.y,    // 0.0f, 1.0f,  // Bas gauche
             0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, static_cast<float>(color5.x), static_cast<float>(color5.y), static_cast<float>(color5.z), static_cast<float>(color5.w), texturePoints.bottomTopRight.x, texturePoints.bottomTopRight.y,        // 1.0f, 0.0f,  // Haut droite
            -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, static_cast<float>(color5.x), static_cast<float>(color5.y), static_cast<float>(color5.z), static_cast<float>(color5.w), texturePoints.bottomTopLeft.x, texturePoints.bottomTopLeft.y,          // 0.0f, 0.0f,  // Top gauche
                                 
                                 
            // Face haut         
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, static_cast<float>(color6.x), static_cast<float>(color6.y), static_cast<float>(color6.z), static_cast<float>(color6.w), texturePoints.topBottomLeft.x, texturePoints.topBottomLeft.y,          // 1.0f, 1.0f,  // Bas gauche
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, static_cast<float>(color6.x), static_cast<float>(color6.y), static_cast<float>(color6.z), static_cast<float>(color6.w), texturePoints.topTopRight.x, texturePoints.topTopRight.y,              // 0.0f, 0.0f,  // Haut droite
             0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, static_cast<float>(color6.x), static_cast<float>(color6.y), static_cast<float>(color6.z), static_cast<float>(color6.w), texturePoints.topBottomRight.x, texturePoints.topBottomRight.y,        // 0.0f, 1.0f,  // Bas droite
                                 
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, static_cast<float>(color6.x), static_cast<float>(color6.y), static_cast<float>(color6.z), static_cast<float>(color6.w), texturePoints.topTopRight.x, texturePoints.topTopRight.y,              // 0.0f, 0.0f,  // Haut droite
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, static_cast<float>(color6.x), static_cast<float>(color6.y), static_cast<float>(color6.z), static_cast<float>(color6.w), texturePoints.topBottomLeft.x, texturePoints.topBottomLeft.y,          // 1.0f, 1.0f,  // Bas gauche
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, static_cast<float>(color6.x), static_cast<float>(color6.y), static_cast<float>(color6.z), static_cast<float>(color6.w), texturePoints.topTopLeft.x, texturePoints.topTopLeft.y                 // 1.0f, 0.0f   // Haut gauche
            
        };

        polygon pol(vboManager->create(name), vaoManager->create(name));
        memory_chunk chunk(vPos, sizeof(vPos));

        vboManager->bind(pol.vbo());
        vaoManager->bind(pol.vao());
        vaoManager->attach_vbo(pol.vao(), pol.vbo());

        vboManager->transmit_data(chunk);
        vboManager->set_vertices_number(36);

        vboManager->add_attribute(0, GL3::gl_attrib_components_number::x3, GL3::gl_type::Float, 12 * sizeof(float), 0);
        vboManager->add_attribute(1, GL3::gl_attrib_components_number::x3, GL3::gl_type::Float, 12 * sizeof(float), 3 * sizeof(float));
        vboManager->add_attribute(2, GL3::gl_attrib_components_number::x4, GL3::gl_type::Float, 12 * sizeof(float), 6 * sizeof(float));
        vboManager->add_attribute(3, GL3::gl_attrib_components_number::x2, GL3::gl_type::Float, 12 * sizeof(float), 10 * sizeof(float));

        vaoManager->bind_default();

        return pol;
    }

    polygon graphic::create_cube(
        const char *name,
        const deep::vec4<float> &color1,
        const deep::vec4<float> &color2,
        const deep::vec4<float> &color3,
        const deep::vec4<float> &color4,
        const deep::vec4<float> &color5,
        const deep::vec4<float> &color6
    )
    {
        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();

        float vPos[] =
        {

            // Face avant        // Normales
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, static_cast<float>(color1.x), static_cast<float>(color1.y), static_cast<float>(color1.z), static_cast<float>(color1.w),  // Bas gauche
             0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, static_cast<float>(color1.x), static_cast<float>(color1.y), static_cast<float>(color1.z), static_cast<float>(color1.w),  // Haut droite
             0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, static_cast<float>(color1.x), static_cast<float>(color1.y), static_cast<float>(color1.z), static_cast<float>(color1.w),  // Bas droite
                                 
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, static_cast<float>(color1.x), static_cast<float>(color1.y), static_cast<float>(color1.z), static_cast<float>(color1.w),  // Bas gauche
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, static_cast<float>(color1.x), static_cast<float>(color1.y), static_cast<float>(color1.z), static_cast<float>(color1.w),  // Haut gauche
             0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, static_cast<float>(color1.x), static_cast<float>(color1.y), static_cast<float>(color1.z), static_cast<float>(color1.w),  // Haut droite
                                 
                                 
            // Face arrière      
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, static_cast<float>(color2.x), static_cast<float>(color2.y), static_cast<float>(color2.z), static_cast<float>(color2.w),  // Bas gauche
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, static_cast<float>(color2.x), static_cast<float>(color2.y), static_cast<float>(color2.z), static_cast<float>(color2.w),  // Haut droite
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, static_cast<float>(color2.x), static_cast<float>(color2.y), static_cast<float>(color2.z), static_cast<float>(color2.w),  // Bas droite
                                 
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, static_cast<float>(color2.x), static_cast<float>(color2.y), static_cast<float>(color2.z), static_cast<float>(color2.w),  // Bas gauche
             0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, static_cast<float>(color2.x), static_cast<float>(color2.y), static_cast<float>(color2.z), static_cast<float>(color2.w),  // Haut gauche
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, static_cast<float>(color2.x), static_cast<float>(color2.y), static_cast<float>(color2.z), static_cast<float>(color2.w),  // Haut droite
                                 
                                 
            // Face droite       
            -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, static_cast<float>(color3.x), static_cast<float>(color3.y), static_cast<float>(color3.z), static_cast<float>(color3.w),  // Haut gauche
            -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, static_cast<float>(color3.x), static_cast<float>(color3.y), static_cast<float>(color3.z), static_cast<float>(color3.w),  // Haut droite
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, static_cast<float>(color3.x), static_cast<float>(color3.y), static_cast<float>(color3.z), static_cast<float>(color3.w),  // Bas droite
                                 
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0, static_cast<float>(color3.x), static_cast<float>(color3.y), static_cast<float>(color3.z), static_cast<float>(color3.w),  // Bas droite
            -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0, static_cast<float>(color3.x), static_cast<float>(color3.y), static_cast<float>(color3.z), static_cast<float>(color3.w),  // Bas gauche
            -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0, static_cast<float>(color3.x), static_cast<float>(color3.y), static_cast<float>(color3.z), static_cast<float>(color3.w),  // Haut gauche
                                 
                                 
            // Face gauche       
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, static_cast<float>(color4.x), static_cast<float>(color4.y), static_cast<float>(color4.z), static_cast<float>(color4.w),  // Haut droite
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, static_cast<float>(color4.x), static_cast<float>(color4.y), static_cast<float>(color4.z), static_cast<float>(color4.w),  // Bas gauche
             0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, static_cast<float>(color4.x), static_cast<float>(color4.y), static_cast<float>(color4.z), static_cast<float>(color4.w),  // Haut gauche
                                 
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0, static_cast<float>(color4.x), static_cast<float>(color4.y), static_cast<float>(color4.z), static_cast<float>(color4.w),  // Bas gauche
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0, static_cast<float>(color4.x), static_cast<float>(color4.y), static_cast<float>(color4.z), static_cast<float>(color4.w),  // Haut droite
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0, static_cast<float>(color4.x), static_cast<float>(color4.y), static_cast<float>(color4.z), static_cast<float>(color4.w),  // Bas droite
                                 
                                 
            // Face bas          
             0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, static_cast<float>(color5.x), static_cast<float>(color5.y), static_cast<float>(color5.z), static_cast<float>(color5.w),  // Haut droite
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, static_cast<float>(color5.x), static_cast<float>(color5.y), static_cast<float>(color5.z), static_cast<float>(color5.w),  // Bas gauche
             0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, static_cast<float>(color5.x), static_cast<float>(color5.y), static_cast<float>(color5.z), static_cast<float>(color5.w),  // Bas droite
                                 
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, static_cast<float>(color5.x), static_cast<float>(color5.y), static_cast<float>(color5.z), static_cast<float>(color5.w),  // Bas gauche
             0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, static_cast<float>(color5.x), static_cast<float>(color5.y), static_cast<float>(color5.z), static_cast<float>(color5.w),  // Haut droite
            -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, static_cast<float>(color5.x), static_cast<float>(color5.y), static_cast<float>(color5.z), static_cast<float>(color5.w),  // Top gauche
                                 
                                 
            // Face haut         
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, static_cast<float>(color6.x), static_cast<float>(color6.y), static_cast<float>(color6.z), static_cast<float>(color6.w),  // Bas gauche
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, static_cast<float>(color6.x), static_cast<float>(color6.y), static_cast<float>(color6.z), static_cast<float>(color6.w),  // Haut droite
             0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, static_cast<float>(color6.x), static_cast<float>(color6.y), static_cast<float>(color6.z), static_cast<float>(color6.w),  // Bas droite
                                 
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, static_cast<float>(color6.x), static_cast<float>(color6.y), static_cast<float>(color6.z), static_cast<float>(color6.w),  // Haut droite
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, static_cast<float>(color6.x), static_cast<float>(color6.y), static_cast<float>(color6.z), static_cast<float>(color6.w),  // Bas gauche
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, static_cast<float>(color6.x), static_cast<float>(color6.y), static_cast<float>(color6.z), static_cast<float>(color6.w)   // Haut gauche
            
        };

        polygon pol(vboManager->create(name), vaoManager->create(name));
        memory_chunk chunk(vPos, sizeof(vPos));

        vboManager->bind(pol.vbo());
        vaoManager->bind(pol.vao());
        vaoManager->attach_vbo(pol.vao(), pol.vbo());

        vboManager->transmit_data(chunk);
        vboManager->set_vertices_number(36);

        vboManager->add_attribute(0, GL3::gl_attrib_components_number::x3, GL3::gl_type::Float, 10 * sizeof(float), 0);
        vboManager->add_attribute(1, GL3::gl_attrib_components_number::x3, GL3::gl_type::Float, 10 * sizeof(float), 3 * sizeof(float));
        vboManager->add_attribute(2, GL3::gl_attrib_components_number::x4, GL3::gl_type::Float, 10 * sizeof(float), 6 * sizeof(float));

        vaoManager->bind_default();

        return pol;
    }

    entity_manager::entity graphic::create_cubemap(
        const char *name,
        entity_collection_id collectionID,
        const vec3<float> &position,
        float width,
        float height,
        float length,
        bool /*collidable*/
    )
    {
        component_manager *componentManager = component_manager::get_singleton();
        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();

        component_id drawableComponentID       = componentManager->create_drawable_component(vboManager->create(name), vaoManager->create(name), nullptr);
        component_id transformationComponentID = componentManager->create_transformation_component(position, vec3<float>(width, height, length), 0.0f);

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

        // TODO: refaire ça
        /*if(collidable)
        {
            component_id colliderComponentID = componentManager->create_collider_component();

            collider_component *colliderComponent = componentManager->get_collider_component(colliderComponentID);
            colliderComponent->contour.pos.x = position.x - width / 2;
            colliderComponent->contour.pos.y = position.y - height / 2;
            colliderComponent->contour.w = width;
            colliderComponent->contour.h = height;

            entityManager->attach_component(name, collectionID, colliderComponentID);
        }*/

        return entity;
    }

}
