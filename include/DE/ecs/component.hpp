#ifndef __DEEP_ENGINE_COMPONENT_HPP__
#define __DEEP_ENGINE_COMPONENT_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/vec.hpp>
#include <DE/memory/list.hpp>
#include <DE/graphic/vertex.hpp>

namespace de {

	using component_id   = de_id;			///< Identificateur de composant.
	using component_type = uint32_t;		///< Type associé à un composant.

	struct DrawableComponent;
	struct TransformationComponent;
	struct MoveableComponent;
	struct ScalableComponent;
	struct RotatableComponent;
	struct VelocityComponent;
	struct ColliderComponent;
	struct HealthComponent;

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
			static DrawableComponent *getDrawableComponent(component_id component);
			static void deleteDrawableComponent(component_id id);

			static component_id createTransformationComponent(const fvec2 &translation, const fvec2 &scaling = fvec2(1.0f, 1.0f), float rotation = 0.0f);
			static TransformationComponent *getTransformationComponent(component_id component);
			static void deleteTransformationComponent(component_id id);

			static component_id createMoveableComponent(const fvec2 &movement);
			static MoveableComponent *getMoveableComponent(component_id component);
			static void deleteMoveableComponent(component_id id);

			static component_id createScalableComponent(const fvec2 &scaling);
			static ScalableComponent *getScalableComponent(component_id component);
			static void deleteScalableComponent(component_id id);

			static component_id createRotatableComponent(float degrees);
			static RotatableComponent *getRotatableComponent(component_id component);
			static void deleteRotatableComponent(component_id id);

			static component_id createVelocityComponent();
			static VelocityComponent *getVelocityComponent(component_id component);
			static void deleteVelocityComponent(component_id id);

			static component_id createColliderComponent();
			static ColliderComponent *getColliderComponent(component_id component);
			static void deleteColliderComponent(component_id id);
			
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

		List vertices;	///< Contient la liste des sommets du composant de dessin.
		uint8_t flags;	///< Contient les options du composant de dessin.
		
		private:
			DrawableComponent();

			friend ComponentManager;
	};

	constexpr component_type TransformationComponentType = (1 << 1);

	/// @struct TransformationComponent
	/// @brief	Composant indiquant qu'une entité possède une transformation.
	struct DE_API TransformationComponent {
		fvec2 translation;
		fvec2 scaling;
		float rotation;

		private:
			TransformationComponent(const fvec2 &translation, const fvec2 &scaling, float rotation);

			friend ComponentManager;
	};

	constexpr component_type MoveableComponentType = (1 << 2);

	/// @struct MoveableComponent
	/// @brief	Composant indiquant qu'une entité est déplaçable.
	struct DE_API MoveableComponent {
		fvec2 movement;

		private:
			MoveableComponent();
			MoveableComponent(const fvec2 &movement);

			friend ComponentManager;
	};

	constexpr component_type ScalableComponentType = (1 << 3);

	/// @struct ScalableComponent
	/// @brief	Composant indiquant qu'une entité est redimensionnable.
	struct DE_API ScalableComponent {
		fvec2 scaling;

		private:
			ScalableComponent();
			ScalableComponent(const fvec2 &scaling);

			friend ComponentManager;
	};

	constexpr component_type RotatableComponentType = (1 << 4);

	/// @struct RotatableComponent
	/// @brief	Composant indiquant qu'une entité est pivotable.
	struct DE_API RotatableComponent {
		float degrees;

		private:
			RotatableComponent();
			RotatableComponent(float degrees);

			friend ComponentManager;
	};

	constexpr component_type VelocityComponentType = (1 << 5);
	struct DE_API VelocityComponent {

		private:
			VelocityComponent();

			friend ComponentManager;
	};

	constexpr component_type ColliderComponentType = (1 << 6);
	struct DE_API ColliderComponent {
		
		private:
			ColliderComponent();

			friend ComponentManager;
	};

	constexpr component_type HealthComponentType = (1 << 7);

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