#include <DE/rendering/opengl_utils.hpp>
#include <DE/string_utils.hpp>

#include <stdio.h>
#include <unordered_map>

#include <glad/glad.h>

namespace de {

	gl_vbo GLVBO::m_CurrentlyBound   = 0;
	gl_vao GLVAO::m_CurrentlyBound   = 0;
	gl_texture GLTexture::m_WhiteTex = 0;

	unsigned int GLProgram::m_CurrentlyBound = 0;

	static std::unordered_map<gl_vbo, int> m_VBONumberOfVertices;

	/*
	=============
	GLVBO::create
	=============
	*/
	gl_vbo GLVBO::create()
	{
		gl_vbo vbo = 0;
		DE_GL_CALL(glGenBuffers(1, &vbo));

		m_VBONumberOfVertices.emplace(vbo, 0);

		return vbo;
	}

	/*
	===========
	GLVBO::bind
	===========
	*/
	void GLVBO::bind(gl_vbo vbo)
	{
		DE_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		
		m_CurrentlyBound = vbo;
	}

	/*
	========================
	GLVBO::setVerticesNumber
	========================
	*/
	void GLVBO::setVerticesNumber(unsigned int number)
	{
		const auto &it = m_VBONumberOfVertices.find(m_CurrentlyBound);
		if(it != m_VBONumberOfVertices.end())
			it->second = number;
	}

	/*
	========================
	GLVBO::getVerticesNumber
	========================
	*/
	unsigned int GLVBO::getVerticesNumber()
	{
		return getVerticesNumber(m_CurrentlyBound);
	}

	/*
	========================
	GLVBO::getVerticesNumber
	========================
	*/
	unsigned int GLVBO::getVerticesNumber(gl_vbo vbo)
	{
		const auto &it = m_VBONumberOfVertices.find(vbo);
		if(it == m_VBONumberOfVertices.end())
			return 0;

		return it->second;
	}

	/*
	===================
	GLVBO::transmitData
	===================
	*/
	void GLVBO::transmitData(MemoryChunk &data)
	{
		DE_GL_CALL(glBufferData(GL_ARRAY_BUFFER, data.size(), data.data(), GL_STATIC_DRAW));
	}

	/*
	===================
	GLVBO::addAttribute
	===================
	*/
	void GLVBO::addAttribute(unsigned int index, GLAttribComponentsNumber::t componentsNumber, GLType::t type, int stride, int offset)
	{
		DE_GL_CALL(glVertexAttribPointer(index, componentsNumber, type, GL_FALSE, stride, (void *) offset));
		DE_GL_CALL(glEnableVertexAttribArray(index));
	}

	/*
	=============
	GLVAO::create
	=============
	*/
	gl_vao GLVAO::create()
	{
		gl_vao vao = 0;
		DE_GL_CALL(glGenVertexArrays(1, &vao));
		return vao;
	}

	/*
	===========
	GLVAO::bind
	===========
	*/
	void GLVAO::bind(gl_vao vao)
	{
		DE_GL_CALL(glBindVertexArray(vao));

		m_CurrentlyBound = vao;
	}

