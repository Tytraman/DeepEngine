#ifndef __DEEP_ENGINE_COMPONENT_HPP__
#define __DEEP_ENGINE_COMPONENT_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/ecs/ecs.hpp"
#include "DE/vec.hpp"
#include "DE/memory/list.hpp"
#include "DE/graphics/vertex.hpp"
#include "DE/graphics/shape.hpp"
#include "DE/graphics/material.hpp"
#include "DE/memory/hash_table.hpp"
#include "DE/memory/ref_counted.hpp"
#include "DE/drivers/opengl/renderer.hpp"

namespace deep
{

    class component_manager;
    struct drawable_component;
    struct transformation_component;
    struct velocity_component;
    struct collider_component;
    struct acceleration_component;
    struct health_component;
    struct material_component;

    class system_manager;
    class window;
    class camera;
    class gl_renderer;

    using drawable_render_callback = void (*)(GL3::gl_renderer &renderer, drawable_component *drawable, transformation_component *transformation, window *window, camera *camera);

    struct drawable_component
    {
        GL3::gl_id vbo;
        GL3::gl_id vao;
        ref<material> mat;
        drawable_render_callback renderCallback;

        DE_API static void classic_render_callback(GL3::gl_renderer &renderer, drawable_component *drawable, transformation_component *transformation, window *window, camera *camera);
        DE_API static void skybox_render_callback(GL3::gl_renderer &renderer, drawable_component *drawable, transformation_component *transformation, window *window, camera *camera);
        
        private:
            drawable_component(GL3::gl_id vbo, GL3::gl_id vao, material *mat = nullptr);

            friend component_manager;
    };

    struct transformation_component
    {
        
        public:
            DE_API vec3<float> get_translation() const;
            DE_API vec3<float> get_scaling() const;
            DE_API float get_rotation_X() const;
            DE_API float get_rotation_Y() const;
            DE_API float get_rotation_Z() const;

            DE_API void set_translation(const vec3<float> &translation);
            DE_API void set_scaling(const vec3<float> &scaling);
            DE_API void set_rotation_X(float rotation);
            DE_API void set_rotation_Y(float rotation);
            DE_API void set_rotation_Z(float rotation);

        private:
            vec3<float> m_Translation;
            vec3<float> m_Scaling;
            float m_RotationX;
            float m_RotationY;
            float m_RotationZ;

            transformation_component(const vec3<float> &translation, const vec3<float> &scaling, float rotationX = 0.0f, float rotationY = 0.0f, float rotationZ = 0.0f);

            friend component_manager;
            friend system_manager;
    };

    struct velocity_component
    {

        public:
            DE_API vec3<float> get_velocity() const;
            DE_API void set_velocity(const vec3<float> &vel);

        private:
            vec3<float> m_Velocity;

            velocity_component();

            friend component_manager;
    };

    struct collider_component
    {

        public:
            vec3<float> position;
            vec3<float> size;
        
        private:
            collider_component();

            friend component_manager;
    };

    struct acceleration_component
    {

        public:
            vec3<float> acceleration;

        private:
            acceleration_component(const vec3<float> &acceleration);

            friend component_manager;
    };

    struct health_component
    {

        public:
            uint32_t pv;
            uint32_t maxPv;

        private:
            health_component();
            health_component(uint32_t pv, uint32_t maxPv);

            friend component_manager;
    };

    class component_manager
    {

        public:
            enum class component_type : uint32_t
            {
                None           = 0,
                Drawable       = (1 << 0),
                Transformation = (1 << 1),
                Collider       = (1 << 2),
                Velocity       = (1 << 3),
                Acceleration   = (1 << 4),
                Health         = (1 << 5)
            };
            

        public:
            DE_API static component_manager *get_singleton();

            DE_API component_type get_type(component_id component);

            DE_API component_id create_drawable_component(GL3::gl_id vbo, GL3::gl_id vao, material *material = nullptr);
            DE_API component_id create_drawable_component(const char *vboName, const char *vaoName, material *material = nullptr);

            DE_API drawable_component *get_drawable_component(component_id component);
            DE_API void delete_drawable_component(component_id id);

            DE_API component_id create_transformation_component(const vec3<float> &translation, const vec3<float> &scaling = vec3<float>(1.0f, 1.0f, 1.0f), float rotation = 0.0f);
            DE_API transformation_component *get_transformation_component(component_id component);
            DE_API void delete_transformation_component(component_id id);

            DE_API component_id create_collider_component();
            DE_API collider_component *get_collider_component(component_id component);
            DE_API void delete_collider_component(component_id id);

            DE_API component_id create_velocity_component();
            DE_API velocity_component *get_velocity_component(component_id component);
            DE_API void delete_velocity_component(component_id id);

            DE_API component_id create_acceleration_component(const vec3<float> &acceleration);
            DE_API acceleration_component *get_acceleration_component(component_id component);
            DE_API void delete_acceleration_component(component_id component);
            
            DE_API component_id create_health_component(uint32_t pv, uint32_t max);
            DE_API health_component *get_health_component(component_id component);
            DE_API void delete_health_component(component_id id);

        private:
            component_manager();

