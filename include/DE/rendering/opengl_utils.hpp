#ifndef __DEEP_ENGINE_OPENGL_UTILS_HPP__
#define __DEEP_ENGINE_OPENGL_UTILS_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>
#include <DE/vec.hpp>
#include <DE/mat.hpp>

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

	/// @brief Vertex Buffer Object
	class DE_API GLVBO {

		public:
			GLVBO();

			void bind();
			void addAttribute(unsigned int index, GLAttribComponentsNumber::t componentsNumber, GLType::t type, int stride, int offset);

			static void transmitData(MemoryChunk &data);

			void setVerticesNumber(unsigned int number);

			/// @brief  Récupère l'ID du VBO.
			/// @return L'ID du VBO.
			unsigned int id() const;

			/// @brief  Récupère le nombre de sommets que le VBO possède.
			/// @return Le nombre de sommets que le VBO possède.
			unsigned int verticesNumber() const;

			/// @brief  Demande à OpenGL quel est le VBO actuellement lié.
			/// @return L'ID du VBO actuellement lié.
			static unsigned int currentlyBound();

		private:
			unsigned int m_VBO;
			unsigned int m_VerticesNumber;

	};

	/*
	========================
	GLVBO::setVerticesNumber
	========================
	*/
	inline void GLVBO::setVerticesNumber(unsigned int number)
	{
		m_VerticesNumber = number;
	}

	/*
	=========
	GLVBO::id
	=========
	*/
	inline unsigned int GLVBO::id() const
	{
		return m_VBO;
	}

	/*
	=====================
	GLVBO::verticesNumber
	=====================
	*/
	inline unsigned int GLVBO::verticesNumber() const
	{
		return m_VerticesNumber;
	}

	class DE_API GLVAO {

		public:
			GLVAO();

			void bind();

			unsigned int id() const;

			static unsigned int currentlyBound();

		private:
			unsigned int m_VAO;

	};

	/*
	=========
	GLVAO::id
	=========
	*/
	inline unsigned int GLVAO::id() const
	{
		return m_VAO;
	}

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
			void send(const fvec2 &vec);
			void send(fmat3x3 &mat);
			void send(fmat4x4 &mat);

		private:
			int m_Location;

	};

	inline bool GLUniform::find(const GLProgram &program, const char *name)
	{
		return find(program.id(), name);
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

			/// @brief        Met à jour le viewport d'OpenGL.
			/// @param width  Largeur du viewport.
			/// @param height Hauteur du viewport.
			static void updateViewport(int width, int height);

	};

}


#endif