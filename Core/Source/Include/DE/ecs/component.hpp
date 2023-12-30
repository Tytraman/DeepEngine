#ifndef __DEEP_ENGINE_COMPONENT_HPP__
#define __DEEP_ENGINE_COMPONENT_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/ecs/ecs.hpp"
#include "DE/vec.hpp"
#include "DE/memory/list.hpp"
#include "DE/graphics/vertex.hpp"
#include "DE/graphics/shape.hpp"
#include "DE/memory/hash_table.hpp"
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

    class system_manager;
    class window;
    class Camera;
    class gl_renderer;

    using drawable_render_callback = void (*)(GL3::gl_renderer &renderer, drawable_component *drawable, transformation_component *transformation, window *window, Camera *camera);

    constexpr component_type DrawableComponentType = (1 << 0);

    /// @struct drawable_component
    /// @brief	Composant indiquant qu'une entité peut dessiner une forme.
    struct DE_API drawable_component
    {
        GL3::gl_id vbo;
        GL3::gl_id vao;
        GL3::gl_id texture;
        uint8_t textureUnit;
        GL3::gl_id program;
        drawable_render_callback renderCallback;

        static void classicRenderCallback(GL3::gl_renderer &renderer, drawable_component *drawable, transformation_component *transformation, window *window, Camera *camera);
        static void skyboxRenderCallback(GL3::gl_renderer &renderer, drawable_component *drawable, transformation_component *transformation, window *window, Camera *camera);
        
        private:
            drawable_component(GL3::gl_id program, GL3::gl_id vbo, GL3::gl_id vao, GL3::gl_id texture = 0, uint8_t textureUnit = 0);

            friend component_manager;
    };

    constexpr component_type TransformationComponentType = (1 << 1);

    /// @struct transformation_component
    /// @brief	Composant indiquant qu'une entité possède une transformation.
    struct DE_API transformation_component
    {
        
        public:
            fvec3 getTranslation() const;
            fvec3 getScaling() const;
            float getRotationX() const;
            float getRotationY() const;
            float getRotationZ() const;

            void setTranslation(const fvec3 &translation);
            void setScaling(const fvec3 &scaling);
            void setRotationX(float rotation);
            void setRotationY(float rotation);
            void setRotationZ(float rotation);

        private:
            fvec3 m_Translation;
            fvec3 m_Scaling;
            float m_RotationX;
            float m_RotationY;
            float m_RotationZ;

            transformation_component(const fvec3 &translation, const fvec3 &scaling, float rotationX = 0.0f, float rotationY = 0.0f, float rotationZ = 0.0f);

            friend component_manager;
            friend system_manager;
    };

    constexpr component_type VelocityComponentType = (1 << 3);
    struct DE_API velocity_component
    {

        public:
            fvec3 getVelocity() const;
            void setVelocity(const fvec3 &vel);

        private:
            fvec3 m_Velocity;

            velocity_component();

            friend component_manager;
    };

    constexpr component_type ColliderComponentType = (1 << 2);

    /// @struct collider_component
    /// @brief  Composant décrivant qu'une entité possède une boîte de collision de type AABB.
    /// @remark En utilisant l'AABB, il n'est pas conseillé de tourner les objets car la boîte de collision est rectangulaire.
    struct DE_API collider_component
    {

        public:
            rect contour;
        
        private:
            collider_component();

            friend component_manager;
    };

    constexpr component_type AccelerationComponentType = (1 << 4);

    struct DE_API acceleration_component
    {

        public:
            fvec2 acceleration;

        private:
            acceleration_component(const fvec2 &acceleration);

            friend component_manager;
    };

    constexpr component_type HealthComponentType = (1 << 5);

    /// @struct health_component
    /// @brief	Composant indiquant qu'une entité possède une quantité de points de vie.
    struct DE_API health_component
    {

        public:
            uint32_t pv;
            uint32_t maxPv;

        private:
            health_component();
            health_component(uint32_t pv, uint32_t maxPv);

            friend component_manager;
    };

    /// @class component_manager
    /// @brief Permet la gestion de composants.
    class component_manager
    {

        public:
            DE_API static component_manager *get_singleton();

            /// @brief           Récupère le type du composant spécifié.
            /// @param component L'ID du composant auquel récupérer le type.
            /// @return          Le type du composant.
            DE_API component_type getType(component_id component);

            DE_API component_id createDrawableComponent(GL3::gl_id program, GL3::gl_id vbo, GL3::gl_id vao, GL3::gl_id texture = 0, uint8_t textureUnit = 0);
            DE_API component_id createDrawableComponent(const char *progName, const char *vboName, const char *vaoName, const char *textName = nullptr, uint8_t textureUnit = 0);

            DE_API drawable_component *getDrawableComponent(component_id component);
            DE_API void deleteDrawableComponent(component_id id);

            DE_API component_id createTransformationComponent(const fvec3 &translation, const fvec3 &scaling = fvec3(1.0f, 1.0f, 1.0f), float rotation = 0.0f);
            DE_API transformation_component *getTransformationComponent(component_id component);
            DE_API void deleteTransformationComponent(component_id id);

            DE_API component_id createColliderComponent();
            DE_API collider_component *getColliderComponent(component_id component);
            DE_API void deleteColliderComponent(component_id id);

            DE_API component_id createVelocityComponent();
            DE_API velocity_component *getVelocityComponent(component_id component);
            DE_API void deleteVelocityComponent(component_id id);

            DE_API component_id createAccelerationComponent(const fvec2 &acceleration);
            DE_API acceleration_component *getAccelerationComponent(component_id component);
            DE_API void deleteAccelerationComponent(component_id component);
            
            DE_API component_id createHealthComponent(uint32_t pv, uint32_t max);
            DE_API health_component *getHealthComponent(component_id component);
            DE_API void deleteHealthComponent(component_id id);

        private:
            component_manager();

            component_id m_ComponentCount;                  ///< Comptabilise le nombre total de composants créés.
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
    ======================================
    component_manager::getDrawableComponent
    ======================================
    */
    inline drawable_component *component_manager::getDrawableComponent(component_id component)
    {
        return &m_DrawableComponents[component]->value;
    }

    /*
    ============================================
    component_manager::getTransformationComponent
    ============================================
    */
    inline transformation_component *component_manager::getTransformationComponent(component_id component)
    {
        return &m_TransformationComponents[component]->value;
    }

    /*
    ======================================
    component_manager::getVelocityComponent
    ======================================
    */
    inline velocity_component *component_manager::getVelocityComponent(component_id component)
    {
        return &m_VelocityComponents[component]->value;
    }

    /*
    ======================================
    component_manager::getColliderComponent
    ======================================
    */
    inline collider_component *component_manager::getColliderComponent(component_id component)
    {
        return &m_ColliderComponents[component]->value;
    }

    /*
    ==========================================
    component_manager::getAccelerationComponent
    ==========================================
    */
    inline acceleration_component *component_manager::getAccelerationComponent(component_id component)
    {
        return &m_AccelerationComponents[component]->value;
    }

    /*
    ====================================
    component_manager::getHealthComponent
    ====================================
    */
    inline health_component *component_manager::getHealthComponent(component_id component)
    {
        return &m_HealthComponents[component]->value;
    }

    /*
    =========================================
    component_manager::deleteDrawableComponent
    =========================================
    */
    inline void component_manager::deleteDrawableComponent(component_id component)
    {
        m_DrawableComponents.remove(component);
        m_ComponentsType.remove(component);
    }

    /*
    ===============================================
    component_manager::deleteTransformationComponent
    ===============================================
    */
    inline void component_manager::deleteTransformationComponent(component_id component)
    {
        m_TransformationComponents.remove(component);
        m_ComponentsType.remove(component);
    }

    /*
    =========================================
    component_manager::deleteVelocityComponent
    =========================================
    */
    inline void component_manager::deleteVelocityComponent(component_id component)
    {
        m_VelocityComponents.remove(component);
        m_ComponentsType.remove(component);
    }

    /*
    =========================================
    component_manager::deleteColliderComponent
    =========================================
    */
    inline void component_manager::deleteColliderComponent(component_id component)
    {
        m_ColliderComponents.remove(component);
        m_ComponentsType.remove(component);
    }

    /*
    =============================================
    component_manager::deleteAccelerationComponent
    =============================================
    */
    inline void component_manager::deleteAccelerationComponent(component_id component)
    {
        m_AccelerationComponents.remove(component);
        m_ComponentsType.remove(component);
    }

    /*
    =======================================
    component_manager::deleteHealthComponent
    =======================================
    */
    inline void component_manager::deleteHealthComponent(component_id id)
    {
        m_HealthComponents.remove(id);
        m_ComponentsType.remove(id);
    }

    

    

    /*
    =======================================
    transformation_component::getTranslation
    =======================================
    */
    inline fvec3 transformation_component::getTranslation() const
    {
        return m_Translation;
    }

    /*
    ===================================
    transformation_component::getScaling
    ===================================
    */
    inline fvec3 transformation_component::getScaling() const
    {
        return m_Scaling;
    }

    /*
    =====================================
    transformation_component::getRotationX
    =====================================
    */
    inline float transformation_component::getRotationX() const
    {
        return m_RotationX;
    }

    /*
    =====================================
    transformation_component::getRotationY
    =====================================
    */
    inline float transformation_component::getRotationY() const
    {
        return m_RotationY;
    }

    /*
    =====================================
    transformation_component::getRotationZ
    =====================================
    */
    inline float transformation_component::getRotationZ() const
    {
        return m_RotationZ;
    }

    /*
    =======================================
    transformation_component::setTranslation
    =======================================
    */
    inline void transformation_component::setTranslation(const fvec3 &translation)
    {
        m_Translation = translation;
    }

    /*
    ===================================
    transformation_component::setScaling
    ===================================
    */
    inline void transformation_component::setScaling(const fvec3 &scaling)
    {
        m_Scaling = scaling;
    }

    /*
    =====================================
    transformation_component::setRotationX
    =====================================
    */
    inline void transformation_component::setRotationX(float rotation)
    {
        m_RotationX = rotation;
    }

    /*
    =====================================
    transformation_component::setRotationY
    =====================================
    */
    inline void transformation_component::setRotationY(float rotation)
    {
        m_RotationY = rotation;
    }

    /*
    =====================================
    transformation_component::setRotationZ
    =====================================
    */
    inline void transformation_component::setRotationZ(float rotation)
    {
        m_RotationZ = rotation;
    }

    

    

    inline fvec3 velocity_component::getVelocity() const
    {
        return m_Velocity;
    }

    inline void velocity_component::setVelocity(const fvec3 &vel)
    {
        m_Velocity = vel;
    }

}

#endif