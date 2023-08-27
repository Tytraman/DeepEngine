#ifndef __DEEP_ENGINE_COMPONENT_HPP__
#define __DEEP_ENGINE_COMPONENT_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/ecs/ecs.hpp>
#include <DE/vec.hpp>
#include <DE/memory/list.hpp>
#include <DE/graphics/vertex.hpp>
#include <DE/graphics/shape.hpp>
#include <DE/rendering/opengl_utils.hpp>

namespace de {

	struct DrawableComponent;
	struct TransformationComponent;
	struct VelocityComponent;
	struct ColliderComponent;
	struct AccelerationComponent;
	struct HealthComponent;

	class SystemManager;

	/// @class ComponentManager
	/// @brief Permet la gestion de composants.
	class DE_API ComponentManager {

		private:
			static component_id m_ComponentCount;	///< Comptabilise le nombre total de composants créés.

		public:
			/// @brief				Récupère le type du composant spécifié.
			/// @param component	L'ID du composant auquel récupérer le type.
			/// @return				Le type du composant.
			static component_type getType(component_id component);

			static component_id createDrawableComponent();
			static component_id createDrawableComponent(const GLVBO &vbo, const GLVAO &vao);
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

	};

	constexpr component_type DrawableComponentType = (1 << 0);

	/// @struct DrawableComponent
	/// @brief	Composant indiquant qu'une entité peut dessiner une forme.
	struct DE_API DrawableComponent {
		static constexpr uint8_t FlagVisible =    (1 << 0);
		static constexpr uint8_t FlagDrawVector = (1 << 1);

		GLVBO vbo;
		GLVAO vao;
		uint8_t flags;	///< Contient les options du composant de dessin.
		
		private:
			DrawableComponent();
			DrawableComponent(const GLVBO &vbo, const GLVAO &vao);

			friend ComponentManager;
	};

	constexpr component_type TransformationComponentType = (1 << 1);

	/// @struct TransformationComponent
	/// @brief	Composant indiquant qu'une entité possède une transformation.
	struct DE_API TransformationComponent {
		
		public:
			fvec3 getTranslation() const;
			fvec3 getScaling() const;
			float getRotation() const;
			fvec3 getLastMovement() const;

			void setTranslation(const fvec3 &translation);
			void setScaling(const fvec3 &scaling);
			void setRotation(float rotation);

		private:
			fvec3 m_Translation;
			fvec3 m_Scaling;
			float m_Rotation;

			fvec3 m_LastMovement;

			TransformationComponent(const fvec3 &translation, const fvec3 &scaling, float rotation);

			friend ComponentManager;
			friend SystemManager;
	};

	inline fvec3 TransformationComponent::getTranslation() const
	{
		return m_Translation;
	}

	inline fvec3 TransformationComponent::getScaling() const
	{
		return m_Scaling;
	}

	inline float TransformationComponent::getRotation() const
	{
		return m_Rotation;
	}

	inline fvec3 TransformationComponent::getLastMovement() const
	{
		return m_LastMovement;
	}

	inline void TransformationComponent::setTranslation(const fvec3 &translation)
	{
		m_LastMovement += m_Translation - translation;
		m_Translation = translation;
	}

	inline void TransformationComponent::setScaling(const fvec3 &scaling)
	{
		m_Scaling = scaling;
	}

	inline void TransformationComponent::setRotation(float rotation)
	{
		m_Rotation = rotation;
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