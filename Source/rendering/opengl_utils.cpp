#include <DE/rendering/opengl_utils.hpp>
#include <DE/string_utils.hpp>
#include <DE/image/bmp.hpp>

#include <stdio.h>

#include <glad/glad.h>

namespace de
{

	void programs_hashtable_free_element_callback(gl_program_int &program)
	{
		program_manager::rawDestroy(program);
	}

	gl_vbo_int vbo_manager::m_CurrentlyBound = 0;
    vbo_id vbo_manager::m_CurrentID = 0;
    hash_table<pair<gl_vbo_int, unsigned int>> vbo_manager::m_VBOs;

	gl_vao_int vao_manager::m_CurrentlyBound = 0;
    vao_id vao_manager::m_CurrentID = 0;
    hash_table<gl_vao_int> vao_manager::m_VAOs;

    hash_table<gl_shader_int> shader_manager::m_Shaders;

	texture_id texture_manager::m_WhiteTex = 0;
	gl_texture_int texture_manager::m_CurrentlyBound = 0;
    texture_id texture_manager::m_CurrentID = 0;
    texture_id texture_manager::m_CurrentCubemapsID = 0;
	uint8_t texture_manager::m_CurrentUnit = 0;
    hash_table<gl_texture_int> texture_manager::m_Textures;

    gl_framebuffer_int framebuffer_manager::m_CurrentlyBound = 0;
    framebuffer_id framebuffer_manager::m_CurrentID = 0;
    hash_table<pair<gl_framebuffer_int, texture_id>> framebuffer_manager::m_Framebuffers;

    hash_table<gl_renderbuffer_int> renderbuffer_manager::m_Renderbuffers;
    gl_renderbuffer_int renderbuffer_manager::m_CurrentlyBound = 0;
    renderbuffer_id renderbuffer_manager::m_CurrentID = 0;

	gl_program_int program_manager::m_CurrentlyBound = 0;
    program_id program_manager::m_CurrentID = 0;
	hash_table<gl_program_int> program_manager::m_Programs(100, string::hash, programs_hashtable_free_element_callback);

	/*
	===================
	vbo_manager::create
	===================
	*/
	vbo_id vbo_manager::create(const char *name)
	{
		gl_vbo_int vbo;
		DE_GL_CALL(glGenBuffers(1, &vbo));

        auto &el = m_VBOs.insert(name, pair(vbo, static_cast<unsigned int>(0)));

        return el.key;
	}

    /*
	====================
	vbo_manager::rawBind
	====================
	*/
    void vbo_manager::rawBind(gl_vbo_int vbo)
    {
        DE_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

        m_CurrentlyBound = vbo;
    }

	/*
	=================
	vbo_manager::bind
	=================
	*/
	bool vbo_manager::bind(vbo_id vbo)
	{
        auto el = m_VBOs[vbo];
        if(el == nullptr)
            return false;

        rawBind(el->value.value1());

        m_CurrentID = vbo;

        return true;
	}

    /*
	=================
	vbo_manager::bind
	=================
	*/
    bool vbo_manager::bind(const char *name)
    {
        auto el = m_VBOs[name];
        if(el == nullptr)
            return false;

		rawBind(el->value.value2());
		
        m_CurrentID = el->key;

        return true;
    }

    /*
	========================
	vbo_manager::bindDefault
	========================
	*/
    void vbo_manager::bindDefault()
    {
        rawBind(0);
        m_CurrentID = 0;
    }

    /*
	=======================
	vbo_manager::rawDestroy
	=======================
	*/
    void vbo_manager::rawDestroy(gl_vbo_int vbo)
    {
        DE_GL_CALL(glDeleteBuffers(1, &vbo));
    }

	/*
	====================
	vbo_manager::destroy
	====================
	*/
	bool vbo_manager::destroy(vbo_id vbo)
	{
        auto el = m_VBOs[vbo];
        if(el == nullptr)
            return false;

		rawDestroy(el->value.value1());
        m_VBOs.remove(el->key);

        return true;
	}

    /*
	====================
	vbo_manager::destroy
	====================
	*/
    bool vbo_manager::destroy(const char *name)
    {
        auto el = m_VBOs[name];
        if(el == nullptr)
            return false;

		rawDestroy(el->value.value1());
        m_VBOs.remove(el->key);

        return true;
    }

	/*
	==============================
	vbo_manager::setVerticesNumber
	==============================
	*/
	void vbo_manager::setVerticesNumber(unsigned int number)
	{
        auto el = m_VBOs[m_CurrentID];
        if(el == nullptr)
            return;

        el->value.value2() = number;
	}

	/*
	==============================
	vbo_manager::getVerticesNumber
	==============================
	*/
	unsigned int vbo_manager::getVerticesNumber()
	{
		return getVerticesNumber(m_CurrentlyBound);
	}

