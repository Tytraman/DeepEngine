#ifndef __DEEP_ENGINE_OPENGL_UTILS_HPP__
#define __DEEP_ENGINE_OPENGL_UTILS_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>
#include <DE/vec.hpp>
#include <DE/mat.hpp>
#include <DE/image/image.hpp>

#include <glad/glad.h>

namespace de {

	/*
			La notion la plus importante à comprendre avec OpenGL est sa façon de fonctionner : c'est une machine à états.
	
			On défini un objet comme étant actif ce qui implique que les futurs appels de fonctions agiront sur cet objet.
	*/


#define DE_GL_CALLV(_x)                                                          \
	[&]() {                                                                      \
		de::GLError::clearErrors();                                              \
		auto ret = _x;                                                           \
		if(de::GLError::checkErrors(#_x, DE_FILE_NAME, DE_LINE)) DE_DEBUG_BREAK; \
		return ret;                                                              \
	}()

#define DE_GL_CALL(_x)                                                       \
	de::GLError::clearErrors();                                              \
	_x;                                                                      \
	if(de::GLError::checkErrors(#_x, DE_FILE_NAME, DE_LINE)) DE_DEBUG_BREAK;


	namespace GLAttribComponentsNumber {
		enum t : int {
			x1 = 1,
			x2 = 2,
			x3 = 3,
			x4 = 4
		};
	}

	namespace GLType {
		enum t {
			Byte  = GL_UNSIGNED_BYTE,
			Float = GL_FLOAT,
			Int   = GL_INT,
			UInt  = GL_UNSIGNED_INT
		};
	}

	namespace GLTextureWrap {
		enum t : int {
			Repeat         = GL_REPEAT,
			MirroredRepeat = GL_MIRRORED_REPEAT,
			ClampToEdge    = GL_CLAMP_TO_EDGE,
			ClampToBorder  = GL_CLAMP_TO_BORDER
		};
	}

	namespace GLTextureFilter {
		enum t : int {
			Nearest = GL_NEAREST,
			Linear  = GL_LINEAR
		};
	}

	using gl_vbo = unsigned int;

	/// @brief Vertex Buffer Object
	class DE_API GLVBO {

		public:
			
			static gl_vbo create();
			static void bind(gl_vbo vbo);
			static void addAttribute(unsigned int index, GLAttribComponentsNumber::t componentsNumber, GLType::t type, int stride, int offset);
			static void setVerticesNumber(unsigned int number);

			static void transmitData(MemoryChunk &data);

			/// @brief  Récupère le nombre de sommets que le VBO possède.
			/// @return Le nombre de sommets que le VBO possède.
			static unsigned int getVerticesNumber();

			static unsigned int getVerticesNumber(gl_vbo vbo);

			/// @brief  Demande à OpenGL quel est le VBO actuellement lié.
			/// @return L'ID du VBO actuellement lié.
			static unsigned int currentlyBound();

		private:
			static gl_vbo m_CurrentlyBound;

			GLVBO() = delete;

	};

	inline unsigned int GLVBO::currentlyBound()
	{
		return m_CurrentlyBound;
	}

	using gl_vao = unsigned int;

	class DE_API GLVAO {

		public:

			static gl_vao create();
			static void bind(gl_vao vao);

			static unsigned int currentlyBound();

		private:
			static gl_vao m_CurrentlyBound;

			GLVAO() = delete;

	};

	namespace GLShaderType {

		enum t : uint8_t {
			Vertex,
			Fragment
		};

	}

	class DE_API GLShader {

		public:
			GLShader(GLShaderType::t shaderType);

			void load(MemoryChunk &program);
			bool compile();
			void destroy();

			unsigned int id() const;

		private:
			unsigned int m_Shader;

	};

	/*
	============
	GLShader::id
	============
	*/
	inline unsigned int GLShader::id() const
	{
		return m_Shader;
	}

	class DE_API GLProgram {

		public:
			GLProgram();

			void attachShader(const GLShader &shader);
			bool link() const;
			void use() const;

			unsigned int id() const;
			static unsigned int currentlyBound();

		private:
			unsigned int m_Program;

			static unsigned int m_CurrentlyBound;

	};

	/*
	=============
	GLProgram::id
	=============
	*/
	inline unsigned int GLProgram::id() const
	{
		return m_Program;
	}

	/*
	=========================
	GLProgram::currentlyBound
	=========================
	*/
	inline unsigned int GLProgram::currentlyBound()
	{
		return m_CurrentlyBound;
	}

	class DE_API GLUniform {

		public:
			GLUniform();

			bool find(unsigned int program, const char *name);
			bool find(const GLProgram &program, const char *name);

			void send(float value);
			void send(int value);
			void send(const fvec2 &vec);
			void send(fmat3x3 &mat);
			void send(fmat4x4 &mat);

		private:
			int m_Location;

	};

	/*
	===============
	GLUniform::find
	===============
	*/
	inline bool GLUniform::find(const GLProgram &program, const char *name)
	{
		return find(program.id(), name);
	}

	using gl_texture = unsigned int;

	class DE_API GLTexture {

		public:
			static gl_texture create();
			static void bind(gl_texture texture, uint8_t unit);

			static void setTextureWrappingS(GLTextureWrap::t mode);
			static void setTextureWrappingT(GLTextureWrap::t mode);
			static void setTextureFiltering(GLTextureFilter::t mode);

			static void transmitTexture(mem_ptr data, int width, int height, ImageColorType::t colorType);

			static gl_texture getWhiteTexture();
			static void setWhiteTexture(gl_texture texture);

		private:
			static gl_texture m_WhiteTex;

			GLTexture() = delete;

	};

	inline gl_texture GLTexture::getWhiteTexture()
	{
		return m_WhiteTex;
	}

	inline void GLTexture::setWhiteTexture(gl_texture texture)
	{
		m_WhiteTex = texture;
	}

	class DE_API GLError {

		public:
			
			/// @brief Nettoie toutes les erreurs qu'OpenGL stock en interne.
			static void clearErrors();

			/// @brief          Vérifie si OpenGL a détecté une ou plusieurs erreurs.
			/// @param function Nom de la fonction OpenGL qui a généré l'erreur.
			/// @param file     Nom du fichier dans lequel s'est produit l'erreur.
			/// @param line     Ligne à laquelle s'est produit l'erreur.
			/// @return         \c true si une ou plusieurs erreurs se sont produites.
			static bool checkErrors(const char *function, const char *file, unsigned int line);

			/// @brief           Retourne une chaîne de caractères du nom de l'erreur OpenGL.
			/// @param errorCode Code d'erreur OpenGL.
			/// @return          Une chaîne de caractères constante du nom de l'erreur.
			static const char *getName(GLenum errorCode);

	};

	

	class DE_API GLCore {

		public:

			/// @brief  Récupère le nom de la version OpenGL actuellement utilisée.
			/// @return Une chaîne de caractères constante du nom de la version OpenGL utilisée.
			static const char *version();

			/// @brief  Récupère le nombre d'attributs de vertex maximal gérée par la carte graphique.
			/// @return Le nombre d'attributs de vertex maximal gérée par la carte graphique.
			static int maxVertexAttribs();

			static int maxTextureImageUnits();

			/// @brief        Met à jour le viewport d'OpenGL.
			/// @param width  Largeur du viewport.
			/// @param height Hauteur du viewport.
			static void updateViewport(int width, int height);

	};

}


#endif