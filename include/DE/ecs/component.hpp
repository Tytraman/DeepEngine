#ifndef __DEEP_ENGINE_COMPONENT_HPP__
#define __DEEP_ENGINE_COMPONENT_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/ecs/ecs.hpp>
#include <DE/vec.hpp>
#include <DE/memory/list.hpp>
#include <DE/graphics/vertex.hpp>
#include <DE/graphics/shape.hpp>
#include <DE/memory/hash_table.hpp>
#include <DE/rendering/opengl_utils.hpp>

namespace de {

	struct DrawableComponent;
	struct TransformationComponent;
	struct VelocityComponent;
	struct ColliderComponent;
	struct AccelerationComponent;
	struct HealthComponent;

	class SystemManager;
	class Window;
	class Camera;
	class OpenGLRenderer;

	/// @class ComponentManager
	/// @brief Permet la gestion de composants.
	class DE_API ComponentManager {

		public:
			/// @brief           Récupère le type du composant spécifié.
			/// @param component L'ID du composant auquel récupérer le type.
			/// @return          Le type du composant.
			static component_type getType(component_id component);

			static component_id createDrawableComponent();
			static component_id createDrawableComponent(gl_program program, gl_vbo vbo, gl_vao vao, gl_texture texture = 0, uint8_t textureUnit = 0);
			static DrawableComponent *getDrawableComponent(component_id component);
			static void deleteDrawableComponent(component_id id);

			static component_id createTransformationComponent(const fvec3 &translation, const fvec3 &scaling = fvec3(1.0f, 1.0f, 1.0f), float rotation = 0.0f);
			static TransformationComponent *getTransformationComponent(component_id component);
			static void deleteTransformationComponent(component_id id);

			static component_id createColliderComponent();
			static ColliderComponent *getColliderComponent(component_id component);
			static void deleteColliderComponent(component_id id);

			static component_id createVelocityComponent();
			static VelocityComponent *getVelocityComponent(component_id component);
			static void deleteVelocityComponent(component_id id);

			static component_id createAccelerationComponent(const fvec2 &acceleration);
			static AccelerationComponent *getAccelerationComponent(component_id component);
			static void deleteAccelerationComponent(component_id component);
			
			static component_id createHealthComponent(uint32_t pv, uint32_t max);
			static HealthComponent *getHealthComponent(component_id component);
			static void deleteHealthComponent(component_id id);

		private:
			static component_id m_ComponentCount;    ///< Comptabilise le nombre total de composants créés.
			static HashTable m_ComponentsType;
			static HashTable m_DrawableComponents;
			static HashTable m_TransformationComponents;
			static HashTable m_VelocityComponents;
			static HashTable m_ColliderComponents;
			static HashTable m_AccelerationComponents;
			static HashTable m_HealthComponents;

	};

	/*
	======================================
	ComponentManager::getDrawableComponent
	======================================
	*/
	inline DrawableComponent *ComponentManager::getDrawableComponent(component_id component)
	{
		return (DrawableComponent *) m_DrawableComponents.getPtr(component);
	}

	/*
	============================================
	ComponentManager::getTransformationComponent
	============================================
	*/
	inline TransformationComponent *ComponentManager::getTransformationComponent(component_id component)
	{
		return (TransformationComponent *) m_TransformationComponents.getPtr(component);
	}

	/*
	======================================
	ComponentManager::getVelocityComponent
	======================================
	*/
	inline VelocityComponent *ComponentManager::getVelocityComponent(component_id component)
	{
		return (VelocityComponent *) m_VelocityComponents.getPtr(component);
	}

	/*
	======================================
	ComponentManager::getColliderComponent
	======================================
	*/
	inline ColliderComponent *ComponentManager::getColliderComponent(component_id component)
	{
		return (ColliderComponent *) m_ColliderComponents.getPtr(component);
	}

	/*
	==========================================
	ComponentManager::getAccelerationComponent
	==========================================
	*/
	inline AccelerationComponent *ComponentManager::getAccelerationComponent(component_id component)
	{
		return (AccelerationComponent *) m_AccelerationComponents.getPtr(component);
	}

	/*
	====================================
	ComponentManager::getHealthComponent
	====================================
	*/
	inline HealthComponent *ComponentManager::getHealthComponent(component_id component)
	{
		return (HealthComponent *) m_HealthComponents.getPtr(component);
	}

	/*
	=========================================
	ComponentManager::deleteDrawableComponent
	=========================================
	*/
	inline void ComponentManager::deleteDrawableComponent(component_id component)
	{
		m_DrawableComponents.destroy(component);
		m_ComponentsType.destroy(component);
	}

	/*
	===============================================
	ComponentManager::deleteTransformationComponent
	===============================================
	*/
	inline void ComponentManager::deleteTransformationComponent(component_id component)
	{
		m_TransformationComponents.destroy(component);
		m_ComponentsType.destroy(component);
	}

	/*
	=========================================
	ComponentManager::deleteVelocityComponent
	=========================================
	*/
	inline void ComponentManager::deleteVelocityComponent(component_id component)
	{
		m_VelocityComponents.destroy(component);
		m_ComponentsType.destroy(component);
	}

	/*
	=========================================
	ComponentManager::deleteColliderComponent
	=========================================
	*/
	inline void ComponentManager::deleteColliderComponent(component_id component)
	{
		m_ColliderComponents.destroy(component);
		m_ComponentsType.destroy(component);
	}