	/*
	==============================
	vbo_manager::getVerticesNumber
	==============================
	*/
	unsigned int vbo_manager::getVerticesNumber(vbo_id vbo)
	{
        auto el = m_VBOs[vbo];
        if(el == nullptr)
            return 0;

        return el->value.value2();
	}

	/*
	=========================
	vbo_manager::transmitData
	=========================
	*/
	void vbo_manager::transmitData(memory_chunk &data)
	{
		DE_GL_CALL(glBufferData(GL_ARRAY_BUFFER, data.size(), data.data(), GL_STATIC_DRAW));
	}

	/*
	=========================
	vbo_manager::addAttribute
	=========================
	*/
	void vbo_manager::addAttribute(unsigned int index, gl_attrib_components_number componentsNumber, gl_type type, int stride, int offset)
	{
		DE_GL_CALL(glVertexAttribPointer(index, static_cast<GLint>(componentsNumber), static_cast<GLenum>(type), GL_FALSE, stride, (void *) offset));
		DE_GL_CALL(glEnableVertexAttribArray(index));
	}

	/*
	===================
	vao_manager::create
	===================
	*/
	vao_id vao_manager::create(const char *name)
	{
		gl_vao_int vao;

		DE_GL_CALL(glGenVertexArrays(1, &vao));

        auto &el = m_VAOs.insert(name, vao);

		return el.key;
	}

    /*
	====================
	vao_manager::rawBind
	====================
	*/
    void vao_manager::rawBind(gl_vao_int vao)
    {
        DE_GL_CALL(glBindVertexArray(vao));

        m_CurrentlyBound = vao;
    }

	/*
	=================
	vao_manager::bind
	=================
	*/
	bool vao_manager::bind(vao_id vao)
	{
        auto el = m_VAOs[vao];
        if(el == nullptr)
            return false;

        rawBind(el->value);

        m_CurrentID = el->key;

        return true;
	}

    /*
	=================
	vao_manager::bind
	=================
	*/
    bool vao_manager::bind(const char *name)
    {
        auto el = m_VAOs[name];
        if(el == nullptr)
            return false;

        rawBind(el->value);

        m_CurrentID = el->key;

        return true;
    }

    /*
	========================
	vao_manager::bindDefault
	========================
	*/
    void vao_manager::bindDefault()
    {
        rawBind(0);
        m_CurrentID = 0;
    }

    /*
	=======================
	vao_manager::rawDestroy
	=======================
	*/
    void vao_manager::rawDestroy(gl_vao_int vao)
    {
        DE_GL_CALL(glDeleteVertexArrays(1, &vao));
    }

	/*
	====================
	vao_manager::destroy
	====================
	*/
	bool vao_manager::destroy(vao_id vao)
	{
        auto el = m_VAOs[vao];
        if(el == nullptr)
            return false;

		rawDestroy(el->value);
        m_VAOs.remove(el->key);

        return true;
	}

    /*
	====================
	vao_manager::destroy
	====================
	*/
    bool vao_manager::destroy(const char *name)
    {
        auto el = m_VAOs[name];
        if(el == nullptr)
            return false;

		rawDestroy(el->value);
        m_VAOs.remove(el->key);

        return true;
    }

	/*
	======================
	gl_uniform::gl_uniform
	======================
	*/
	gl_uniform::gl_uniform()
		: m_Location(-1)
	{ }

	/*
	===================
	gl_uniform::rawFind
	===================
	*/
	bool gl_uniform::rawFind(gl_program_int program, const char *name)
	{
		m_Location = DE_GL_CALLV(glGetUniformLocation(program, name));
		if(m_Location == -1)
			return false;

		return true;
	}

    /*
	================
	gl_uniform::find
	================
	*/
    bool gl_uniform::find(program_id program, const char *name)
    {
        auto el = program_manager::get(program);
        if(el == nullptr)
            return false;

        return rawFind(el->value, name);
    }

    /*
	================
	gl_uniform::find
	================
	*/
    bool gl_uniform::find(const char *progName, const char *name)
    {
        auto el = program_manager::get(progName);
        if(el == nullptr)
            return false;

        return rawFind(el->value, name);
    }

	/*
	================
	gl_uniform::send
	================
	*/
	void gl_uniform::send(float value)
	{
		DE_GL_CALL(glUniform1f(m_Location, value));
	}

	/*
	================
	gl_uniform::send
	================
	*/
	void gl_uniform::send(int value)
	{
		DE_GL_CALL(glUniform1i(m_Location, value));
	}

	/*
	================
	gl_uniform::send
	================
	*/
	void gl_uniform::send(const fvec2 &vec)
	{
		DE_GL_CALL(glUniform2f(m_Location, vec.x, vec.y));
	}

	/*
	================
	gl_uniform::send
	================
	*/
	void gl_uniform::send(const fvec3 &vec)
	{
		DE_GL_CALL(glUniform3f(m_Location, vec.x, vec.y, vec.z));
	}

