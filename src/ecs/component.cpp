#include <DE/ecs/component.hpp>

#include <unordered_map>

namespace de {

	component_id ComponentManager::m_ComponentCount = 0;

	static std::unordered_map<component_id, component_type>				m_ComponentsType;
	static std::unordered_map<component_id, DrawableComponent>			m_DrawableComponents;
	static std::unordered_map<component_id, TransformationComponent>	m_TransformationComponents;
	static std::unordered_map<component_id, MoveableComponent>			m_MoveableComponents;
	static std::unordered_map<component_id, ScalableComponent>			m_ScalableComponents;
	static std::unordered_map<component_id, RotatableComponent>			m_RotatableComponents;
	static std::unordered_map<component_id, VelocityComponent>			m_VelocityComponents;
	static std::unordered_map<component_id, ColliderComponent>			m_ColliderComponents;
	static std::unordered_map<component_id, HealthComponent>			m_HealthComponents;

	/*
	=========================
	ComponentManager::getType
	=========================
	*/
	component_type ComponentManager::getType(component_id component)
	{
		return m_ComponentsType[component];
	}

	/*
	====================================
	DrawableComponent::DrawableComponent
	====================================
	*/
	DrawableComponent::DrawableComponent()
		: vertices(sizeof(Vertex)), flags(0)
	{ }

	/*
	=========================================
	ComponentManager::createDrawableComponent
	=========================================
	*/
	component_id ComponentManager::createDrawableComponent()
	{
		component_id id = m_ComponentCount;

		m_DrawableComponents.emplace(id, DrawableComponent());
		m_ComponentsType[id] = DrawableComponentType;

		m_ComponentCount = id + 1;

		return id;
	}

	/*
	======================================
	ComponentManager::getDrawableComponent
	======================================
	*/
	DrawableComponent *ComponentManager::getDrawableComponent(component_id component)
	{
		const auto &it = m_DrawableComponents.find(component);
		if(it == m_DrawableComponents.end())
			return nullptr;

		return &it->second;
	}

	/*
	=========================================
	ComponentManager::deleteDrawableComponent
	=========================================
	*/
	void ComponentManager::deleteDrawableComponent(component_id id)
	{
		m_DrawableComponents.erase(id);
		m_ComponentsType.erase(id);
	}

	/*
	================================================
	TransformationComponent::TransformationComponent
	================================================
	*/
	TransformationComponent::TransformationComponent(const fvec2 &_translation, const fvec2 &_scaling, float _rotation)
		: translation(_translation), scaling(_scaling), rotation(_rotation)
	{ }

	/*
	===============================================
	ComponentManager::createTransformationComponent
	===============================================
	*/
	component_id ComponentManager::createTransformationComponent(const fvec2 &translation, const fvec2 &scaling, float rotation)
	{
		component_id id = m_ComponentCount;

		m_TransformationComponents.emplace(id, TransformationComponent(translation, scaling, rotation));
		m_ComponentsType[id] = TransformationComponentType;

		m_ComponentCount = id + 1;

		return id;
	}

	/*
	============================================
	ComponentManager::getTransformationComponent
	============================================
	*/
	TransformationComponent *ComponentManager::getTransformationComponent(component_id component)
	{
		const auto &it = m_TransformationComponents.find(component);
		if(it == m_TransformationComponents.end())
			return nullptr;

		return &it->second;
	}

	/*
	===============================================
	ComponentManager::deleteTransformationComponent
	===============================================
	*/
	void ComponentManager::deleteTransformationComponent(component_id id)
	{
		m_TransformationComponents.erase(id);
		m_ComponentsType.erase(id);
	}

	/*
	====================================
	MoveableComponent::MoveableComponent
	====================================
	*/
	MoveableComponent::MoveableComponent()
		: movement(fvec2(0.0f, 0.0f))
	{ }

	/*
	====================================
	MoveableComponent::MoveableComponent
	====================================
	*/
	MoveableComponent::MoveableComponent(const fvec2 &__movement)
		: movement(__movement)
	{ }

	/*
	=========================================
	ComponentManager::createMoveableComponent
	=========================================
	*/
	component_id ComponentManager::createMoveableComponent(const fvec2 &movement)
	{
		component_id id = m_ComponentCount;

		m_MoveableComponents.emplace(id, MoveableComponent(movement));
		m_ComponentsType[id] = MoveableComponentType;

		m_ComponentCount = id + 1;

		return id;
	}

	/*
	=========================================
	ComponentManager::deleteMoveableComponent
	=========================================
	*/
	void ComponentManager::deleteMoveableComponent(component_id id)
	{
		m_MoveableComponents.erase(id);
		m_ComponentsType.erase(id);
	}

