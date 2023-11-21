#include <DE/rendering/opengl_utils.hpp>
#include <DE/string_utils.hpp>

#include <stdio.h>
#include <unordered_map>

#include <glad/glad.h>

namespace de
{

	void programs_hashtable_free_element_callback(gl_program_int &program)
	{
		gl_program::destroy(program);
	}

	gl_vbo_int gl_vbo::m_CurrentlyBound   = 0;
	gl_vao_int gl_vao::m_CurrentlyBound   = 0;

	gl_texture_int gl_texture::m_WhiteTex = 0;
	gl_texture_int gl_texture::m_CurrentlyBound = 0;
	uint8_t gl_texture::m_CurrentUnit = 0;

	unsigned int gl_program::m_CurrentlyBound = 0;
	hash_table<gl_program_int> gl_program::m_Programs(100, string::hash, programs_hashtable_free_element_callback);

	static std::unordered_map<gl_vbo_int, int> m_VBONumberOfVertices;

	

	/*
	=============
	gl_vbo::create
	=============
	*/
	gl_vbo_int gl_vbo::create()
	{
		gl_vbo_int vbo = 0;
		DE_GL_CALL(glGenBuffers(1, &vbo));

		m_VBONumberOfVertices.emplace(vbo, 0);

		return vbo;
	}

	/*
	===========
	gl_vbo::bind
	===========
	*/
	void gl_vbo::bind(gl_vbo_int vbo)
	{
		DE_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		
		m_CurrentlyBound = vbo;
	}

	/*
	==============
	gl_vbo::destroy
	==============
	*/
	void gl_vbo::destroy(gl_vbo_int vbo)
	{
		DE_GL_CALL(glDeleteBuffers(1, &vbo));
	}

	/*
	========================
	gl_vbo::setVerticesNumber
	========================
	*/
	void gl_vbo::setVerticesNumber(unsigned int number)
	{
		const auto &it = m_VBONumberOfVertices.find(m_CurrentlyBound);
		if(it != m_VBONumberOfVertices.end())
			it->second = number;
	}

	/*
	========================
	gl_vbo::getVerticesNumber
	========================
	*/
	unsigned int gl_vbo::getVerticesNumber()
	{
		return getVerticesNumber(m_CurrentlyBound);
	}

	/*
	========================
	gl_vbo::getVerticesNumber
	========================
	*/
	unsigned int gl_vbo::getVerticesNumber(gl_vbo_int vbo)
	{
		const auto &it = m_VBONumberOfVertices.find(vbo);
		if(it == m_VBONumberOfVertices.end())
			return 0;

		return it->second;
	}

	/*
	===================
	gl_vbo::transmitData
	===================
	*/
	void gl_vbo::transmitData(memory_chunk &data)
	{
		DE_GL_CALL(glBufferData(GL_ARRAY_BUFFER, data.size(), data.data(), GL_STATIC_DRAW));
	}

	/*
	===================
	gl_vbo::addAttribute
	===================
	*/
	void gl_vbo::addAttribute(unsigned int index, gl_attrib_components_number componentsNumber, gl_type type, int stride, int offset)
	{
		DE_GL_CALL(glVertexAttribPointer(index, static_cast<GLint>(componentsNumber), static_cast<GLenum>(type), GL_FALSE, stride, (void *) offset));
		DE_GL_CALL(glEnableVertexAttribArray(index));
	}

	/*
	=============
	gl_vao::create
	=============
	*/
	gl_vao_int gl_vao::create()
	{
		gl_vao_int vao = 0;
		DE_GL_CALL(glGenVertexArrays(1, &vao));
		return vao;
	}

	/*
	===========
	gl_vao::bind
	===========
	*/
	void gl_vao::bind(gl_vao_int vao)
	{
		DE_GL_CALL(glBindVertexArray(vao));

		m_CurrentlyBound = vao;
	}

	/*
	==============
	gl_vao::destroy
	==============
	*/
	void gl_vao::destroy(gl_vao_int vao)
	{
		DE_GL_CALL(glDeleteVertexArrays(1, &vao));
	}

