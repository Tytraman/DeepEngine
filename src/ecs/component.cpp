#include <DE/ecs/component.hpp>
#include <DE/graphics/graphic.hpp>
#include <DE/mat.hpp>
#include <DE/window.hpp>

#include <unordered_map>

namespace de {

	void destroy_drawable_callback(mem_ptr ptr)
	{
		DrawableComponent *drawable = (DrawableComponent *) ptr;

		GLVAO::destroy(drawable->vao);
		GLVBO::destroy(drawable->vbo);
	}

	component_id ComponentManager::m_ComponentCount = 0;

	HashTable ComponentManager::m_ComponentsType(sizeof(component_type), 1000);
	HashTable ComponentManager::m_DrawableComponents(sizeof(DrawableComponent), 1000, StringUtils::hash, destroy_drawable_callback);
	HashTable ComponentManager::m_TransformationComponents(sizeof(TransformationComponent), 1000);
	HashTable ComponentManager::m_VelocityComponents(sizeof(VelocityComponent), 1000);
	HashTable ComponentManager::m_ColliderComponents(sizeof(ColliderComponent), 1000);
	HashTable ComponentManager::m_AccelerationComponents(sizeof(AccelerationComponent), 1000);
	HashTable ComponentManager::m_HealthComponents(sizeof(HealthComponent), 1000);

	/*
	========================================
	DrawableComponent::classicRenderCallback
	========================================
	*/
	void DrawableComponent::classicRenderCallback(OpenGLRenderer &renderer, DrawableComponent *drawable, TransformationComponent *transformation, Window *window, Camera *camera)
	{
		GLUniform uniModel;

		gl_program program = drawable->program;
		gl_vao vao = drawable->vao;
		gl_texture texture = drawable->texture;
		uint8_t textureUnit = drawable->textureUnit;

		// Pas besoin d'indiquer à OpenGL d'utiliser un programme qui est déjà en cours d'utilisation.
		if(program != GLProgram::currentlyBound())
			GLProgram::use(drawable->program);

		// Pas besoin d'indiquer à OpenGL d'utiliser un VAO qui est déjà en cours d'utilisation.
		if(vao != GLVAO::currentlyBound())
			GLVAO::bind(drawable->vao);

		// Pas besoin d'indiquer à OpenGL d'utiliser une texture si elle est déjà en cours d'utilisation.
		if(texture != GLTexture::currentlyBound() || textureUnit != GLTexture::currentUnit())
			GLTexture::bind(drawable->texture, drawable->textureUnit);

		if(uniModel.find(drawable->program, "myTex"))
			uniModel.send(0);

		if(uniModel.find(drawable->program, "mTrs")) {
			uniModel.send(transformation->getTranslation());

			if(uniModel.find(drawable->program, "mRotX")) {
				uniModel.send(transformation->getRotationX());

				if(uniModel.find(drawable->program, "mRotY")) {
					uniModel.send(transformation->getRotationY());

					if(uniModel.find(drawable->program, "mRotZ")) {
						uniModel.send(transformation->getRotationZ());

						if(uniModel.find(drawable->program, "mScl")) {
							uniModel.send(transformation->getScaling());

							if(uniModel.find(drawable->program, "view")) {
								uniModel.send(camera->lookAt());

								if(uniModel.find(drawable->program, "proj")) {
									uniModel.send(fmat4x4::perspective(fmat4x4(), 45.0f, (float) window->getWidth() / (float) window->getHeight(), 0.1f, 1000.0f));
								}
							}
						}
					}
				}
			}
		}

		renderer.draw(GLVBO::getVerticesNumber(drawable->vbo));
	}

	/*
	=======================================
	DrawableComponent::skyboxRenderCallback
	=======================================
	*/
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
		mem_ptr ptr = m_ComponentsType.getPtr(component);
		if(ptr == nullptr)
			return 0;

		return *((component_type *) ptr);
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
		DrawableComponent drawable;
		component_type type = DrawableComponentType;

		m_DrawableComponents.insertCopy(id, &drawable);
		m_ComponentsType.insertCopy(id, &type);

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
		DrawableComponent drawable(program, vbo, vao, texture, textureUnit);
		component_type type = DrawableComponentType;

		m_DrawableComponents.insertCopy(id, &drawable);
		m_ComponentsType.insertCopy(id, &type);

		m_ComponentCount = id + 1;

		return id;
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
		TransformationComponent transformation(translation, scaling, rotation);
		component_type type = TransformationComponentType;

		m_TransformationComponents.insertCopy(id, &transformation);
		m_ComponentsType.insertCopy(id, &type);

		m_ComponentCount = id + 1;

		return id;
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
		VelocityComponent velocity;
		component_type type = VelocityComponentType;

		m_VelocityComponents.insertCopy(id, &velocity);
		m_ComponentsType.insertCopy(id, &type);

		m_ComponentCount = id + 1;

		return id;
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
		ColliderComponent collider;
		component_type type = ColliderComponentType;

		m_ColliderComponents.insertCopy(id, &collider);
		m_ComponentsType.insertCopy(id, &type);

		m_ComponentCount = id + 1;

		return id;
	}

	/*
	============================================
	AccelerationComponent::AccelerationComponent
	============================================
	*/
	AccelerationComponent::AccelerationComponent(const fvec2 &_acceleration)
		: acceleration(_acceleration)
	{ }

	/*
	=============================================
	ComponentManager::createAccelerationComponent
	=============================================
	*/
	component_id ComponentManager::createAccelerationComponent(const fvec2 &acceleration)
	{
		component_id id = m_ComponentCount;
		AccelerationComponent acc(acceleration);
		component_type type = AccelerationComponentType;

		m_AccelerationComponents.insertCopy(id, &acc);
		m_ComponentsType.insertCopy(id, &type);

		m_ComponentCount = id + 1;

		return id;
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
		HealthComponent health(pv, max);
		component_type type = HealthComponentType;

		m_HealthComponents.insertCopy(id, &health);
		m_ComponentsType.insertCopy(id, &type);

		m_ComponentCount = id + 1;

		return id;
	}

}