	/*
	=====================================
	ScalableComponent::	ScalableComponent
	=====================================
	*/
	ScalableComponent::	ScalableComponent()
		: scaling(fvec2(0.0f, 0.0f))
	{ }

	/*
	=====================================
	ScalableComponent::	ScalableComponent
	=====================================
	*/
	ScalableComponent::ScalableComponent(const fvec2 &__scaling)
		: scaling(__scaling)
	{ }

	/*
	=========================================
	ComponentManager::createScalableComponent
	=========================================
	*/
	component_id ComponentManager::createScalableComponent(const fvec2 &scaling)
	{
		component_id id = m_ComponentCount;

		m_ScalableComponents.emplace(id, ScalableComponent(scaling));
		m_ComponentsType[id] = ScalableComponentType;

		m_ComponentCount = id + 1;

		return id;
	}

	/*
	=========================================
	ComponentManager::deleteScalableComponent
	=========================================
	*/
	void ComponentManager::deleteScalableComponent(component_id id)
	{
		m_ScalableComponents.erase(id);
		m_ComponentsType.erase(id);
	}

	/*
	======================================
	RotatableComponent::RotatableComponent
	======================================
	*/
	RotatableComponent::RotatableComponent()
		: degrees(0.0f)
	{ }
	
	/*
	======================================
	RotatableComponent::RotatableComponent
	======================================
	*/
	RotatableComponent::RotatableComponent(float __degrees)
		: degrees(__degrees)
	{ }

	/*
	==========================================
	ComponentManager::createRotatableComponent
	==========================================
	*/
	component_id ComponentManager::createRotatableComponent(float degrees)
	{
		component_id id = m_ComponentCount;

		m_RotatableComponents.emplace(id, RotatableComponent(degrees));
		m_ComponentsType[id] = RotatableComponentType;

		m_ComponentCount = id + 1;

		return id;
	}

	/*
	==========================================
	ComponentManager::deleteRotatableComponent
	==========================================
	*/
	void ComponentManager::deleteRotatableComponent(component_id id)
	{
		m_RotatableComponents.erase(id);
		m_ComponentsType.erase(id);
	}

	/*
	====================================
	VelocityComponent::VelocityComponent
	====================================
	*/
	VelocityComponent::VelocityComponent()
	{ }

	/*
	=========================================
	ComponentManager::createVelocityComponent
	=========================================
	*/
	component_id ComponentManager::createVelocityComponent()
	{
		component_id id = m_ComponentCount;

		m_VelocityComponents.emplace(id, VelocityComponent());
		m_ComponentsType[id] = VelocityComponentType;

		m_ComponentCount = id + 1;

		return id;
	}

	/*
	=========================================
	ComponentManager::deleteVelocityComponent
	=========================================
	*/
	void ComponentManager::deleteVelocityComponent(component_id id)
	{
		m_VelocityComponents.erase(id);
		m_ComponentsType.erase(id);
	}

	/*
	====================================
	ColliderComponent::ColliderComponent
	====================================
	*/
	ColliderComponent::ColliderComponent()
	{ }

	/*
	=========================================
	ComponentManager::createColliderComponent
	=========================================
	*/
	component_id ComponentManager::createColliderComponent()
	{
		component_id id = m_ComponentCount;

		m_ColliderComponents.emplace(id, ColliderComponent());
		m_ComponentsType[id] = ColliderComponentType;

		m_ComponentCount = id + 1;

		return id;
	}

	/*
	=========================================
	ComponentManager::deleteColliderComponent
	=========================================
	*/
	void ComponentManager::deleteColliderComponent(component_id id)
	{
		m_ColliderComponents.erase(id);
		m_ComponentsType.erase(id);
	}

	/*
	================================
	HealthComponent::HealthComponent
	================================
	*/
	HealthComponent::HealthComponent()
		: pv(0), maxPv(0)
	{ }

	/*
	================================
	HealthComponent::HealthComponent
	================================
	*/
	HealthComponent::HealthComponent(uint32_t __pv, uint32_t __maxPv)
		: pv(__pv), maxPv(__maxPv)
	{ }

	/*
	=======================================
	ComponentManager::createHealthComponent
	=======================================
	*/
	component_id ComponentManager::createHealthComponent(uint32_t pv, uint32_t max)
	{
		component_id id = m_ComponentCount;

		m_HealthComponents.emplace(id, HealthComponent(pv, max));
		m_ComponentsType[id] = HealthComponentType;

		m_ComponentCount = id + 1;

		return id;
	}

	/*
	=======================================
	ComponentManager::deleteHealthComponent
	=======================================
	*/
	void ComponentManager::deleteHealthComponent(component_id id)
	{
		m_HealthComponents.erase(id);
		m_ComponentsType.erase(id);
	}

}

