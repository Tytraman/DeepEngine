#include <DE/ecs/component.hpp>
#include <DE/graphics/graphic.hpp>
#include <DE/mat.hpp>
#include <DE/window.hpp>

#include <unordered_map>

namespace de {

	component_id ComponentManager::m_ComponentCount = 0;

	static std::unordered_map<component_id, component_type>          m_ComponentsType;
	static std::unordered_map<component_id, DrawableComponent>       m_DrawableComponents;
	static std::unordered_map<component_id, TransformationComponent> m_TransformationComponents;
	static std::unordered_map<component_id, VelocityComponent>       m_VelocityComponents;
	static std::unordered_map<component_id, ColliderComponent>       m_ColliderComponents;
	static std::unordered_map<component_id, AccelerationComponent>   m_AccelerationComponents;
	static std::unordered_map<component_id, HealthComponent>         m_HealthComponents;

	void DrawableComponent::classicRenderCallback(OpenGLRenderer &renderer, DrawableComponent *drawable, TransformationComponent *transformation, Window *window, Camera *camera)
	{
		GLUniform uniModel;

		GLProgram::use(drawable->program);
		GLVAO::bind(drawable->vao);

		GLTexture::bind(drawable->texture, drawable->textureUnit);
		if(uniModel.find(drawable->program, "myTex"))
			uniModel.send(0);

		if(uniModel.find(drawable->program, "model")) {
			fmat4x4 model = fmat4x4::translate(fmat4x4(), transformation->getTranslation());
					model = fmat4x4::rotateX(model, transformation->getRotationX());
					model = fmat4x4::rotateY(model, transformation->getRotationY());
					model = fmat4x4::rotateZ(model, transformation->getRotationZ());
					model = fmat4x4::scale(model, transformation->getScaling());

			uniModel.send(model);

			if(uniModel.find(drawable->program, "view")) {
				uniModel.send(camera->lookAt());

				if(uniModel.find(drawable->program, "proj")) {
					uniModel.send(fmat4x4::perspective(fmat4x4(), 45.0f, (float) window->getWidth() / (float) window->getHeight(), 0.1f, 1000.0f));
				}
			}
		}

		renderer.draw(GLVBO::getVerticesNumber(drawable->vbo));
	}

	void DrawableComponent::skyboxRenderCallback(OpenGLRenderer &renderer, DrawableComponent *drawable, TransformationComponent *transformation, Window *window, Camera *camera)
	{
		GLUniform uniModel;

		GLCore::enableDepthMask(false);
		GLCore::setDepthFunction(GLDepthFunction::Lequal);

		GLProgram::use(drawable->program);
		GLVAO::bind(drawable->vao);

		GLTexture::bindCubemaps(drawable->texture);

		if(uniModel.find(drawable->program, "skybox")) {
			uniModel.send(0);

			if(uniModel.find(drawable->program, "view")) {
				fmat4x4 view = camera->lookAt();
				view[fmat4x4_index::w1] = 0.0f;
				view[fmat4x4_index::w2] = 0.0f;
				view[fmat4x4_index::w3] = 0.0f;

				uniModel.send(view);

				if(uniModel.find(drawable->program, "proj")) {
					uniModel.send(fmat4x4::perspective(fmat4x4(), 45.0f, (float) window->getWidth() / (float) window->getHeight(), 0.1f, 1000.0f));
				}
			}
		}

		renderer.draw(GLVBO::getVerticesNumber(drawable->vbo));
		GLCore::setDepthFunction(GLDepthFunction::Less);
		GLCore::enableDepthMask(true);
	}

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
		: vbo(GLVBO::create()),
		  vao(GLVAO::create()),
		  texture(0),
		  textureUnit(0),
		  program(0),
		  renderCallback(nullptr)
	{ }

	/*
	====================================
	DrawableComponent::DrawableComponent
	====================================
	*/
	DrawableComponent::DrawableComponent(gl_program _program, gl_vbo _vbo, gl_vao _vao, gl_texture _texture, uint8_t _textureUnit)
		: vbo(_vbo),
		  vao(_vao),
		  texture(_texture),
		  textureUnit(_textureUnit),
		  program(_program),
		  renderCallback(nullptr)
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
	=========================================
	ComponentManager::createDrawableComponent
	=========================================
	*/
	component_id ComponentManager::createDrawableComponent(gl_program program, gl_vbo vbo, gl_vao vao, gl_texture texture, uint8_t textureUnit)
	{
		component_id id = m_ComponentCount;

		m_DrawableComponents.emplace(id, DrawableComponent(program, vbo, vao, texture, textureUnit));
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
	TransformationComponent::TransformationComponent(const fvec3 &translation, const fvec3 &scaling, float rotationX, float rotationY, float rotationZ)
		: m_Translation(translation),
		  m_Scaling(scaling),
		  m_RotationX(rotationX),
		  m_RotationY(rotationY),
		  m_RotationZ(rotationZ)
	{ }

	/*
	===============================================
	ComponentManager::createTransformationComponent
	===============================================
	*/
	component_id ComponentManager::createTransformationComponent(const fvec3 &translation, const fvec3 &scaling, float rotation)
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
		: m_Velocity(0.0f, 0.0f, 0.0f)
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

