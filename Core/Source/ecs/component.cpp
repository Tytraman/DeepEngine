#include "ecs/component.hpp"
#include "graphics/graphic.hpp"
#include "maths/mat.hpp"
#include "os/window.hpp"
#include "drivers/opengl/core.hpp"
#include "drivers/opengl/vbo.hpp"
#include "drivers/opengl/vao.hpp"
#include "drivers/opengl/shader.hpp"

#include <vector>

namespace deep
{

    void destroy_drawable_callback(mem_ptr ptr)
    {
        std::vector<float> test;

        test.emplace_back(32.0f);

        drawable_component *drawable = (drawable_component *) ptr;

        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();

        vaoManager->destroy(drawable->vao);
        vboManager->destroy(drawable->vbo);
    }

    /*
    ===========================================
    drawable_component::classic_render_callback
    ===========================================
    */
    void drawable_component::classic_render_callback(GL3::gl_renderer &renderer, drawable_component *drawable, transformation_component *transformation, window *window, camera *camera)
    {
        GL3::gl_id program;
        GL3::gl_id vao = drawable->vao;

        if(drawable->mat.get() != nullptr)
            program = drawable->mat->get_program();
        else
            program = 0;

        GL3::program_manager *programManager = GL3::program_manager::get_singleton();
        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();

        // Pas besoin d'indiquer à OpenGL d'utiliser un programme qui est déjà en cours d'utilisation.
        if(program != programManager->current_id())
            programManager->use(program);

        // Pas besoin d'indiquer à OpenGL d'utiliser un VAO qui est déjà en cours d'utilisation.
        if(vao != vaoManager->current_id())
            vaoManager->bind(vao);

        programManager->set_uniform("mTrs", transformation->get_translation());
        programManager->set_uniform("mRotX", transformation->get_rotation_X());
        programManager->set_uniform("mRotY", transformation->get_rotation_Y());
        programManager->set_uniform("mRotZ", transformation->get_rotation_Z());
        programManager->set_uniform("mScl", transformation->get_scaling());
        programManager->set_uniform("view", camera->get_look_at());
        programManager->set_uniform("deViewPos", camera->get_position());
        programManager->set_uniform("proj", mat4x4<float>::perspective(mat4x4<float>(), 45.0f, (float) window->get_width() / (float) window->get_height(), 0.1f, 1000.0f));
        
        if(drawable->mat.get() != nullptr)
            drawable->mat->send_data();
        else
            programManager->send_uniforms();

        renderer.draw(vboManager->get_vertices_number(drawable->vbo));
    }

    /*
    ==========================================
    drawable_component::skybox_render_callback
    ==========================================
    */
    void drawable_component::skybox_render_callback(GL3::gl_renderer &renderer, drawable_component *drawable, transformation_component *, window *window, camera *camera)
    {
        GL3::program_manager *programManager = GL3::program_manager::get_singleton();
        GL3::vbo_manager *vboManager = GL3::vbo_manager::get_singleton();
        GL3::vao_manager *vaoManager = GL3::vao_manager::get_singleton();

        GL3::core::disable_depth_mask();
        GL3::core::set_depth_function(GL3::core::gl_depth_function::Lequal);

        if(drawable->mat.get() != nullptr)
            programManager->use(drawable->mat->get_program());

        vaoManager->bind(drawable->vao);

        mat4x4 view = camera->get_look_at();
        view[static_cast<uint8_t>(mat4x4<float>::index::w1)] = 0.0f;
        view[static_cast<uint8_t>(mat4x4<float>::index::w2)] = 0.0f;
        view[static_cast<uint8_t>(mat4x4<float>::index::w3)] = 0.0f;

        programManager->set_uniform("skybox", 0);
        programManager->set_uniform("view", view);
        programManager->set_uniform("proj", mat4x4<float>::perspective(mat4x4<float>(), 45.0f, (float) window->get_width() / (float) window->get_height(), 0.1f, 1000.0f));

        if(drawable->mat.get() != nullptr)
            drawable->mat->send_data();
        else
            programManager->send_uniforms();

        renderer.draw(vboManager->get_vertices_number(drawable->vbo));
        GL3::core::set_depth_function(GL3::core::gl_depth_function::Less);
        GL3::core::enable_depth_mask();
    }

    /*
    ====================================
    component_manager::component_manager
    ====================================
    */
    component_manager::component_manager()
        : m_ComponentCount(0),
          m_ComponentsType(1000),
          m_DrawableComponents(1000),
          m_TransformationComponents(1000),
          m_VelocityComponents(1000),
          m_ColliderComponents(1000),
          m_AccelerationComponents(1000),
          m_HealthComponents(1000)
    { }

    /*
    ================================
    component_manager::get_singleton
    ================================
    */
    component_manager *component_manager::get_singleton()
    {
        static component_manager singleton;

        return &singleton;
    }

    /*
    ===========================
    component_manager::get_type
    ===========================
    */
    component_manager::component_type component_manager::get_type(component_id component)
    {
        const auto hs = m_ComponentsType[component];
        if(hs == nullptr)
            return component_manager::component_type::None;

        return hs->value;
    }

