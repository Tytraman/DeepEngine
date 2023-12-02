#include <DE/ecs/component.hpp>
#include <DE/graphics/graphic.hpp>
#include <DE/mat.hpp>
#include <DE/window.hpp>

#include <unordered_map>

namespace de
{

	void destroy_drawable_callback(mem_ptr ptr)
	{
		drawable_component *drawable = (drawable_component *) ptr;

		vao_manager::destroy(drawable->vao);
		vbo_manager::destroy(drawable->vbo);
	}

	component_id component_manager::m_ComponentCount = 0;

	hash_table<component_type> component_manager::m_ComponentsType(1000);
	hash_table<drawable_component> component_manager::m_DrawableComponents(1000/*, destroy_drawable_callback*/);
	hash_table<transformation_component> component_manager::m_TransformationComponents(1000);
	hash_table<velocity_component> component_manager::m_VelocityComponents(1000);
	hash_table<collider_component> component_manager::m_ColliderComponents(1000);
	hash_table<acceleration_component> component_manager::m_AccelerationComponents(1000);
	hash_table<health_component> component_manager::m_HealthComponents(1000);

	/*
	=========================================
	drawable_component::classicRenderCallback
	=========================================
	*/
	void drawable_component::classicRenderCallback(gl_renderer &renderer, drawable_component *drawable, transformation_component *transformation, window *window, Camera *camera)
	{
		gl_uniform uniModel;

		program_id program = drawable->program;
		vao_id vao = drawable->vao;
		texture_id texture = drawable->texture;
		uint8_t textureUnit = drawable->textureUnit;

		// Pas besoin d'indiquer à OpenGL d'utiliser un programme qui est déjà en cours d'utilisation.
		if(program != program_manager::currentID())
			program_manager::use(program);

		// Pas besoin d'indiquer à OpenGL d'utiliser un VAO qui est déjà en cours d'utilisation.
		if(vao != vao_manager::currentID())
			vao_manager::bind(vao);

		// Pas besoin d'indiquer à OpenGL d'utiliser une texture si elle est déjà en cours d'utilisation.
		if(texture != texture_manager::currentID() || textureUnit != texture_manager::currentUnit())
			texture_manager::bind(texture, textureUnit);

		if(uniModel.find(program, "myTex"))
			uniModel.send(0);

		if(uniModel.find(program, "mTrs"))
        {
			uniModel.send(transformation->getTranslation());

			if(uniModel.find(program, "mRotX"))
            {
				uniModel.send(transformation->getRotationX());

				if(uniModel.find(program, "mRotY"))
                {
					uniModel.send(transformation->getRotationY());

					if(uniModel.find(program, "mRotZ"))
                    {
						uniModel.send(transformation->getRotationZ());

						if(uniModel.find(program, "mScl"))
                        {
							uniModel.send(transformation->getScaling());

							if(uniModel.find(program, "view"))
                            {
								uniModel.send(camera->lookAt());

								if(uniModel.find(program, "proj"))
                                {
									uniModel.send(fmat4x4::perspective(fmat4x4(), 45.0f, (float) window->getWidth() / (float) window->getHeight(), 0.1f, 1000.0f));
								}
							}
						}
					}
				}
			}
		}

		renderer.draw(vbo_manager::getVerticesNumber(drawable->vbo));
	}

	/*
	=======================================
	drawable_component::skyboxRenderCallback
	=======================================
	*/
	void drawable_component::skyboxRenderCallback(gl_renderer &renderer, drawable_component *drawable, transformation_component *transformation, window *window, Camera *camera)
	{
		gl_uniform uniModel;

		gpu_core::enableDepthMask(false);
		gpu_core::setDepthFunction(gl_depth_function::Lequal);

		program_manager::use(drawable->program);
		vao_manager::bind(drawable->vao);

		texture_manager::bindCubemaps(drawable->texture);

		if(uniModel.find(drawable->program, "skybox"))
        {
			uniModel.send(0);

			if(uniModel.find(drawable->program, "view"))
            {
				fmat4x4 view = camera->lookAt();
				view[static_cast<uint8_t>(fmat4x4_index::w1)] = 0.0f;
				view[static_cast<uint8_t>(fmat4x4_index::w2)] = 0.0f;
				view[static_cast<uint8_t>(fmat4x4_index::w3)] = 0.0f;

				uniModel.send(view);

				if(uniModel.find(drawable->program, "proj"))
                {
					uniModel.send(fmat4x4::perspective(fmat4x4(), 45.0f, (float) window->getWidth() / (float) window->getHeight(), 0.1f, 1000.0f));
				}
			}
		}

		renderer.draw(vbo_manager::getVerticesNumber(drawable->vbo));
		gpu_core::setDepthFunction(gl_depth_function::Less);
		gpu_core::enableDepthMask(true);
	}