	/*
	====================
	gl_uniform::gl_uniform
	====================
	*/
	gl_uniform::gl_uniform()
		: m_Location(-1)
	{ }

	/*
	===============
	gl_uniform::find
	===============
	*/
	bool gl_uniform::find(unsigned int program, const char *name)
	{
		m_Location = DE_GL_CALLV(glGetUniformLocation(program, name));
		if(m_Location == -1)
			return false;

		return true;
	}

	/*
	===============
	gl_uniform::send
	===============
	*/
	void gl_uniform::send(float value)
	{
		DE_GL_CALL(glUniform1f(m_Location, value));
	}

	/*
	===============
	gl_uniform::send
	===============
	*/
	void gl_uniform::send(int value)
	{
		DE_GL_CALL(glUniform1i(m_Location, value));
	}

	/*
	===============
	gl_uniform::send
	===============
	*/
	void gl_uniform::send(const fvec2 &vec)
	{
		DE_GL_CALL(glUniform2f(m_Location, vec.x, vec.y));
	}

	/*
	===============
	gl_uniform::send
	===============
	*/
	void gl_uniform::send(const fvec3 &vec)
	{
		DE_GL_CALL(glUniform3f(m_Location, vec.x, vec.y, vec.z));
	}

	/*
	===============
	gl_uniform::send
	===============
	*/
	void gl_uniform::send(fmat3x3 &mat)
	{
		DE_GL_CALL(glUniformMatrix3fv(m_Location, 1, GL_TRUE, mat.ptr()));
	}

	/*
	===============
	gl_uniform::send
	===============
	*/
	void gl_uniform::send(fmat4x4 &mat)
	{
		DE_GL_CALL(glUniformMatrix4fv(m_Location, 1, GL_TRUE, mat.ptr()));
	}

	/*
	================
	gl_shader::create
	================
	*/
	gl_shader_int gl_shader::create(gl_shader_type shaderType)
	{
		return DE_GL_CALLV(glCreateShader((shaderType == gl_shader_type::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER)));
	}

	/*
	==============
	gl_shader::load
	==============
	*/
	void gl_shader::load(gl_shader_int shader, memory_chunk &program)
	{
		char *source = (char *) program.data();
		DE_GL_CALL(glShaderSource(shader, 1, &source, NULL));
	}