    /*
    ======================================
    drawable_component::drawable_component
    ======================================
    */
    drawable_component::drawable_component(GL3::gl_id _vbo, GL3::gl_id _vao, material *_material)
        : vbo(_vbo),
          vao(_vao),
          renderCallback(nullptr),
          mat(_material)
    { }

    /*
    ============================================
    component_manager::create_drawable_component
    ============================================
    */
    component_id component_manager::create_drawable_component(GL3::gl_id vbo, GL3::gl_id vao, material *material)
    {
        component_id id = m_ComponentCount;
        drawable_component drawable(vbo, vao, material);
        component_type type = component_manager::component_type::Drawable;

        m_DrawableComponents.insert(id, drawable);
        m_ComponentsType.insert(id, type);

        m_ComponentCount = id + 1;

        return id;
    }

    /*
    ============================================
    component_manager::create_drawable_component
    ============================================
    */
    component_id component_manager::create_drawable_component(const char *vboName, const char *vaoName, material *material)
    {
        GL3::program_manager *programManager = GL3::program_manager::get_singleton();

        hash_function hash = programManager->get_hash_function();

        GL3::gl_id vbo  = hash(vboName);
        GL3::gl_id vao  = hash(vaoName);

        return create_drawable_component(vbo, vao, material);
    }

    /*
    ==================================================
    transformation_component::transformation_component
    ==================================================
    */
    transformation_component::transformation_component(const vec3<float> &translation, const vec3<float> &scaling, float rotationX, float rotationY, float rotationZ)
        : m_Translation(translation),
          m_Scaling(scaling),
          m_RotationX(rotationX),
          m_RotationY(rotationY),
          m_RotationZ(rotationZ)
    { }

    /*
    ================================================
    component_manager::create_transformation_component
    ================================================
    */
    component_id component_manager::create_transformation_component(const vec3<float> &translation, const vec3<float> &scaling, float rotation)
    {
        component_id id = m_ComponentCount;
        transformation_component transformation(translation, scaling, rotation);
        component_type type = component_manager::component_type::Transformation;

        m_TransformationComponents.insert(id, transformation);
        m_ComponentsType.insert(id, type);

        m_ComponentCount = id + 1;

        return id;
    }

    /*
    ====================================
    velocity_component::velocity_component
    ====================================
    */
    velocity_component::velocity_component()
        : m_Velocity(0.0f, 0.0f, 0.0f)
    { }

    /*
    =========================================
    component_manager::create_velocity_component
    =========================================
    */
    component_id component_manager::create_velocity_component()
    {
        component_id id = m_ComponentCount;
        velocity_component velocity;
        component_type type = component_manager::component_type::Velocity;

        m_VelocityComponents.insert(id, velocity);
        m_ComponentsType.insert(id, type);

        m_ComponentCount = id + 1;

        return id;
    }

    /*
    ====================================
    collider_component::collider_component
    ====================================
    */
    collider_component::collider_component()
        : position(0.0f, 0.0f, 0.0f),
          size(0.0f, 0.0f, 0.0f)
    { }

    /*
    =========================================
    component_manager::create_collider_component
    =========================================
    */
    component_id component_manager::create_collider_component()
    {
        component_id id = m_ComponentCount;
        collider_component collider;
        component_type type = component_manager::component_type::Collider;

        m_ColliderComponents.insert(id, collider);
        m_ComponentsType.insert(id, type);

        m_ComponentCount = id + 1;

        return id;
    }

    /*
    ============================================
    acceleration_component::acceleration_component
    ============================================
    */
    acceleration_component::acceleration_component(const vec3<float> &_acceleration)
        : acceleration(_acceleration)
    { }

    /*
    =============================================
    component_manager::create_acceleration_component
    =============================================
    */
    component_id component_manager::create_acceleration_component(const vec3<float> &acceleration)
    {
        component_id id = m_ComponentCount;
        acceleration_component acc(acceleration);
        component_type type = component_manager::component_type::Acceleration;

        m_AccelerationComponents.insert(id, acc);
        m_ComponentsType.insert(id, type);

        m_ComponentCount = id + 1;

        return id;
    }

    /*
    ================================
    health_component::health_component
    ================================
    */
    health_component::health_component()
        : pv(0), maxPv(0)
    { }

    /*
    ================================
    health_component::health_component
    ================================
    */
    health_component::health_component(uint32_t __pv, uint32_t __maxPv)
        : pv(__pv), maxPv(__maxPv)
    { }

    /*
    =======================================
    component_manager::create_health_component
    =======================================
    */
    component_id component_manager::create_health_component(uint32_t pv, uint32_t max)
    {
        component_id id = m_ComponentCount;
        health_component health(pv, max);
        component_type type = component_manager::component_type::Health;

        m_HealthComponents.insert(id, health);
        m_ComponentsType.insert(id, type);

        m_ComponentCount = id + 1;

        return id;
    }

}

