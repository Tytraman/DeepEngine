#include <DE/rendering/opengl_utils.hpp>
#include <DE/string_utils.hpp>

#include <stdio.h>

#include <glad/glad.h>

namespace de {

	unsigned int GLProgram::m_CurrentlyBound = 0;

	/*
	============
	GLVBO::GLVBO
	============
	*/
	GLVBO::GLVBO()
		: m_VerticesNumber(0)
	{
		m_VBO = 0;
		DE_GL_CALL(glGenBuffers(1, &m_VBO));
	}

	/*
	===========
	GLVBO::bind
	===========
	*/
	void GLVBO::bind()
	{
		DE_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
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
	=====================
	GLVBO::currentlyBound
	=====================
	*/
	unsigned int GLVBO::currentlyBound()
	{
		unsigned int val = 0;
		DE_GL_CALL(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (int *) &val));
		return val;
	}

	/*
	============
	GLVAO::GLVAO
	============
	*/
	GLVAO::GLVAO()
	{
		m_VAO = 0;
		DE_GL_CALL(glGenVertexArrays(1, &m_VAO));
	}

	/*
	===========
	GLVAO::bind
	===========
	*/
	void GLVAO::bind()
	{
		DE_GL_CALL(glBindVertexArray(m_VAO));
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
	==================
	GLShader::GLShader
	==================
	*/
	GLShader::GLShader(GLShaderType::t shaderType)
		: m_Shader(DE_GL_CALLV(glCreateShader((shaderType == GLShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER))))
	{ }

	/*
	==============
	GLShader::load
	==============
	*/
	void GLShader::load(MemoryChunk &program)
	{
		char *source = (char *) program.data();
		DE_GL_CALL(glShaderSource(m_Shader, 1, &source, NULL));
	}

	/*
	=================
	GLShader::compile
	=================
	*/
	bool GLShader::compile()
	{
		int  success;
		char infoLog[512];

		DE_GL_CALL(glCompileShader(m_Shader));
		DE_GL_CALL(glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &success));

		if(!success) {
			DE_GL_CALL(glGetShaderInfoLog(m_Shader, 512, NULL, infoLog));
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
	void GLShader::destroy()
	{
		DE_GL_CALL(glDeleteShader(m_Shader));
	}

	/*
	====================
	GLProgram::GLProgram
	====================
	*/
	GLProgram::GLProgram()
		: m_Program(DE_GL_CALLV(glCreateProgram()))
	{ }

	/*
	=======================
	GLProgram::attachShader
	=======================
	*/
	void GLProgram::attachShader(const GLShader &shader)
	{
		DE_GL_CALL(glAttachShader(m_Program, shader.id()));
	}

	/*
	===============
	GLProgram::link
	===============
	*/
	bool GLProgram::link() const
	{
		int  success;
		char infoLog[512];

		DE_GL_CALL(glLinkProgram(m_Program));
		DE_GL_CALL(glGetProgramiv(m_Program, GL_LINK_STATUS, &success));

		if(!success) {
			DE_GL_CALL(glGetProgramInfoLog(m_Program, sizeof(infoLog), NULL, infoLog));
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
	void GLProgram::use() const
	{
		DE_GL_CALL(glUseProgram(m_Program));
		m_CurrentlyBound = m_Program;
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

	const char *GLCore::version()
	{
		return (const char *) glGetString(GL_VERSION);
	}

	int GLCore::maxVertexAttribs()
	{
		int val = 0;
		DE_GL_CALL(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &val));
		return val;
	}

	void GLCore::updateViewport(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

}
