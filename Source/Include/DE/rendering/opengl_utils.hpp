#ifndef __DEEP_ENGINE_OPENGL_UTILS_HPP__
#define __DEEP_ENGINE_OPENGL_UTILS_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>
#include <DE/vec.hpp>
#include <DE/mat.hpp>
#include <DE/image/image.hpp>
#include <DE/memory/hash_table.hpp>


#include <glad/glad.h>

namespace de
{

	/*
			La notion la plus importante à comprendre avec OpenGL est sa façon de fonctionner : c'est une machine à états.
	
			On défini un objet comme étant actif ce qui implique que les futurs appels de fonctions agiront sur cet objet.
	*/


#define DE_GL_CALLV(_x)                                                          \
	[&]() {                                                                      \
		de::gl_error::clearErrors();                                              \
		auto ret = _x;                                                           \
		if(de::gl_error::checkErrors(#_x, DE_FILE_NAME, DE_LINE)) DE_DEBUG_BREAK; \
		return ret;                                                              \
	}()

#define DE_GL_CALL(_x)                                                       \
	de::gl_error::clearErrors();                                              \
	_x;                                                                      \
	if(de::gl_error::checkErrors(#_x, DE_FILE_NAME, DE_LINE)) DE_DEBUG_BREAK;

    using gl_vbo_int          = unsigned int;
    using gl_vao_int          = unsigned int;
    using gl_shader_int       = unsigned int;
    using gl_program_int      = unsigned int;
    using gl_texture_int      = unsigned int;
    using gl_framebuffer_int  = unsigned int;
    using gl_renderbuffer_int = unsigned int;

    class gl_vbo;
    class gl_vao;
    class gl_shader;
    class gl_program;
    class gl_texture;
    class gl_framebuffer;
    class gl_renderbuffer;
    class gl_framerenderbuffer;

    class window;

	
	enum class gl_attrib_components_number : int
    {
		x1 = 1,
		x2 = 2,
		x3 = 3,
		x4 = 4
	};

	enum class gl_type {
		Byte  = GL_UNSIGNED_BYTE,
		Float = GL_FLOAT,
		Int   = GL_INT,
		UInt  = GL_UNSIGNED_INT
	};

	
	enum class gl_texture_wrap : int
    {
		Repeat         = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		ClampToEdge    = GL_CLAMP_TO_EDGE,
		ClampToBorder  = GL_CLAMP_TO_BORDER
	};

	enum class gl_texture_filter : int
    {
		Nearest = GL_NEAREST,
		Linear  = GL_LINEAR
	};

	enum class gl_depth_function
    {
		Always   = GL_ALWAYS,
		Never    = GL_NEVER,
		Less     = GL_LESS,
		Equal    = GL_EQUAL,
		Lequal   = GL_LEQUAL,
		Greater  = GL_GREATER,
		NotEqual = GL_NOTEQUAL,
		Gequal   = GL_GEQUAL
	};

	enum class gl_cull_face
    {
		Back         = GL_BACK,
		Front        = GL_FRONT,
		FrontAndBack = GL_FRONT_AND_BACK
	};

    enum class gl_shader_type : uint8_t
    {
		Vertex,
		Fragment
	};

	/// @brief Vertex Buffer Object
	class DE_API gl_vbo
    {

		public:
            gl_vbo() = delete;
			
			static gl_vbo_int create();
			static void bind(gl_vbo_int vbo);
			static void destroy(gl_vbo_int vbo);
			static void addAttribute(unsigned int index, gl_attrib_components_number componentsNumber, gl_type type, int stride, int offset);
			static void setVerticesNumber(unsigned int number);

			static void transmitData(memory_chunk &data);

			/// @brief  Récupère le nombre de sommets que le VBO possède.
			/// @return Le nombre de sommets que le VBO possède.
			static unsigned int getVerticesNumber();

			static unsigned int getVerticesNumber(gl_vbo_int vbo);

			/// @brief  Demande à OpenGL quel est le VBO actuellement lié.
			/// @return L'ID du VBO actuellement lié.
			static unsigned int currentlyBound();

		private:
			static gl_vbo_int m_CurrentlyBound;

	};

	/*
	=====================
	gl_vbo::currentlyBound
	=====================
	*/
	inline unsigned int gl_vbo::currentlyBound()
	{
		return m_CurrentlyBound;
	}

	class DE_API gl_vao
    {

		public:
			static gl_vao_int create();
			static void bind(gl_vao_int vao);
			static void destroy(gl_vao_int vao);

			static gl_vao_int currentlyBound();

		private:
			static gl_vao_int m_CurrentlyBound;

        public:
            gl_vao() = delete;
	};

	/*
	=====================
	gl_vao::currentlyBound
	=====================
	*/
	inline gl_vao_int gl_vao::currentlyBound()
	{
		return m_CurrentlyBound;
	}

	class DE_API gl_shader
    {

		public:
			static gl_shader_int create(gl_shader_type shaderType);

			static void load(gl_shader_int shader, memory_chunk &program);
			static bool compile(gl_shader_int shader);
			static void destroy(gl_shader_int shader);

        public:
            gl_shader() = delete;
	};

	class DE_API gl_program
    {

		public:
			static gl_program_int create(const char *name);
			static gl_program_int get(const char *name);

			static void attachShader(gl_program_int program, gl_shader_int shader);
			static bool link(gl_program_int program);
			static void use(gl_program_int program);
			static void destroy(gl_program_int program);

			static void destroyAllPrograms();

			static gl_program_int currentlyBound();

		private:
			static gl_program_int m_CurrentlyBound;
			static hash_table<gl_program_int> m_Programs;

        public:
            gl_program() = delete;

	};

	/*
	=============================
	gl_program::destroyAllPrograms
	=============================
	*/
	inline void gl_program::destroyAllPrograms()
	{
		m_Programs.clear();
	}

	/*
	=========================
	gl_program::currentlyBound
	=========================
	*/
	inline gl_program_int gl_program::currentlyBound()
	{
		return m_CurrentlyBound;
	}

	class DE_API gl_uniform
    {

		public:
			gl_uniform();

			bool find(gl_program_int program, const char *name);

			void send(float value);
			void send(int value);
			void send(const fvec2 &vec);
			void send(const fvec3 &vec);
			void send(fmat3x3 &mat);
			void send(fmat4x4 &mat);

		private:
			int m_Location;

	};

	class DE_API gl_texture
    {

		public:
			static gl_texture_int create();
			static void bind(gl_texture_int texture, uint8_t unit);
			static void bindCubemaps(gl_texture_int texture);

			static void setTextureWrappingS(gl_texture_wrap mode);
			static void setTextureWrappingT(gl_texture_wrap mode);
			static void setTextureWrappingR(gl_texture_wrap mode);
			static void setTextureFiltering(gl_texture_filter mode);

			static void setTextureWrappingSCubemaps(gl_texture_wrap mode);
			static void setTextureWrappingTCubemaps(gl_texture_wrap mode);
			static void setTextureWrappingRCubemaps(gl_texture_wrap mode);
			static void setTextureFilteringCubemaps(gl_texture_filter mode);

			static void transmitTexture(mem_ptr data, int width, int height, image_color_type colorType);
			static void transmitTextureCubemaps(mem_ptr left, mem_ptr front, mem_ptr right, mem_ptr back, mem_ptr bottom, mem_ptr top, int width, int height, image_color_type colorType);

			static gl_texture_int getWhiteTexture();
			static void setWhiteTexture(gl_texture_int texture);

			static gl_texture_int currentlyBound();
			static uint8_t currentUnit();

		private:
			static gl_texture_int m_WhiteTex;

			static gl_texture_int m_CurrentlyBound;
			static uint8_t m_CurrentUnit;

        public:
			gl_texture() = delete;

	};

	/*
	=========================
	gl_texture::currentlyBound
	=========================
	*/
	inline gl_texture_int gl_texture::currentlyBound()
	{
		return m_CurrentlyBound;
	}

	/*
	======================
	gl_texture::currentUnit
	======================
	*/
	inline uint8_t gl_texture::currentUnit()
	{
		return m_CurrentUnit;
	}

	/*
	==========================
	gl_texture::getWhiteTexture
	==========================
	*/
	inline gl_texture_int gl_texture::getWhiteTexture()
	{
		return m_WhiteTex;
	}

	/*
	==========================
	gl_texture::setWhiteTexture
	==========================
	*/
	inline void gl_texture::setWhiteTexture(gl_texture_int texture)
	{
		m_WhiteTex = texture;
	}

	class DE_API gl_error
    {

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
			/// @param errorCode code d'erreur OpenGL.
			/// @return          Une chaîne de caractères constante du nom de l'erreur.
			static const char *getName(GLenum errorCode);

	};

	class DE_API gl_core
    {

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

			/// @brief Active ou désactive la vérification sur l'axe Z.
			/// @param value 
			static void enableDepthMask(bool value);

			static void setDepthFunction(gl_depth_function func);

			static void setCullFace(gl_cull_face cullFace);

	};

    class DE_API gl_framebuffer
    {

        public:
            static gl_framebuffer_int create();
            static void bind(gl_framebuffer_int fbo);
            static void bindDefault();
            static bool check();
            static void destroy(gl_framebuffer_int fbo);

            static void attachTexture(int width, int height);
            static void attachRenderbuffer(gl_renderbuffer_int rbo);

    };

    class DE_API gl_renderbuffer
    {

        public:
            static gl_renderbuffer_int create();
            static void bind(gl_renderbuffer_int rbo);
            static void bindDefault();
            static void store(int width, int height);

    };

    class DE_API gl_framerenderbuffer
    {

        public:
            bool create(int width, int height);

            gl_framebuffer_int framebuffer() const;
            gl_renderbuffer_int renderbuffer() const;

        private:
            gl_framebuffer_int m_Framebuffer;
            gl_renderbuffer_int m_Renderbuffer;

    };

    inline gl_framebuffer_int gl_framerenderbuffer::framebuffer() const
    {
        return m_Framebuffer;
    }

    inline gl_renderbuffer_int gl_framerenderbuffer::renderbuffer() const
    {
        return m_Renderbuffer;
    }

    class DE_API gl_renderer
    {

		public:
			gl_renderer();

			static bool create(gl_renderer &dest, window *window);

			void clear() const;
            void clearColor() const;
			void draw(unsigned int numberOfVertices) const;
			void swapBuffers() const;

			SDL_GLContext context();
			window *getWindow();

			colora getClearColor() const;

			void setClearColor(const colora color);

		private:
			SDL_GLContext m_Context;
			window *m_Window;

			colora m_ClearColor;

	};

	inline SDL_GLContext gl_renderer::context()
	{
		return m_Context;
	}

	inline window *gl_renderer::getWindow()
	{
		return m_Window;
	}

	inline colora gl_renderer::getClearColor() const
	{
		return m_ClearColor;
	}

}


#endif