	/*
	=====================
	GLVAO::currentlyBound
	=====================
	*/
	unsigned int GLVAO::currentlyBound()
	{
		unsigned int val = 0;
		DE_GL_CALL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (int *) &val));
		return val;
	}

	/*
	====================
	GLUniform::GLUniform
	====================
	*/
	GLUniform::GLUniform()
		: m_Location(-1)
	{ }

	/*
	===============
	GLUniform::find
	===============
	*/
	bool GLUniform::find(unsigned int program, const char *name)
	{
		m_Location = DE_GL_CALLV(glGetUniformLocation(program, name));
		if(m_Location == -1)
			return false;

		return true;
	}

	/*
	===============
	GLUniform::send
	===============
	*/
	void GLUniform::send(float value)
	{
		DE_GL_CALL(glUniform1f(m_Location, value));
	}

	/*
	===============
	GLUniform::send
	===============
	*/
	void GLUniform::send(int value)
	{
		DE_GL_CALL(glUniform1i(m_Location, value));
	}

	/*
	===============
	GLUniform::send
	===============
	*/
	void GLUniform::send(const fvec2 &vec)
	{
		DE_GL_CALL(glUniform2f(m_Location, vec.x, vec.y));
	}

	/*
	===============
	GLUniform::send
	===============
	*/
	void GLUniform::send(fmat3x3 &mat)
	{
		DE_GL_CALL(glUniformMatrix3fv(m_Location, 1, GL_TRUE, mat.ptr()));
	}

	/*
	===============
	GLUniform::send
	===============
	*/
	void GLUniform::send(fmat4x4 &mat)
	{
		DE_GL_CALL(glUniformMatrix4fv(m_Location, 1, GL_TRUE, mat.ptr()));
	}

	/*
	================
	GLShader::create
	================
	*/
	gl_shader GLShader::create(GLShaderType::t shaderType)
	{
		return DE_GL_CALLV(glCreateShader((shaderType == GLShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER)));
	}

	/*
	==============
	GLShader::load
	==============
	*/
	void GLShader::load(gl_shader shader, MemoryChunk &program)
	{
		char *source = (char *) program.data();
		DE_GL_CALL(glShaderSource(shader, 1, &source, NULL));
	}

	/*
	=================
	GLShader::compile
	=================
	*/
	bool GLShader::compile(gl_shader shader)
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
	GLShader::destroy
	=================
	*/
	void GLShader::destroy(gl_shader shader)
	{
		DE_GL_CALL(glDeleteShader(shader));
	}

	/*
	=================
	GLProgram::create
	=================
	*/
	gl_program GLProgram::create()
	{
		return DE_GL_CALLV(glCreateProgram());
	}

	/*
	=======================
	GLProgram::attachShader
	=======================
	*/
	void GLProgram::attachShader(gl_program program, gl_shader shader)
	{
		DE_GL_CALL(glAttachShader(program, shader));
	}

	/*
	===============
	GLProgram::link
	===============
	*/
	bool GLProgram::link(gl_program program)
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
	GLProgram::use
	==============
	*/
	void GLProgram::use(gl_program program)
	{
		DE_GL_CALL(glUseProgram(program));
		m_CurrentlyBound = program;
	}

	/*
	====================
	GLError::clearErrors
	====================
	*/
	void GLError::clearErrors()
	{
		while(glGetError() != GL_NO_ERROR);
	}

	/*
	====================
	GLError::checkErrors
	====================
	*/
	bool GLError::checkErrors(const char *function, const char *file, unsigned int line)
	{
		GLenum error;
		bool ret = false;

		while((error = glGetError()) != GL_NO_ERROR) {
			fprintf(stderr, "[GLError] (%d %s) %s %s: %u\n", error, getName(error), function, file, line);
			ret = true;
		}

		return ret;	
	}

	/*
	================
	GLError::getName
	================
	*/
	const char *GLError::getName(GLenum errorCode)
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
	GLCore::version
	===============
	*/
	const char *GLCore::version()
	{
		return (const char *) glGetString(GL_VERSION);
	}

	/*
	======================
	GLCore::updateViewport
	======================
	*/
	void GLCore::updateViewport(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	/*
	========================
	GLCore::maxVertexAttribs
	========================
	*/
	int GLCore::maxVertexAttribs()
	{
		int val = 0;
		DE_GL_CALL(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &val));
		return val;
	}

	/*
	============================
	GLCore::maxTextureImageUnits
	============================
	*/
	int GLCore::maxTextureImageUnits()
	{
		int val = 0;
		DE_GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &val));
		return val;
	}

	/*
	=======================
	GLCore::enableDepthMask
	=======================
	*/
	void GLCore::enableDepthMask(bool value)
	{
		DE_GL_CALL(glDepthMask(value));
	}

	/*
	========================
	GLCore::setDepthFunction
	========================
	*/
	void GLCore::setDepthFunction(GLDepthFunction::t func)
	{
		DE_GL_CALL(glDepthFunc(func));
	}

	/*
	==============
	GLCore::create
	==============
	*/
	gl_texture GLTexture::create()
	{
		gl_texture texture = 0;
		DE_GL_CALL(glGenTextures(1, &texture));
		return texture;
	}

	/*
	===============
	GLTexture::bind
	===============
	*/
	void GLTexture::bind(gl_texture texture, uint8_t unit)
	{
		DE_GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
		DE_GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));
	}

	/*
	=======================
	GLTexture::bindCubemaps
	=======================
	*/
	void GLTexture::bindCubemaps(gl_texture texture)
	{
		DE_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, texture));
	}

	/*
	==============================
	GLTexture::setTextureWrappingS
	==============================
	*/
	void GLTexture::setTextureWrappingS(GLTextureWrap::t mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode));
	}

	/*
	==============================
	GLTexture::setTextureWrappingT
	==============================
	*/
	void GLTexture::setTextureWrappingT(GLTextureWrap::t mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode));
	}

	/*
	==============================
	GLTexture::setTextureWrappingR
	==============================
	*/
	void GLTexture::setTextureWrappingR(GLTextureWrap::t mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, mode));
	}

	/*
	==============================
	GLTexture::setTextureFiltering
	==============================
	*/
	void GLTexture::setTextureFiltering(GLTextureFilter::t mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode));
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode));
	}

	/*
	======================================
	GLTexture::setTextureWrappingSCubemaps
	======================================
	*/
	void GLTexture::setTextureWrappingSCubemaps(GLTextureWrap::t mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mode));
	}

	/*
	======================================
	GLTexture::setTextureWrappingTCubemaps
	======================================
	*/
	void GLTexture::setTextureWrappingTCubemaps(GLTextureWrap::t mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mode));
	}

	/*
	======================================
	GLTexture::setTextureWrappingRCubemaps
	======================================
	*/
	void GLTexture::setTextureWrappingRCubemaps(GLTextureWrap::t mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, mode));
	}

	/*
	======================================
	GLTexture::setTextureFilteringCubemaps
	======================================
	*/
	void GLTexture::setTextureFilteringCubemaps(GLTextureFilter::t mode)
	{
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mode));
		DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mode));
	}

	/*
	==========================
	GLTexture::transmitTexture
	==========================
	*/
	void GLTexture::transmitTexture(mem_ptr data, int width, int height, ImageColorType::t colorType)
	{
		GLenum internalFormat;
		GLenum format;
		switch(colorType) {
			default: return;
			case ImageColorType::RGB: {
				internalFormat = GL_RGB8;
				format = GL_RGB;
			}break;
			case ImageColorType::RGBA: {
				internalFormat = GL_RGBA8;
				format = GL_RGBA;
			} break;
		}

		DE_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data));
	}

	/*
	==================================
	GLTexture::transmitTextureCubemaps
	==================================
	*/
	void GLTexture::transmitTextureCubemaps(mem_ptr left, mem_ptr front, mem_ptr right, mem_ptr back, mem_ptr bottom, mem_ptr top, int width, int height, ImageColorType::t colorType)
	{
		GLenum internalFormat;
		GLenum format;
		switch(colorType) {
			default: return;
			case ImageColorType::RGB: {
				internalFormat = GL_RGB8;
				format = GL_RGB;
			}break;
			case ImageColorType::RGBA: {
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

}