	/*
	==========================
	component_manager::getType
	==========================
	*/
	component_type component_manager::getType(component_id component)
	{
        const auto hs = m_ComponentsType[component];
		if(hs == nullptr)
			return 0;

		return hs->value;
	}

	/*
	======================================
	drawable_component::drawable_component
	======================================
	*/
	drawable_component::drawable_component(program_id _program, vbo_id _vbo, vao_id _vao, texture_id _texture, uint8_t _textureUnit)
		: vbo(_vbo),
		  vao(_vao),
		  texture(_texture),
		  textureUnit(_textureUnit),
		  program(_program),
		  renderCallback(nullptr)
	{ }

	/*
	==========================================
	component_manager::createDrawableComponent
	==========================================
	*/
	component_id component_manager::createDrawableComponent(program_id program, vbo_id vbo, vao_id vao, texture_id texture, uint8_t textureUnit)
	{
		component_id id = m_ComponentCount;
		drawable_component drawable(program, vbo, vao, texture, textureUnit);
		component_type type = DrawableComponentType;

		m_DrawableComponents.insert(id, drawable);
		m_ComponentsType.insert(id, type);

		m_ComponentCount = id + 1;

		return id;
	}

    /*
	==========================================
	component_manager::createDrawableComponent
	==========================================
	*/
    component_id component_manager::createDrawableComponent(const char *progName, const char *vboName, const char *vaoName, const char *textName, uint8_t textureUnit)
    {
        hash_function hash = program_manager::getHashFunction();

        program_id prog = hash(progName);
        vbo_id vbo = hash(vboName);
        vao_id vao = hash(vaoName);

        texture_id texture = 0;
        if(textName != nullptr)
            texture = hash(textName);

        return createDrawableComponent(prog, vbo, vao, texture, textureUnit);
    }

	/*
	==================================================
	transformation_component::transformation_component
	==================================================
	*/
	transformation_component::transformation_component(const fvec3 &translation, const fvec3 &scaling, float rotationX, float rotationY, float rotationZ)
		: m_Translation(translation),
		  m_Scaling(scaling),
		  m_RotationX(rotationX),
		  m_RotationY(rotationY),
		  m_RotationZ(rotationZ)
	{ }

	/*
	================================================
	component_manager::createTransformationComponent
	================================================
	*/
	component_id component_manager::createTransformationComponent(const fvec3 &translation, const fvec3 &scaling, float rotation)
	{
		component_id id = m_ComponentCount;
		transformation_component transformation(translation, scaling, rotation);
		component_type type = TransformationComponentType;

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
	component_manager::createVelocityComponent
	=========================================
	*/
	component_id component_manager::createVelocityComponent()
	{
		component_id id = m_ComponentCount;
		velocity_component velocity;
		component_type type = VelocityComponentType;

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
		: contour(fvec2(0.0f, 0.0f), 0.0f, 0.0f)
	{ }

	/*
	=========================================
	component_manager::createColliderComponent
	=========================================
	*/
	component_id component_manager::createColliderComponent()
	{
		component_id id = m_ComponentCount;
		collider_component collider;
		component_type type = ColliderComponentType;

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
	acceleration_component::acceleration_component(const fvec2 &_acceleration)
		: acceleration(_acceleration)
	{ }

	/*
	=============================================
	component_manager::createAccelerationComponent
	=============================================
	*/
	component_id component_manager::createAccelerationComponent(const fvec2 &acceleration)
	{
		component_id id = m_ComponentCount;
		acceleration_component acc(acceleration);
		component_type type = AccelerationComponentType;

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
	component_manager::createHealthComponent
	=======================================
	*/
	component_id component_manager::createHealthComponent(uint32_t pv, uint32_t max)
	{
		component_id id = m_ComponentCount;
		health_component health(pv, max);
		component_type type = HealthComponentType;

		m_HealthComponents.insert(id, health);
		m_ComponentsType.insert(id, type);

		m_ComponentCount = id + 1;

		return id;
	}

}

