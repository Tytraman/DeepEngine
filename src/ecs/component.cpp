#include <DE/ecs/component.hpp>
#include <DE/graphic/graphic.hpp>
#include <DE/mat.hpp>

#include <unordered_map>

namespace de {

	component_id ComponentManager::m_ComponentCount = 0;

	static std::unordered_map<component_id, component_type>				m_ComponentsType;
	static std::unordered_map<component_id, DrawableComponent>			m_DrawableComponents;
	static std::unordered_map<component_id, TransformationComponent>	m_TransformationComponents;
	static std::unordered_map<component_id, VelocityComponent>			m_VelocityComponents;
	static std::unordered_map<component_id, ColliderComponent>			m_ColliderComponents;
	static std::unordered_map<component_id, AccelerationComponent>      m_AccelerationComponents;
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
	==================================
	DrawableComponent::calcRectContour
	==================================
	*/
	Rect DrawableComponent::calcRectContour()
	{
		size_t length = vertices.getNumberOfElements();
		size_t i;
		Vertex *vertList = (Vertex *) vertices.getData();
		Vertex *vert;

		float topY    = vertList->pos.y;
		float bottomY = vertList->pos.y;
		float leftX   = vertList->pos.x;
		float rightX  = vertList->pos.x;

		// Itère à travers tous les sommets.
		for(i = 1; i < length; ++i) {
			vert = vertList + i;

			// Top Y
			if(vert->pos.y < topY)
				topY = vert->pos.y;
			// Bottom Y
			else if(vert->pos.y > bottomY)
				bottomY = vert->pos.y;

			// Left X
			if(vert->pos.x < leftX)
				leftX = vert->pos.x;
			// Right X
			else if(vert->pos.x > rightX)
				rightX = vert->pos.x;
		}

		return Rect(fvec2(leftX, topY), rightX - leftX, bottomY - topY);
	}

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
	void ComponentManager::deleteDrawableComponent(component_id component)
	{
		m_DrawableComponents.erase(component);
		m_ComponentsType.erase(component);
	}

	/*
	================================================
	TransformationComponent::TransformationComponent
	================================================
	*/
	TransformationComponent::TransformationComponent(const fvec2 &translation, const fvec2 &scaling, float rotation)
		: m_Translation(translation), m_Scaling(scaling), m_Rotation(rotation), m_LastMovement(0.0f, 0.0f)
	{ }

	/*
	================================================
	TransformationComponent::applyAABBTransformation
	================================================
	*/
	void TransformationComponent::applyAABBTransformation(Rect &rectangle) const
	{
		fmat3x3 trans = fmat3x3::translate(fmat3x3(), m_Translation);
		        trans = fmat3x3::scale(trans, m_Scaling);

		fvec3 vec(rectangle.pos.x, rectangle.pos.y, 1.0f);
		vec = vec * trans;

		rectangle.pos.x = vec.x;
		rectangle.pos.y = vec.y;

		rectangle.w *= m_Scaling.x;
		rectangle.h *= m_Scaling.y;
	}

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
	void ComponentManager::deleteTransformationComponent(component_id component)
	{
		m_TransformationComponents.erase(component);
		m_ComponentsType.erase(component);
	}

	/*
	====================================
	VelocityComponent::VelocityComponent
	====================================
	*/
	VelocityComponent::VelocityComponent()
		: m_Velocity(0.0f, 0.0f)
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
	======================================
	ComponentManager::getVelocityComponent
	======================================
	*/
	VelocityComponent *ComponentManager::getVelocityComponent(component_id component)
	{
		const auto &it = m_VelocityComponents.find(component);
		if(it == m_VelocityComponents.end())
			return nullptr;

		return &it->second;
	}

	/*
	=========================================
	ComponentManager::deleteVelocityComponent
	=========================================
	*/
	void ComponentManager::deleteVelocityComponent(component_id component)
	{
		m_VelocityComponents.erase(component);
		m_ComponentsType.erase(component);
	}

	/*
	====================================
	ColliderComponent::ColliderComponent
	====================================
	*/
	ColliderComponent::ColliderComponent()
		: contour(fvec2(0.0f, 0.0f), 0.0f, 0.0f)
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
	======================================
	ComponentManager::getColliderComponent
	======================================
	*/
	ColliderComponent *ComponentManager::getColliderComponent(component_id component)
	{
		const auto &it = m_ColliderComponents.find(component);
		if(it == m_ColliderComponents.end())
			return nullptr;

		return &it->second;
	}

	/*
	=========================================
	ComponentManager::deleteColliderComponent
	=========================================
	*/
	void ComponentManager::deleteColliderComponent(component_id component)
	{
		m_ColliderComponents.erase(component);
		m_ComponentsType.erase(component);
	}


	AccelerationComponent::AccelerationComponent(const fvec2 &_acceleration)
		: acceleration(_acceleration)
	{ }

	component_id ComponentManager::createAccelerationComponent(const fvec2 &acceleration)
	{
		component_id id = m_ComponentCount;

		m_AccelerationComponents.emplace(id, AccelerationComponent(acceleration));
		m_ComponentsType[id] = AccelerationComponentType;

		m_ComponentCount = id + 1;

		return id;
	}

	AccelerationComponent *ComponentManager::getAccelerationComponent(component_id component)
	{
		const auto &it = m_AccelerationComponents.find(component);
		if(it == m_AccelerationComponents.end())
			return nullptr;

		return &it->second;
	}

	void ComponentManager::deleteAccelerationComponent(component_id component)
	{
		m_AccelerationComponents.erase(component);
		m_ComponentsType.erase(component);
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