	/*
	================
	gl_uniform::send
	================
	*/
	void gl_uniform::send(fmat3x3 &mat)
	{
		DE_GL_CALL(glUniformMatrix3fv(m_Location, 1, GL_TRUE, mat.ptr()));
	}

	/*
	================
	gl_uniform::send
	================
	*/
	void gl_uniform::send(fmat4x4 &mat)
	{
		DE_GL_CALL(glUniformMatrix4fv(m_Location, 1, GL_TRUE, mat.ptr()));
	}

	/*
	======================
	shader_manager::create
	======================
	*/
	shader_id shader_manager::create(const char *name, gl_shader_type shaderType)
	{
		gl_shader_int shader = DE_GL_CALLV(glCreateShader((shaderType == gl_shader_type::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER)));

        auto &el = m_Shaders.insert(name, shader);

        return el.key;
	}

    /*
	======================
	shader_manager::rawLoad
	======================
	*/
    void shader_manager::rawLoad(gl_shader_int shader, memory_chunk &program)
    {
        char *source = (char *) program.data();
		DE_GL_CALL(glShaderSource(shader, 1, &source, NULL));
    }

	/*
	====================
	shader_manager::load
	====================
	*/
	bool shader_manager::load(shader_id shader, memory_chunk &program)
	{
		auto el = m_Shaders[shader];
        if(el == nullptr)
            return false;
        
        rawLoad(el->value, program);

        return true;
	}

    /*
	====================
	shader_manager::load
	====================
	*/
    bool shader_manager::load(const char *name, memory_chunk &program)
    {
        auto el = m_Shaders[name];
        if(el == nullptr)
            return false;
        
        rawLoad(el->value, program);

        return true;
    }