	/*
	=============================================
	ComponentManager::deleteAccelerationComponent
	=============================================
	*/
	inline void ComponentManager::deleteAccelerationComponent(component_id component)
	{
		m_AccelerationComponents.destroy(component);
		m_ComponentsType.destroy(component);
	}

	/*
	=======================================
	ComponentManager::deleteHealthComponent
	=======================================
	*/
	inline void ComponentManager::deleteHealthComponent(component_id id)
	{
		m_HealthComponents.destroy(id);
		m_ComponentsType.destroy(id);
	}

	using DrawableRenderCallback = void (*)(OpenGLRenderer &renderer, DrawableComponent *drawable, TransformationComponent *transformation, Window *window, Camera *camera);

	constexpr component_type DrawableComponentType = (1 << 0);

	/// @struct DrawableComponent
	/// @brief	Composant indiquant qu'une entité peut dessiner une forme.
	struct DE_API DrawableComponent {
		gl_vbo vbo;
		gl_vao vao;
		gl_texture texture;
		uint8_t textureUnit;
		gl_program program;
		DrawableRenderCallback renderCallback;

		static void classicRenderCallback(OpenGLRenderer &renderer, DrawableComponent *drawable, TransformationComponent *transformation, Window *window, Camera *camera);
		static void skyboxRenderCallback(OpenGLRenderer &renderer, DrawableComponent *drawable, TransformationComponent *transformation, Window *window, Camera *camera);
		
		private:
			DrawableComponent();
			DrawableComponent(gl_program program, gl_vbo vbo, gl_vao vao, gl_texture texture = 0, uint8_t textureUnit = 0);

			friend ComponentManager;
	};

	constexpr component_type TransformationComponentType = (1 << 1);

	/// @struct TransformationComponent
	/// @brief	Composant indiquant qu'une entité possède une transformation.
	struct DE_API TransformationComponent {
		
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

			TransformationComponent(const fvec3 &translation, const fvec3 &scaling, float rotationX = 0.0f, float rotationY = 0.0f, float rotationZ = 0.0f);

			friend ComponentManager;
			friend SystemManager;
	};

	/*
	=======================================
	TransformationComponent::getTranslation
	=======================================
	*/
	inline fvec3 TransformationComponent::getTranslation() const
	{
		return m_Translation;
	}

	/*
	===================================
	TransformationComponent::getScaling
	===================================
	*/
	inline fvec3 TransformationComponent::getScaling() const
	{
		return m_Scaling;
	}

	/*
	=====================================
	TransformationComponent::getRotationX
	=====================================
	*/
	inline float TransformationComponent::getRotationX() const
	{
		return m_RotationX;
	}

	/*
	=====================================
	TransformationComponent::getRotationY
	=====================================
	*/
	inline float TransformationComponent::getRotationY() const
	{
		return m_RotationY;
	}

	/*
	=====================================
	TransformationComponent::getRotationZ
	=====================================
	*/
	inline float TransformationComponent::getRotationZ() const
	{
		return m_RotationZ;
	}

	/*
	=======================================
	TransformationComponent::setTranslation
	=======================================
	*/
	inline void TransformationComponent::setTranslation(const fvec3 &translation)
	{
		m_Translation = translation;
	}

	/*
	===================================
	TransformationComponent::setScaling
	===================================
	*/
	inline void TransformationComponent::setScaling(const fvec3 &scaling)
	{
		m_Scaling = scaling;
	}

	/*
	=====================================
	TransformationComponent::setRotationX
	=====================================
	*/
	inline void TransformationComponent::setRotationX(float rotation)
	{
		m_RotationX = rotation;
	}

	/*
	=====================================
	TransformationComponent::setRotationY
	=====================================
	*/
	inline void TransformationComponent::setRotationY(float rotation)
	{
		m_RotationY = rotation;
	}

	/*
	=====================================
	TransformationComponent::setRotationZ
	=====================================
	*/
	inline void TransformationComponent::setRotationZ(float rotation)
	{
		m_RotationZ = rotation;
	}

	constexpr component_type ColliderComponentType = (1 << 2);

	/// @struct ColliderComponent
	/// @brief  Composant décrivant qu'une entité possède une boîte de collision de type AABB.
	/// @remark En utilisant l'AABB, il n'est pas conseillé de tourner les objets car la boîte de collision est rectangulaire.
	struct DE_API ColliderComponent {
		Rect contour;
		
		private:
			ColliderComponent();

			friend ComponentManager;
	};

	constexpr component_type VelocityComponentType = (1 << 3);
	struct DE_API VelocityComponent {
		public:
			fvec3 getVelocity() const;
			void setVelocity(const fvec3 &vel);

		private:
			fvec3 m_Velocity;

			VelocityComponent();

			friend ComponentManager;
	};

	inline fvec3 VelocityComponent::getVelocity() const
	{
		return m_Velocity;
	}

	inline void VelocityComponent::setVelocity(const fvec3 &vel)
	{
		m_Velocity = vel;
	}

	constexpr component_type AccelerationComponentType = (1 << 4);

	struct DE_API AccelerationComponent {
		fvec2 acceleration;

		private:
			AccelerationComponent(const fvec2 &acceleration);

			friend ComponentManager;
	};

	constexpr component_type HealthComponentType = (1 << 5);

	/// @struct HealthComponent
	/// @brief	Composant indiquant qu'une entité possède une quantité de points de vie.
	struct DE_API HealthComponent {
		uint32_t pv;
		uint32_t maxPv;

		private:
			HealthComponent();
			HealthComponent(uint32_t pv, uint32_t maxPv);

			friend ComponentManager;
	};

}

#endif