            component_id m_ComponentCount;
            hash_table<component_type> m_ComponentsType;
            hash_table<drawable_component> m_DrawableComponents;
            hash_table<transformation_component> m_TransformationComponents;
            hash_table<velocity_component> m_VelocityComponents;
            hash_table<collider_component> m_ColliderComponents;
            hash_table<acceleration_component> m_AccelerationComponents;
            hash_table<health_component> m_HealthComponents;

        public:
            component_manager(const component_manager &) = delete;

    };

    /*
    =========================================
    component_manager::get_drawable_component
    =========================================
    */
    inline drawable_component *component_manager::get_drawable_component(component_id component)
    {
        return &m_DrawableComponents[component]->value;
    }

    /*
    ===============================================
    component_manager::get_transformation_component
    ===============================================
    */
    inline transformation_component *component_manager::get_transformation_component(component_id component)
    {
        return &m_TransformationComponents[component]->value;
    }

    /*
    =========================================
    component_manager::get_velocity_component
    =========================================
    */
    inline velocity_component *component_manager::get_velocity_component(component_id component)
    {
        return &m_VelocityComponents[component]->value;
    }

    /*
    =========================================
    component_manager::get_collider_component
    =========================================
    */
    inline collider_component *component_manager::get_collider_component(component_id component)
    {
        return &m_ColliderComponents[component]->value;
    }

    /*
    =============================================
    component_manager::get_acceleration_component
    =============================================
    */
    inline acceleration_component *component_manager::get_acceleration_component(component_id component)
    {
        return &m_AccelerationComponents[component]->value;
    }

    /*
    =======================================
    component_manager::get_health_component
    =======================================
    */
    inline health_component *component_manager::get_health_component(component_id component)
    {
        return &m_HealthComponents[component]->value;
    }

    /*
    ============================================
    component_manager::delete_drawable_component
    ============================================
    */
    inline void component_manager::delete_drawable_component(component_id component)
    {
        m_DrawableComponents.remove(component);
        m_ComponentsType.remove(component);
    }

    /*
    ==================================================
    component_manager::delete_transformation_component
    ==================================================
    */
    inline void component_manager::delete_transformation_component(component_id component)
    {
        m_TransformationComponents.remove(component);
        m_ComponentsType.remove(component);
    }

    /*
    ============================================
    component_manager::delete_velocity_component
    ============================================
    */
    inline void component_manager::delete_velocity_component(component_id component)
    {
        m_VelocityComponents.remove(component);
        m_ComponentsType.remove(component);
    }

    /*
    ============================================
    component_manager::delete_collider_component
    ============================================
    */
    inline void component_manager::delete_collider_component(component_id component)
    {
        m_ColliderComponents.remove(component);
        m_ComponentsType.remove(component);
    }

    /*
    ================================================
    component_manager::delete_acceleration_component
    ================================================
    */
    inline void component_manager::delete_acceleration_component(component_id component)
    {
        m_AccelerationComponents.remove(component);
        m_ComponentsType.remove(component);
    }

    /*
    ==========================================
    component_manager::delete_health_component
    ==========================================
    */
    inline void component_manager::delete_health_component(component_id id)
    {
        m_HealthComponents.remove(id);
        m_ComponentsType.remove(id);
    }

    /*
    ========================================
    transformation_component::get_translation
    ========================================
    */
    inline vec3<float> transformation_component::get_translation() const
    {
        return m_Translation;
    }

    /*
    ====================================
    transformation_component::get_scaling
    ====================================
    */
    inline vec3<float> transformation_component::get_scaling() const
    {
        return m_Scaling;
    }

    /*
    ======================================
    transformation_component::get_rotation_X
    ======================================
    */
    inline float transformation_component::get_rotation_X() const
    {
        return m_RotationX;
    }

    /*
    ======================================
    transformation_component::get_rotation_Y
    ======================================
    */
    inline float transformation_component::get_rotation_Y() const
    {
        return m_RotationY;
    }

    /*
    ========================================
    transformation_component::get_rotation_Z
    ========================================
    */
    inline float transformation_component::get_rotation_Z() const
    {
        return m_RotationZ;
    }

    /*
    =========================================
    transformation_component::set_translation
    =========================================
    */
    inline void transformation_component::set_translation(const vec3<float> &translation)
    {
        m_Translation = translation;
    }

    /*
    =====================================
    transformation_component::set_scaling
    =====================================
    */
    inline void transformation_component::set_scaling(const vec3<float> &scaling)
    {
        m_Scaling = scaling;
    }

    /*
    ========================================
    transformation_component::set_rotation_X
    ========================================
    */
    inline void transformation_component::set_rotation_X(float rotation)
    {
        m_RotationX = rotation;
    }

    /*
    ========================================
    transformation_component::set_rotation_Y
    ========================================
    */
    inline void transformation_component::set_rotation_Y(float rotation)
    {
        m_RotationY = rotation;
    }

    /*
    ========================================
    transformation_component::set_rotation_Z
    ========================================
    */
    inline void transformation_component::set_rotation_Z(float rotation)
    {
        m_RotationZ = rotation;
    }

    inline vec3<float> velocity_component::get_velocity() const
    {
        return m_Velocity;
    }

    inline void velocity_component::set_velocity(const vec3<float> &vel)
    {
        m_Velocity = vel;
    }

}

#endif