	/*
	==========================
	shader_manager::rawCompile
	==========================
	*/
	bool shader_manager::rawCompile(gl_shader_int shader)
	{
		int  success;
		char infoLog[512];

		DE_GL_CALL(glCompileShader(shader));
		DE_GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));

		if(!success)
        {
			DE_GL_CALL(glGetShaderInfoLog(shader, 512, NULL, infoLog));
			fprintf(stderr, "GLSL compilation error:\n%s\n", infoLog);

			return false;
		}

		return true;
	}

    /*
	=======================
	shader_manager::compile
	=======================
	*/
    bool shader_manager::compile(shader_id shader)
    {
        auto el = m_Shaders[shader];
        if(el == nullptr)
            return false;

        return rawCompile(el->value);
    }

    /*
	=======================
	shader_manager::compile
	=======================
	*/
    bool shader_manager::compile(const char *name)
    {
        auto el = m_Shaders[name];
        if(el == nullptr)
            return false;

        return rawCompile(el->value);
    }

	/*
	==========================
	shader_manager::rawDestroy
	==========================
	*/
	void shader_manager::rawDestroy(gl_shader_int shader)
	{
		DE_GL_CALL(glDeleteShader(shader));
	}

    /*
	=======================
	shader_manager::destroy
	=======================
	*/
    bool shader_manager::destroy(shader_id shader)
    {
        auto el = m_Shaders[shader];
        if(el == nullptr)
            return false;

        rawDestroy(el->value);
        m_Shaders.remove(el->key);

        return true;
    }

    /*
	=======================
	shader_manager::destroy
	=======================
	*/
    bool shader_manager::destroy(const char *name)
    {
        auto el = m_Shaders[name];
        if(el == nullptr)
            return false;

        rawDestroy(el->value);
        m_Shaders.remove(el->key);

        return true;
    }

	/*
	=======================
	program_manager::create
	=======================
	*/
	program_id program_manager::create(const char *name)
	{
		gl_program_int program = DE_GL_CALLV(glCreateProgram());

		auto &el = m_Programs.insert(name, program);

		return el.key;
	}

	/*
	================================
	program_manager::rawAttachShader
	================================
	*/
	void program_manager::rawAttachShader(gl_program_int program, gl_shader_int shader)
	{
		DE_GL_CALL(glAttachShader(program, shader));
	}

    /*
	=============================
	program_manager::attachShader
	=============================
	*/
    bool program_manager::attachShader(program_id program, shader_id shader)
    {
        auto prog = m_Programs[program];
        if(prog == nullptr)
            return false;

        auto shad = shader_manager::m_Shaders[shader];
        if(shad == nullptr)
            return false;

        rawAttachShader(prog->value, shad->value);

        return true;
    }

    /*
	=============================
	program_manager::attachShader
	=============================
	*/
    bool program_manager::attachShader(const char *progName, const char *shadName)
    {
        auto prog = m_Programs[progName];
        if(prog == nullptr)
            return false;

        auto shad = shader_manager::m_Shaders[shadName];
        if(shad == nullptr)
            return false;

        rawAttachShader(prog->value, shad->value);

        return true;
    }

    /*
	=============================
	program_manager::attachShader
	=============================
	*/
    bool program_manager::attachShader(const char *progName, shader_id shader)
    {
        auto prog = m_Programs[progName];
        if(prog == nullptr)
            return false;

        auto shad = shader_manager::m_Shaders[shader];
        if(shad == nullptr)
            return false;

        rawAttachShader(prog->value, shad->value);

        return true;
    }

    /*
	=============================
	program_manager::attachShader
	=============================
	*/
    bool program_manager::attachShader(program_id program, const char *shadName)
    {
        auto prog = m_Programs[program];
        if(prog == nullptr)
            return false;

        auto shad = shader_manager::m_Shaders[shadName];
        if(shad == nullptr)
            return false;

        rawAttachShader(prog->value, shad->value);

        return true;
    }

	/*
	========================
	program_manager::rawLink
	========================
	*/
	bool program_manager::rawLink(gl_program_int program)
	{
		int  success;
		char infoLog[512];

		DE_GL_CALL(glLinkProgram(program));
		DE_GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &success));

		if(!success)
        {
			DE_GL_CALL(glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog));
			fprintf(stderr, "GLSL link error:\n%s\n", infoLog);

			return false;
		}

		return true;
	}

    /*
	=====================
	program_manager::link
	=====================
	*/
    bool program_manager::link(program_id program)
    {
        auto el = m_Programs[program];
        if(el == nullptr)
            return false;

        return rawLink(el->value);
    }

    /*
	=====================
	program_manager::link
	=====================
	*/
    bool program_manager::link(const char *name)
    {
        auto el = m_Programs[name];
        if(el == nullptr)
            return false;

        return rawLink(el->value);
    }

	/*
	=======================
	program_manager::rawUse
	=======================
	*/
	void program_manager::rawUse(gl_program_int program)
	{
		DE_GL_CALL(glUseProgram(program));
		m_CurrentlyBound = program;
	}

    /*
	====================
	program_manager::use
	====================
	*/
    bool program_manager::use(program_id program)
    {
        auto el = m_Programs[program];
        if(el == nullptr)
            return false;

        rawUse(el->value);
        m_CurrentID = program;

        return true;
    }

    /*
	====================
	program_manager::use
	====================
	*/
    bool program_manager::use(const char *name)
    {
        auto el = m_Programs[name];
        if(el == nullptr)
            return false;

        rawUse(el->value);
        m_CurrentID = el->key;

        return true;
    }

	/*
	===========================
	program_manager::rawDestroy
	===========================
	*/
	void program_manager::rawDestroy(gl_program_int program)
	{
		DE_GL_CALL(glDeleteProgram(program));
	}

    /*
	========================
	program_manager::destroy
	========================
	*/
    bool program_manager::destroy(program_id program)
    {
        auto el = m_Programs[program];
        if(el == nullptr)
            return false;

        rawDestroy(el->value);
        m_Programs.remove(el->key);

        return true;
    }

    /*
	========================
	program_manager::destroy
	========================
	*/
    bool program_manager::destroy(const char *name)
    {
        auto el = m_Programs[name];
        if(el == nullptr)
            return false;

        rawDestroy(el->value);
        m_Programs.remove(el->key);

        return true;
    }

    /*
	===================================
	program_manager::destroyAllPrograms
	===================================
	*/
	void program_manager::destroyAllPrograms()
	{
        auto &beg = m_Programs.begin();
        auto &end = m_Programs.end();

        for(; beg != end; ++beg)
            rawDestroy(beg->value);

		m_Programs.clear();
	}

	/*
	=====================
	gl_error::clearErrors
	=====================
	*/
	void gl_error::clearErrors()
	{
		while(glGetError() != GL_NO_ERROR);
	}

	/*
	=====================
	gl_error::checkErrors
	=====================
	*/
	bool gl_error::checkErrors(const char *function, const char *file, unsigned int line)
	{
		GLenum error;
		bool ret = false;

		while((error = glGetError()) != GL_NO_ERROR) {
			fprintf(stderr, "[gl_error] (%d %s) %s %s: %u\n", error, getName(error), function, file, line);
			ret = true;
		}

		return ret;	
	}

	/*
	=================
	gl_error::getName
	=================
	*/
	const char *gl_error::getName(GLenum errorCode)
	{
		switch (errorCode)
        {
			default:                               return "UNKNOWN";
			case GL_NO_ERROR:                      return "NO_ERROR";
            case GL_INVALID_ENUM:                  return "INVALID_ENUM";
            case GL_INVALID_VALUE:                 return "INVALID_VALUE";
            case GL_INVALID_OPERATION:             return "INVALID_OPERATION";
            case GL_OUT_OF_MEMORY:                 return "OUT_OF_MEMORY";
            case GL_INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION";
        }
	}

	/*
	================
	gpu_core::version
	================
	*/
	const char *gpu_core::version()
	{
		return (const char *) glGetString(GL_VERSION);
	}

	/*
	=======================
	gpu_core::updateViewport
	=======================
	*/
	void gpu_core::updateViewport(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	/*
	=========================
	gpu_core::maxVertexAttribs
	=========================
	*/
	int gpu_core::maxVertexAttribs()
	{
		int val = 0;
		DE_GL_CALL(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &val));
		return val;
	}

	/*
	=============================
	gpu_core::maxTextureImageUnits
	=============================
	*/
	int gpu_core::maxTextureImageUnits()
	{
		int val = 0;
		DE_GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &val));
		return val;
	}

	/*
	=========================
	gpu_core::enableDepthMask
	=========================
	*/
	void gpu_core::enableDepthMask(bool value)
	{
		DE_GL_CALL(glDepthMask(value));
	}

    /*
	=========================
	gpu_core::enableDepthTest
	=========================
	*/
    void gpu_core::enableDepthTest()
    {
        DE_GL_CALL(glEnable(GL_DEPTH_TEST));
    }

    /*
	==========================
	gpu_core::disableDepthTest
	==========================
	*/
    void gpu_core::disableDepthTest()
    {
        DE_GL_CALL(glDisable(GL_DEPTH_TEST));
    }

	/*
	=========================
	gpu_core::setDepthFunction
	=========================
	*/
	void gpu_core::setDepthFunction(gl_depth_function func)
	{
		DE_GL_CALL(glDepthFunc(static_cast<GLenum>(func)));
	}

	/*
	====================
	gpu_core::setCullFace
	====================
	*/
	void gpu_core::setCullFace(gl_cull_face cullFace)
	{
		DE_GL_CALL(glCullFace(static_cast<GLenum>(cullFace)));
	}

	/*
	=======================
	texture_manager::create
	=======================
	*/
	texture_id texture_manager::create(const char *name)
	{
		gl_texture_int texture;

		DE_GL_CALL(glGenTextures(1, &texture));

        auto &el = m_Textures.insert(name, texture);

		return el.key;
	}

	/*
	========================
	texture_manager::rawBind
	========================
	*/
	void texture_manager::rawBind(gl_texture_int texture, uint8_t unit)
	{
		DE_GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
		DE_GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));

		m_CurrentlyBound = texture;
		m_CurrentUnit = unit;
	}

    /*
	=====================
	texture_manager::bind
	=====================
	*/
    bool texture_manager::bind(texture_id texture, uint8_t unit)
    {
        auto el = m_Textures[texture];
        if(el == nullptr)
            return false;

        rawBind(el->value, unit);

        m_CurrentID = el->key;

        return true;
    }

    /*
	=====================
	texture_manager::bind
	=====================
	*/
    bool texture_manager::bind(const char *name, uint8_t unit)
    {
        auto el = m_Textures[name];
        if(el == nullptr)
            return false;

        rawBind(el->value, unit);

        m_CurrentID = el->key;

        return true;
    }

	/*
	================================
	texture_manager::rawBindCubemaps
	================================
	*/
	void texture_manager::rawBindCubemaps(gl_texture_int texture)
	{
		DE_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, texture));

		m_CurrentlyBound = texture;
	}

    /*
	=============================
	texture_manager::bindCubemaps
	=============================
	*/
    bool texture_manager::bindCubemaps(texture_id texture)
    {
        auto el = m_Textures[texture];
        if(el == nullptr)
            return false;

        rawBindCubemaps(el->value);

        m_CurrentCubemapsID = el->key;

        return true;
    }

    /*
	=============================
	texture_manager::bindCubemaps
	=============================
	*/
    bool texture_manager::bindCubemaps(const char *name)
    {
        auto el = m_Textures[name];
        if(el == nullptr)
            return false;

        rawBindCubemaps(el->value);

        m_CurrentCubemapsID = el->key;

        return true;
    }

    /*
	===========================
	texture_manager::rawDestroy
	===========================
	*/
    void texture_manager::rawDestroy(gl_texture_int texture)
    {
        DE_GL_CALL(glDeleteTextures(1, &texture));
    }

    /*
	========================
	texture_manager::destroy
	========================
	*/
    bool texture_manager::destroy(texture_id texture)
    {
        auto el = m_Textures[texture];
        if(el == nullptr)
            return false;

        rawDestroy(el->value);
        m_Textures.remove(el->key);

        return true;
    }

    /*
	========================
	texture_manager::destroy
	========================
	*/
    bool texture_manager::destroy(const char *name)
    {
        auto el = m_Textures[name];
        if(el == nullptr)
            return false;

        rawDestroy(el->value);
        m_Textures.remove(el->key);

        return true;
    }

	/*
	====================================
	texture_manager::setTextureWrappingS
	====================================
	*/
	void texture_manager::setTextureWrappingS(gl_texture_wrap mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(mode)));
	}

	/*
	====================================
	texture_manager::setTextureWrappingT
	====================================
	*/
	void texture_manager::setTextureWrappingT(gl_texture_wrap mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(mode)));
	}

	/*
	====================================
	texture_manager::setTextureWrappingR
	====================================
	*/
	void texture_manager::setTextureWrappingR(gl_texture_wrap mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, static_cast<GLint>(mode)));
	}

	/*
	====================================
	texture_manager::setTextureFiltering
	====================================
	*/
	void texture_manager::setTextureFiltering(gl_texture_filter mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(mode)));
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mode)));
	}

	/*
	============================================
	texture_manager::setTextureWrappingSCubemaps
	============================================
	*/
	void texture_manager::setTextureWrappingSCubemaps(gl_texture_wrap mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<GLint>(mode)));
	}

	/*
	============================================
	texture_manager::setTextureWrappingTCubemaps
	============================================
	*/
	void texture_manager::setTextureWrappingTCubemaps(gl_texture_wrap mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<GLint>(mode)));
	}

	/*
	============================================
	texture_manager::setTextureWrappingRCubemaps
	============================================
	*/
	void texture_manager::setTextureWrappingRCubemaps(gl_texture_wrap mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<GLint>(mode)));
	}

	/*
	============================================
	texture_manager::setTextureFilteringCubemaps
	============================================
	*/
	void texture_manager::setTextureFilteringCubemaps(gl_texture_filter mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(mode)));
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mode)));
	}

    /*
	===========================
	texture_manager::allocSpace
	===========================
	*/
    void texture_manager::allocSpace(int width, int height)
    {
        DE_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    }

	/*
	================================
	texture_manager::transmitTexture
	================================
	*/
	void texture_manager::transmitTexture(mem_ptr data, int width, int height, image_color_type colorType)
	{
		GLenum internalFormat;
		GLenum format;
		switch(colorType)
        {
			default:
                return;
			case image_color_type::RGB:
            {
				internalFormat = GL_RGB8;
				format = GL_RGB;
			} break;
			case image_color_type::RGBA:
            {
				internalFormat = GL_RGBA8;
				format = GL_RGBA;
			} break;
		}

		DE_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data));
	}

	/*
	========================================
	texture_manager::transmitTextureCubemaps
	========================================
	*/
	void texture_manager::transmitTextureCubemaps(mem_ptr left, mem_ptr front, mem_ptr right, mem_ptr back, mem_ptr bottom, mem_ptr top, int width, int height, image_color_type colorType)
	{
		GLenum internalFormat;
		GLenum format;
		switch(colorType)
        {
			default:
                return;
			case image_color_type::RGB:
            {
				internalFormat = GL_RGB8;
				format = GL_RGB;
			} break;
			case image_color_type::RGBA:
            {
				internalFormat = GL_RGBA8;
				format = GL_RGBA;
			} break;
		}

		// Gauche
		DE_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, left));
		// Avant
		DE_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, front));
		// Droite
		DE_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, right));
		// Arrière
		DE_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, back));
		// Dessous
		DE_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, bottom));
		// Dessus
		DE_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, top));
	}

    /*
	===========================
	framebuffer_manager::create
	===========================
	*/
    framebuffer_id framebuffer_manager::create(const char *name)
    {
        gl_framebuffer_int fbo;
        DE_GL_CALL(glGenFramebuffers(1, &fbo));

        auto &el = m_Framebuffers.insert(name, pair(fbo, static_cast<texture_id>(0)));

        return el.key;
    }

    /*
	============================
	framebuffer_manager::rawBind
	============================
	*/
    void framebuffer_manager::rawBind(gl_framebuffer_int fbo)
    {
        DE_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

        m_CurrentlyBound = fbo;
    }

    /*
	=========================
	framebuffer_manager::bind
	=========================
	*/
    bool framebuffer_manager::bind(framebuffer_id fbo)
    {
        auto el = m_Framebuffers[fbo];
        if(el == nullptr)
            return false;

        rawBind(el->value.value1());
        m_CurrentID = fbo;

        return true;
    }

    /*
	=========================
	framebuffer_manager::bind
	=========================
	*/
    bool framebuffer_manager::bind(const char *name)
    {
        auto el = m_Framebuffers[name];
        if(el == nullptr)
            return false;

        rawBind(el->value.value1());
        m_CurrentID = el->key;

        return true;
    }

    /*
	==========================
	framebuffer_manager::check
	==========================
	*/
    bool framebuffer_manager::check()
    {
        return DE_GL_CALLV(glCheckFramebufferStatus(GL_FRAMEBUFFER)) == GL_FRAMEBUFFER_COMPLETE;
    }

    /*
	================================
	framebuffer_manager::bindDefault
	================================
	*/
    void framebuffer_manager::bindDefault()
    {
        DE_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        m_CurrentlyBound = 0;
        m_CurrentID = 0;
    }

    /*
	===============================
	framebuffer_manager::rawDestroy
	===============================
	*/
    void framebuffer_manager::rawDestroy(gl_framebuffer_int fbo)
    {
        DE_GL_CALL(glDeleteFramebuffers(1, &fbo));
    }

    /*
	============================
	framebuffer_manager::destroy
	============================
	*/
    bool framebuffer_manager::destroy(framebuffer_id fbo, bool destroyTexture)
    {
        auto el = m_Framebuffers[fbo];
        if(el == nullptr)
            return false;

        if(destroyTexture)
            texture_manager::destroy(el->value.value2());

        rawDestroy(el->value.value1());
        m_Framebuffers.remove(el->key);

        return true;
    }

    /*
	============================
	framebuffer_manager::destroy
	============================
	*/
    bool framebuffer_manager::destroy(const char *name, bool destroyTexture)
    {
        auto el = m_Framebuffers[name];
        if(el == nullptr)
            return false;

        if(destroyTexture)
            texture_manager::destroy(el->value.value2());

        rawDestroy(el->value.value1());
        m_Framebuffers.remove(el->key);

        return true;
    }

    /*
	==================================
	framebuffer_manager::attachTexture
	==================================
	*/
    bool framebuffer_manager::attachTexture(texture_id texture)
    {
        auto el = texture_manager::get(texture);
        if(el == nullptr)
            return false;

        // Attache la texture au framebuffer.
        DE_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, el->value, 0));
        m_Framebuffers[m_CurrentID]->value.value2() = texture;

        return true;
    }

    /*
	==================================
	framebuffer_manager::attachTexture
	==================================
	*/
    bool framebuffer_manager::attachTexture(const char *name)
    {
        auto el = texture_manager::get(name);
        if(el == nullptr)
            return false;

        // Attache la texture au framebuffer.
        DE_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, el->value, 0));
        m_Framebuffers[m_CurrentID]->value.value2() = el->value;

        return true;
    }

    /*
	=======================================
	framebuffer_manager::attachRenderbuffer
	=======================================
	*/
    bool framebuffer_manager::attachRenderbuffer(framebuffer_id fbo)
    {
        auto el = m_Framebuffers[fbo];
        if(el == nullptr)
            return false;

        DE_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, el->value.value1()));

        return true;
    }

    /*
	=======================================
	framebuffer_manager::attachRenderbuffer
	=======================================
	*/
    bool framebuffer_manager::attachRenderbuffer(const char *name)
    {
        auto el = m_Framebuffers[name];
        if(el == nullptr)
            return false;

        DE_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, el->value.value1()));

        return true;
    }

    /*
	============================
	renderbuffer_manager::create
	============================
	*/
    renderbuffer_id renderbuffer_manager::create(const char *name)
    {
        gl_renderbuffer_int rbo;
        DE_GL_CALL(glGenRenderbuffers(1, &rbo));

        auto &el = m_Renderbuffers.insert(name, rbo);

        return el.key;
    }

    /*
	=============================
	renderbuffer_manager::rawBind
	=============================
	*/
    void renderbuffer_manager::rawBind(gl_renderbuffer_int rbo)
    {
        DE_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
    }

    /*
	==========================
	renderbuffer_manager::bind
	==========================
	*/
    bool renderbuffer_manager::bind(renderbuffer_id rbo)
    {
        auto el = m_Renderbuffers[rbo];
        if(el == nullptr)
            return false;

        rawBind(el->value);
        m_CurrentID = rbo;

        return true;
    }

    /*
	==========================
	renderbuffer_manager::bind
	==========================
	*/
    bool renderbuffer_manager::bind(const char *name)
    {
        auto el = m_Renderbuffers[name];
        if(el == nullptr)
            return false;

        rawBind(el->value);
        m_CurrentID = el->key;

        return true;
    }

    /*
	=================================
	renderbuffer_manager::bindDefault
	=================================
	*/
    void renderbuffer_manager::bindDefault()
    {
        DE_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

        m_CurrentlyBound = 0;
        m_CurrentID = 0;
    }

    /*
	===========================
	renderbuffer_manager::store
	===========================
	*/
    void renderbuffer_manager::store(int width, int height)
    {
        DE_GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
    }

    /*
	================================
	renderbuffer_manager::rawDestroy
	================================
	*/
    void renderbuffer_manager::rawDestroy(gl_renderbuffer_int rbo)
    {
        DE_GL_CALL(glDeleteRenderbuffers(1, &rbo));
    }

    /*
	=============================
	renderbuffer_manager::destroy
	=============================
	*/
    bool renderbuffer_manager::destroy(renderbuffer_id rbo)
    {
        auto el = m_Renderbuffers[rbo];
        if(el == nullptr)
            return false;

        rawDestroy(el->value);
        m_Renderbuffers.remove(el->key);

        return true;
    }

    /*
	=============================
	renderbuffer_manager::destroy
	=============================
	*/
    bool renderbuffer_manager::destroy(const char *name)
    {
        auto el = m_Renderbuffers[name];
        if(el == nullptr)
            return false;

        rawDestroy(el->value);
        m_Renderbuffers.remove(el->key);

        return true;
    }
    /*
	=======================================
	framebuffer_manager::saveTextureAsImage
	=======================================
	*/
    bool framebuffer_manager::saveTextureAsImage(int width, int height, const char *filedest)
    {
        uint8_t *buffer = static_cast<uint8_t *>(mem::alloc(static_cast<uint64_t>(width) * height * sizeof(uint8_t) * 4));
        if(buffer == nullptr)
            return false;

        // Cette fonction permet de récupérer l'ensemble des pixels du framebuffer actif.
        // Après plusieurs tests, il semblerait que si le format est GL_RGB,
        // l'octet correspondant à la transparence est mis à 0 au lieu d'être ignoré.
        // Donc autant utiliser GL_RGBA ou GL_BGRA.
        DE_GL_CALL(glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, buffer));   // GL_BGRA est utilisé car le format BMP stock les pixels dans ce format.

        bmp image;
        if(!image.create(width, height, 32, image_color_type::RGBA))
        {
            mem::free(buffer);
            return false;
        }

        image.convertRaw(buffer);
        image.save(filedest);
        image.destroy();

        mem::free(buffer);

        return true;
    }

    /*
	=========================
	framerenderbuffer::create
	=========================
	*/
    bool framerenderbuffer::create(const char *name, int width, int height)
    {
        // Crée un framebuffer.
        m_Framebuffer = framebuffer_manager::create(name);
        framebuffer_manager::bind(m_Framebuffer);

        string strName = name;
        strName.append("_fb");

        // Alloue de la place pour stocker la texture du framebuffer.
        texture_id texture = texture_manager::create(strName.str());
        texture_manager::bind(texture, 0);
        texture_manager::allocSpace(width, height);
        texture_manager::setTextureFiltering(gl_texture_filter::Linear);

        // Indique que le framebuffer doit utiliser la place précédemment allouée.
        framebuffer_manager::attachTexture(texture);

        // Crée un renderbuffer.
        m_Renderbuffer = renderbuffer_manager::create(name);
        renderbuffer_manager::bind(m_Renderbuffer);
        renderbuffer_manager::store(width, height);     // Indique la taille du renderbuffer, a la même taille que le framebuffer car ils seront liés.
        renderbuffer_manager::bindDefault();

        // Lie le renderbuffer au framebuffer.
        framebuffer_manager::attachRenderbuffer(m_Renderbuffer);

        // Vérifie que le framebuffer a bien été créé et fonctionnel.
        bool ret = framebuffer_manager::check();

        framebuffer_manager::bindDefault();

        if(!ret)
        {
            renderbuffer_manager::destroy(m_Renderbuffer);
            texture_manager::destroy(texture);
            framebuffer_manager::destroy(m_Framebuffer);

            return false;
        }

        strName = name;
        strName.append("_vbo");
        m_VBO = vbo_manager::create(strName.str());

        string vaoName = name;
        vaoName.append("_vao");
        m_VAO = vao_manager::create(vaoName.str());

        float vPos[] =
        {
			// Positions  // Coordonnées de texture
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
		};

        memory_chunk chunk(vPos, sizeof(vPos));

        vbo_manager::bind(m_VBO);
        vao_manager::bind(m_VAO);

        vbo_manager::transmitData(chunk);
		vbo_manager::setVerticesNumber(6);

        vbo_manager::addAttribute(0, gl_attrib_components_number::x2, gl_type::Float, 4 * sizeof(float), 0);
		vbo_manager::addAttribute(1, gl_attrib_components_number::x2, gl_type::Float, 4 * sizeof(float), 2 * sizeof(float));

        vao_manager::bindDefault();
        vbo_manager::bindDefault();

        m_Width = width;
        m_Height = height;

        return true;
    }

    /*
	==========================
	framerenderbuffer::destroy
	==========================
	*/
    void framerenderbuffer::destroy()
    {
        renderbuffer_manager::destroy(m_Renderbuffer);
        framebuffer_manager::destroy(m_Framebuffer, true);
    }

    /*
	=====================================
	framerenderbuffer::saveTextureAsImage
	=====================================
	*/
    bool framerenderbuffer::saveTextureAsImage(const char *destpath)
    {
        framebuffer_id currentBuffer = framebuffer_manager::currentID();

        if(!framebuffer_manager::bind(m_Framebuffer))
            return false;

        bool ret = framebuffer_manager::saveTextureAsImage(m_Width, m_Height, destpath);

        framebuffer_manager::bind(currentBuffer);

        return ret;
    }

}