	/*
	=================
	gl_shader::compile
	=================
	*/
	bool gl_shader::compile(gl_shader_int shader)
	{
		int  success;
		char infoLog[512];

		DE_GL_CALL(glCompileShader(shader));
		DE_GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));

		if(!success) {
			DE_GL_CALL(glGetShaderInfoLog(shader, 512, NULL, infoLog));
			fprintf(stderr, "GLSL compilation error:\n%s\n", infoLog);

			return false;
		}

		return true;
	}

	/*
	=================
	gl_shader::destroy
	=================
	*/
	void gl_shader::destroy(gl_shader_int shader)
	{
		DE_GL_CALL(glDeleteShader(shader));
	}

	/*
	=================
	gl_program::create
	=================
	*/
	gl_program_int gl_program::create(const char *name)
	{
		gl_program_int program = DE_GL_CALLV(glCreateProgram());

		m_Programs.insert(name, program);

		return program;
	}

	/*
	==============
	gl_program::get
	==============
	*/
	gl_program_int gl_program::get(const char *name)
	{
		const auto hs = m_Programs[name];
		if(hs == nullptr)
			return 0;

		return hs->value;
	}

	/*
	=======================
	gl_program::attachShader
	=======================
	*/
	void gl_program::attachShader(gl_program_int program, gl_shader_int shader)
	{
		DE_GL_CALL(glAttachShader(program, shader));
	}

	/*
	===============
	gl_program::link
	===============
	*/
	bool gl_program::link(gl_program_int program)
	{
		int  success;
		char infoLog[512];

		DE_GL_CALL(glLinkProgram(program));
		DE_GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &success));

		if(!success) {
			DE_GL_CALL(glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog));
			fprintf(stderr, "GLSL link error:\n%s\n", infoLog);

			return false;
		}

		return true;
	}

	/*
	==============
	gl_program::use
	==============
	*/
	void gl_program::use(gl_program_int program)
	{
		DE_GL_CALL(glUseProgram(program));
		m_CurrentlyBound = program;
	}

	/*
	==================
	gl_program::destroy
	==================
	*/
	void gl_program::destroy(gl_program_int program)
	{
		m_Programs.remove(program);
		DE_GL_CALL(glDeleteProgram(program));
	}

	

	/*
	====================
	gl_error::clearErrors
	====================
	*/
	void gl_error::clearErrors()
	{
		while(glGetError() != GL_NO_ERROR);
	}

	/*
	====================
	gl_error::checkErrors
	====================
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
	================
	gl_error::getName
	================
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
	===============
	gl_core::version
	===============
	*/
	const char *gl_core::version()
	{
		return (const char *) glGetString(GL_VERSION);
	}

	/*
	======================
	gl_core::updateViewport
	======================
	*/
	void gl_core::updateViewport(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	/*
	========================
	gl_core::maxVertexAttribs
	========================
	*/
	int gl_core::maxVertexAttribs()
	{
		int val = 0;
		DE_GL_CALL(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &val));
		return val;
	}

	/*
	============================
	gl_core::maxTextureImageUnits
	============================
	*/
	int gl_core::maxTextureImageUnits()
	{
		int val = 0;
		DE_GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &val));
		return val;
	}

	/*
	=======================
	gl_core::enableDepthMask
	=======================
	*/
	void gl_core::enableDepthMask(bool value)
	{
		DE_GL_CALL(glDepthMask(value));
	}

	/*
	========================
	gl_core::setDepthFunction
	========================
	*/
	void gl_core::setDepthFunction(gl_depth_function func)
	{
		DE_GL_CALL(glDepthFunc(static_cast<GLenum>(func)));
	}

	/*
	===================
	gl_core::setCullFace
	===================
	*/
	void gl_core::setCullFace(gl_cull_face cullFace)
	{
		DE_GL_CALL(glCullFace(static_cast<GLenum>(cullFace)));
	}

	/*
	=================
	gl_texture::create
	=================
	*/
	gl_texture_int gl_texture::create()
	{
		gl_texture_int texture = 0;
		DE_GL_CALL(glGenTextures(1, &texture));
		return texture;
	}

	/*
	===============
	gl_texture::bind
	===============
	*/
	void gl_texture::bind(gl_texture_int texture, uint8_t unit)
	{
		DE_GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
		DE_GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));

		m_CurrentlyBound = texture;
		m_CurrentUnit = unit;
	}

	/*
	=======================
	gl_texture::bindCubemaps
	=======================
	*/
	void gl_texture::bindCubemaps(gl_texture_int texture)
	{
		DE_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, texture));

		m_CurrentlyBound = texture;
	}

	/*
	==============================
	gl_texture::setTextureWrappingS
	==============================
	*/
	void gl_texture::setTextureWrappingS(gl_texture_wrap mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(mode)));
	}

	/*
	==============================
	gl_texture::setTextureWrappingT
	==============================
	*/
	void gl_texture::setTextureWrappingT(gl_texture_wrap mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(mode)));
	}

	/*
	==============================
	gl_texture::setTextureWrappingR
	==============================
	*/
	void gl_texture::setTextureWrappingR(gl_texture_wrap mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, static_cast<GLint>(mode)));
	}

	/*
	==============================
	gl_texture::setTextureFiltering
	==============================
	*/
	void gl_texture::setTextureFiltering(gl_texture_filter mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(mode)));
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mode)));
	}

	/*
	======================================
	gl_texture::setTextureWrappingSCubemaps
	======================================
	*/
	void gl_texture::setTextureWrappingSCubemaps(gl_texture_wrap mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<GLint>(mode)));
	}

	/*
	======================================
	gl_texture::setTextureWrappingTCubemaps
	======================================
	*/
	void gl_texture::setTextureWrappingTCubemaps(gl_texture_wrap mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<GLint>(mode)));
	}

	/*
	======================================
	gl_texture::setTextureWrappingRCubemaps
	======================================
	*/
	void gl_texture::setTextureWrappingRCubemaps(gl_texture_wrap mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<GLint>(mode)));
	}

	/*
	======================================
	gl_texture::setTextureFilteringCubemaps
	======================================
	*/
	void gl_texture::setTextureFilteringCubemaps(gl_texture_filter mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(mode)));
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mode)));
	}

	/*
	==========================
	gl_texture::transmitTexture
	==========================
	*/
	void gl_texture::transmitTexture(mem_ptr data, int width, int height, image_color_type colorType)
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
	===================================
	gl_texture::transmitTextureCubemaps
	===================================
	*/
	void gl_texture::transmitTextureCubemaps(mem_ptr left, mem_ptr front, mem_ptr right, mem_ptr back, mem_ptr bottom, mem_ptr top, int width, int height, image_color_type colorType)
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
	======================
	gl_framebuffer::create
	======================
	*/
    gl_framebuffer_int gl_framebuffer::create()
    {
        gl_framebuffer_int fbo;
        DE_GL_CALL(glGenFramebuffers(1, &fbo));

        return fbo;
    }

    /*
	====================
	gl_framebuffer::bind
	====================
	*/
    void gl_framebuffer::bind(gl_framebuffer_int fbo)
    {
        DE_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
    }

    /*
	=====================
	gl_framebuffer::check
	=====================
	*/
    bool gl_framebuffer::check()
    {
        return DE_GL_CALLV(glCheckFramebufferStatus(GL_FRAMEBUFFER)) == GL_FRAMEBUFFER_COMPLETE;
    }

    /*
	===========================
	gl_framebuffer::bindDefault
	===========================
	*/
    void gl_framebuffer::bindDefault()
    {
        DE_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    /*
	=======================
	gl_framebuffer::destroy
	=======================
	*/
    void gl_framebuffer::destroy(gl_framebuffer_int fbo)
    {
        DE_GL_CALL(glDeleteFramebuffers(1, &fbo));
    }

    /*
	=============================
	gl_framebuffer::attachTexture
	=============================
	*/
    void gl_framebuffer::attachTexture(int width, int height)
    {
        unsigned int texture;

        // Génère une nouvelle texture.
        DE_GL_CALL(glGenTextures(1, &texture));
        DE_GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));

        // Crée la texture.
        DE_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));

        DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        // Attache la texture au framebuffer.
        DE_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0));
    }

    /*
	==================================
	gl_framebuffer::attachRenderbuffer
	==================================
	*/
    void gl_framebuffer::attachRenderbuffer(gl_renderbuffer_int rbo)
    {
        DE_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
    }

    /*
	=======================
	gl_renderbuffer::create
	=======================
	*/
    gl_renderbuffer_int gl_renderbuffer::create()
    {
        gl_renderbuffer_int rbo;
        DE_GL_CALL(glGenRenderbuffers(1, &rbo));

        return rbo;
    }

    /*
	=====================
	gl_renderbuffer::bind
	=====================
	*/
    void gl_renderbuffer::bind(gl_renderbuffer_int rbo)
    {
        DE_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
    }

    /*
	============================
	gl_renderbuffer::bindDefault
	============================
	*/
    void gl_renderbuffer::bindDefault()
    {
        DE_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    }

    /*
	======================
	gl_renderbuffer::store
	======================
	*/
    void gl_renderbuffer::store(int width, int height)
    {
        DE_GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
    }

    /*
	============================
	gl_framerenderbuffer::create
	============================
	*/
    bool gl_framerenderbuffer::create(int width, int height)
    {
        m_Framebuffer = gl_framebuffer::create();
        gl_framebuffer::bind(m_Framebuffer);
        gl_framebuffer::attachTexture(width, height);

        m_Renderbuffer = gl_renderbuffer::create();
        gl_renderbuffer::bind(m_Renderbuffer);
        gl_renderbuffer::store(width, height);
        gl_renderbuffer::bindDefault();

        gl_framebuffer::attachRenderbuffer(m_Renderbuffer);

        bool ret = gl_framebuffer::check();

        gl_framebuffer::bindDefault();

        return ret;
    }

}
