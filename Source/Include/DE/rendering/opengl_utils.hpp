#ifndef __DEEP_ENGINE_OPENGL_UTILS_HPP__
#define __DEEP_ENGINE_OPENGL_UTILS_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>
#include <DE/vec.hpp>
#include <DE/mat.hpp>
#include <DE/image/image.hpp>
#include <DE/memory/hash_table.hpp>
#include <DE/memory/pair.hpp>

#include <glad/glad.h>

#include <stdint.h>

namespace deep
{

	/*
			La notion la plus importante à comprendre avec OpenGL est sa façon de fonctionner : c'est une machine à états.
	
			On défini un objet comme étant actif ce qui implique que les futurs appels de fonctions agiront sur cet objet.
	*/


#define DE_GL_CALLV(_x)                                                          \
	[&]() {                                                                      \
		deep::gl_error::clearErrors();                                              \
		auto ret = _x;                                                           \
		if(deep::gl_error::checkErrors(#_x, DE_FILE_NAME, DE_LINE)) DE_DEBUG_BREAK; \
		return ret;                                                              \
	}()

#define DE_GL_CALL(_x)                                                       \
	deep::gl_error::clearErrors();                                              \
	_x;                                                                      \
	if(deep::gl_error::checkErrors(#_x, DE_FILE_NAME, DE_LINE)) DE_DEBUG_BREAK;

    using gl_vbo_int          = unsigned int;
    using vbo_id              = uint64_t;

    using gl_vao_int          = unsigned int;
    using vao_id              = uint64_t;

    using gl_shader_int       = unsigned int;
    using shader_id           = uint64_t;

    using gl_program_int      = unsigned int;
    using program_id          = uint64_t;

    using gl_texture_int      = unsigned int;
    using texture_id          = uint64_t;

    using gl_framebuffer_int  = unsigned int;
    using framebuffer_id      = uint64_t;

    using gl_renderbuffer_int = unsigned int;
    using renderbuffer_id     = uint64_t;

    class vbo_manager;
    class vao_manager;
    class shader_manager;
    class program_manager;
    class texture_manager;
    class framebuffer_manager;
    class renderbuffer_manager;
    class framerenderbuffer;

    class window;

	
	enum class gl_attrib_components_number : int
    {
		x1 = 1,
		x2 = 2,
		x3 = 3,
		x4 = 4
	};

	enum class gl_type
    {
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
	class DE_API vbo_manager
    {

		public:
			static vbo_id create(const char *name);

            static void rawBind(gl_vbo_int vbo);
			static bool bind(vbo_id vbo);
            static bool bind(const char *name);

            static void bindDefault();

            static void rawDestroy(gl_vbo_int vbo);
			static bool destroy(vbo_id vbo);
            static bool destroy(const char *name);

			static void addAttribute(unsigned int index, gl_attrib_components_number componentsNumber, gl_type type, int stride, int offset);
			static void setVerticesNumber(unsigned int number);

			static void transmitData(memory_chunk &data);

			/// @brief  Récupère le nombre de sommets que le VBO possède.
			/// @return Le nombre de sommets que le VBO possède.
			static unsigned int getVerticesNumber();

			static unsigned int getVerticesNumber(vbo_id vbo);

			static gl_vbo_int currentlyBound();
            static vbo_id currentID();

		private:
			static gl_vbo_int m_CurrentlyBound;
            static vbo_id m_CurrentID;
            static hash_table<pair<gl_vbo_int, unsigned int>> m_VBOs;

        public:
            vbo_manager() = delete;

	};

	/*
	===========================
	vbo_manager::currentlyBound
	===========================
	*/
	inline gl_vbo_int vbo_manager::currentlyBound()
	{
		return m_CurrentlyBound;
	}

    /*
	======================
	vbo_manager::currentID
	======================
	*/
    inline vbo_id vbo_manager::currentID()
    {
        return m_CurrentID;
    }

	class DE_API vao_manager
    {

		public:
			static vao_id create(const char *name);

            static void rawBind(gl_vao_int vao);
			static bool bind(vao_id vao);
            static bool bind(const char *name);

            static void bindDefault();

            static void rawDestroy(gl_vao_int vao);
			static bool destroy(vao_id vao);
            static bool destroy(const char *name);

			static gl_vao_int currentlyBound();
            static vao_id currentID();

		private:
			static gl_vao_int m_CurrentlyBound;
            static vao_id m_CurrentID;
            static hash_table<gl_vao_int> m_VAOs;

        public:
            vao_manager() = delete;
	};

	/*
	===========================
	vao_manager::currentlyBound
	===========================
	*/
	inline gl_vao_int vao_manager::currentlyBound()
	{
		return m_CurrentlyBound;
	}

    /*
	======================
	vao_manager::currentID
	======================
	*/
    inline vao_id vao_manager::currentID()
    {
        return m_CurrentID;
    }

	class DE_API shader_manager
    {

		public:
			static shader_id create(const char *name, gl_shader_type shaderType);

			static void rawLoad(gl_shader_int shader, memory_chunk &program);
            static bool load(shader_id shader, memory_chunk &program);
            static bool load(const char *name, memory_chunk &program);

			static bool rawCompile(gl_shader_int shader);
            static bool compile(shader_id shader);
            static bool compile(const char *name);

			static void rawDestroy(gl_shader_int shader);
            static bool destroy(shader_id shader);
            static bool destroy(const char *name);

        private:
            static hash_table<gl_shader_int> m_Shaders;

        public:
            shader_manager() = delete;

            friend program_manager;
	};

    struct DE_API program_item
    {
        gl_program_int program;
        hash_table<pair<int, float>> fUniforms;
        hash_table<pair<int, int>> iUniforms;
        hash_table<pair<int, fvec3>> fv3Uniforms;
        hash_table<pair<int, fmat4x4>> fm4Uniforms;

        program_item(gl_program_int program);
    };

	class DE_API program_manager
    {

		public:
			static program_id create(const char *name);

			static void rawAttachShader(gl_program_int program, gl_shader_int shader);
            static bool attachShader(program_id program, shader_id shader);
            static bool attachShader(const char *progName, const char *shadName);
            static bool attachShader(const char *progName, shader_id shader);
            static bool attachShader(program_id program, const char *shadName);

            static bool addUniform(const char *uniformName, int location, float value);
            static bool addUniform(const char *uniformName, int location, int value);
            static bool addUniform(const char *uniformName, int location, const fvec3 &value);
            static bool addUniform(const char *uniformName, int location, const fmat4x4 &value);

            static bool setUniform(const char *uniformName, float value);
            static bool setUniform(const char *uniformName, int value);
            static bool setUniform(const char *uniformName, const fvec3 &value);
            static bool setUniform(const char *uniformName, const fmat4x4 &value);

			static bool rawLink(gl_program_int program);
            static bool link(program_id program);
            static bool link(const char *name);

			static void rawUse(gl_program_int program);
            static bool use(program_id program);
            static bool use(const char *name);

			static void rawDestroy(gl_program_int program);
            static bool destroy(program_id program);
            static bool destroy(const char *name);

            static bool sendUniforms();
			static void destroyAllPrograms();

            static bool exists(program_id program);
            static bool exists(const char *name);

            static hash_entry<program_item> *get(program_id program);
            static hash_entry<program_item> *get(const char *name);

			static gl_program_int currentlyBound();
            static program_id currentID();

            static hash_function getHashFunction();

		private:
			static gl_program_int m_CurrentlyBound;
            static program_id m_CurrentID;
			static hash_table<program_item> m_Programs;

        public:
            program_manager() = delete;

	};

    /*
	=======================
	program_manager::exists
	=======================
	*/
    inline bool program_manager::exists(program_id program)
    {
        return m_Programs[program] != nullptr;
    }

    /*
	=======================
	program_manager::exists
	=======================
	*/
    inline bool program_manager::exists(const char *name)
    {
        return m_Programs[name] != nullptr;
    }

    /*
	====================
	program_manager::get
	====================
	*/
    inline hash_entry<program_item> *program_manager::get(program_id program)
    {
        return m_Programs[program];
    }

    /*
	====================
	program_manager::get
	====================
	*/
    inline hash_entry<program_item> *program_manager::get(const char *name)
    {
        return m_Programs[name];
    }

	/*
	===============================
	program_manager::currentlyBound
	===============================
	*/
	inline gl_program_int program_manager::currentlyBound()
	{
		return m_CurrentlyBound;
	}

    /*
	==========================
	program_manager::currentID
	==========================
	*/
    inline program_id program_manager::currentID()
    {
        return m_CurrentID;
    }

    /*
	================================
	program_manager::getHashFunction
	================================
	*/
    inline hash_function program_manager::getHashFunction()
    {
        return m_Programs.getHashFunction();
    }

	class DE_API uniform_manager
    {

		public:
			static int rawFind(gl_program_int program, const char *name);
            static int find(program_id program, const char *name);
            static int find(const char *progName, const char *name);

			static void send(int location, float value);
			static void send(int location, int value);
			static void send(int location, const fvec2 &vec);
			static void send(int location, const fvec3 &vec);
			static void send(int location, fmat3x3 &mat);
			static void send(int location, fmat4x4 &mat);

        public:
            uniform_manager() = delete;

	};

	class DE_API texture_manager
    {

		public:
			static texture_id create2D(const char *name);

			static void rawBind(gl_texture_int texture, uint8_t unit);
            static bool bind(texture_id texture, uint8_t unit);
            static bool bind(const char *name, uint8_t unit);

			static void rawBindCubemaps(gl_texture_int texture);
            static bool bindCubemaps(texture_id texture);
            static bool bindCubemaps(const char *name);

            static void rawDestroy(gl_texture_int texture);
            static bool destroy(texture_id texture);
            static bool destroy(const char *name);

			static void setTextureWrappingS(gl_texture_wrap mode);
			static void setTextureWrappingT(gl_texture_wrap mode);
			static void setTextureWrappingR(gl_texture_wrap mode);
			static void setTextureFiltering(gl_texture_filter mode);

			static void setTextureWrappingSCubemaps(gl_texture_wrap mode);
			static void setTextureWrappingTCubemaps(gl_texture_wrap mode);
			static void setTextureWrappingRCubemaps(gl_texture_wrap mode);
			static void setTextureFilteringCubemaps(gl_texture_filter mode);

            static void allocSpace(int width, int height);

			static void transmitTexture(mem_ptr data, int width, int height, image_color_type colorType);
			static void transmitTextureCubemaps(mem_ptr left, mem_ptr front, mem_ptr right, mem_ptr back, mem_ptr bottom, mem_ptr top, int width, int height, image_color_type colorType);

			static texture_id getWhiteTexture();
			static void setWhiteTexture(texture_id texture);

            static hash_entry<gl_texture_int> *get(texture_id texture);
            static hash_entry<gl_texture_int> *get(const char *name);

			static gl_texture_int currentlyBound();
            static texture_id currentID();
            static texture_id currentCubemapsID();
			static uint8_t currentUnit();

		private:
			static texture_id m_WhiteTex;

			static gl_texture_int m_CurrentlyBound;
            static texture_id m_CurrentID;
            static texture_id m_CurrentCubemapsID;
			static uint8_t m_CurrentUnit;

            static hash_table<gl_texture_int> m_Textures;

        public:
			texture_manager() = delete;

	};

    /*
	====================
	texture_manager::get
	====================
	*/
    inline hash_entry<gl_texture_int> *texture_manager::get(texture_id texture)
    {
        return m_Textures[texture];
    }

    /*
	====================
	texture_manager::get
	====================
	*/
    inline hash_entry<gl_texture_int> *texture_manager::get(const char *name)
    {
        return m_Textures[name];
    }

	/*
	===============================
	texture_manager::currentlyBound
	===============================
	*/
	inline gl_texture_int texture_manager::currentlyBound()
	{
		return m_CurrentlyBound;
	}

    /*
	==========================
	texture_manager::currentID
	==========================
	*/
    inline texture_id texture_manager::currentID()
    {
        return m_CurrentID;
    }

    /*
	==================================
	texture_manager::currentCubemapsID
	==================================
	*/
    inline texture_id texture_manager::currentCubemapsID()
    {
        return m_CurrentCubemapsID;
    }

	/*
	============================
	texture_manager::currentUnit
	============================
	*/
	inline uint8_t texture_manager::currentUnit()
	{
		return m_CurrentUnit;
	}

	/*
	================================
	texture_manager::getWhiteTexture
	================================
	*/
	inline texture_id texture_manager::getWhiteTexture()
	{
		return m_WhiteTex;
	}

	/*
	================================
	texture_manager::setWhiteTexture
	================================
	*/
	inline void texture_manager::setWhiteTexture(texture_id texture)
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

	class DE_API gpu_core
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

            static void enableDepthTest();
            static void disableDepthTest();

			static void setDepthFunction(gl_depth_function func);

			static void setCullFace(gl_cull_face cullFace);

	};

    struct DE_API framebuffer_item
    {
        gl_framebuffer_int fbo;
        texture_id texture;
        renderbuffer_id rbo;

        framebuffer_item(gl_framebuffer_int fbo);
    };

    class DE_API framebuffer_manager
    {

        public:
            static framebuffer_id create(const char *name);

            static void rawBind(gl_framebuffer_int fbo);
            static bool bind(framebuffer_id fbo);
            static bool bind(const char *name);

            static void bindDefault();

            static bool check();

            static void rawDestroy(gl_framebuffer_int fbo);
            static bool destroy(framebuffer_id fbo, bool destroyTexture = false);
            static bool destroy(const char *name, bool destroyTexture = false);

            static bool attachTexture(texture_id texture);
            static bool attachTexture(const char *name);

            static bool attachRenderbuffer(framebuffer_id fbo, renderbuffer_id rbo);
            static bool attachRenderbuffer(const char *fboName, const char *rboName);

            static bool saveTextureAsImage(int width, int height, const char *filedest);

            static hash_entry<framebuffer_item> *get(framebuffer_id fbo);
            static hash_entry<framebuffer_item> *get(const char *name);

            static gl_framebuffer_int currentlyBound();
            static framebuffer_id currentID();
            static texture_id currentTextureID();

        private:
            static gl_framebuffer_int m_CurrentlyBound;
            static framebuffer_id m_CurrentID;

			static hash_table<framebuffer_item> m_Framebuffers;

    };

    /*
	========================
	framebuffer_manager::get
	========================
	*/
    inline hash_entry<framebuffer_item> *framebuffer_manager::get(framebuffer_id fbo)
    {
        return m_Framebuffers[fbo];
    }

    /*
	========================
	framebuffer_manager::get
	========================
	*/
    inline hash_entry<framebuffer_item> *framebuffer_manager::get(const char *name)
    {
        return m_Framebuffers[name];
    }

    /*
	===================================
	framebuffer_manager::currentlyBound
	===================================
	*/
    inline gl_framebuffer_int framebuffer_manager::currentlyBound()
    {
        return m_CurrentlyBound;
    }

    /*
	==============================
	framebuffer_manager::currentID
	==============================
	*/
    inline framebuffer_id framebuffer_manager::currentID()
    {
        return m_CurrentID;
    }

    /*
	=====================================
	framebuffer_manager::currentTextureID
	=====================================
	*/
    inline texture_id framebuffer_manager::currentTextureID()
    {
        auto el = m_Framebuffers[m_CurrentID];
        if(el == nullptr)
            return 0;

        return el->value.texture;
    }

    class DE_API renderbuffer_manager
    {

        public:

            static gl_renderbuffer_int rawCreate();
            static renderbuffer_id create(const char *name);

            static void rawBind(gl_renderbuffer_int rbo);
            static bool bind(renderbuffer_id rbo);
            static bool bind(const char *name);

            static void bindDefault();

            static hash_entry<gl_renderbuffer_int> *get(renderbuffer_id rbo);
            static hash_entry<gl_renderbuffer_int> *get(const char *name);

            static void store(int width, int height);
            static bool resize(int width, int height);

            static void rawDestroy(gl_renderbuffer_int rbo);
            static bool destroy(renderbuffer_id rbo);
            static bool destroy(const char *name);

            static gl_renderbuffer_int currentlyBound();
            static renderbuffer_id currentID();

        private:
            static hash_table<gl_renderbuffer_int> m_Renderbuffers;
            static gl_renderbuffer_int m_CurrentlyBound;
            static renderbuffer_id m_CurrentID;

        public:
            friend framebuffer_manager;
    };

    /*
	=========================
	renderbuffer_manager::get
	=========================
	*/
    inline hash_entry<gl_renderbuffer_int> *renderbuffer_manager::get(renderbuffer_id rbo)
    {
        return m_Renderbuffers[rbo];
    }

    /*
	=========================
	renderbuffer_manager::get
	=========================
	*/
    inline hash_entry<gl_renderbuffer_int> *renderbuffer_manager::get(const char *name)
    {
        return m_Renderbuffers[name];
    }

    /*
	====================================
	renderbuffer_manager::currentlyBound
	====================================
	*/
    inline gl_renderbuffer_int renderbuffer_manager::currentlyBound()
    {
        return m_CurrentlyBound;
    }

    /*
	===============================
	renderbuffer_manager::currentID
	===============================
	*/
    inline renderbuffer_id renderbuffer_manager::currentID()
    {
        return m_CurrentID;
    }

    class DE_API framerenderbuffer
    {

        public:
            bool create(const char *name, int width, int height);

            void destroy();

            bool saveTextureAsImage(const char *destpath);

            bool resize(const char *newName, int width, int height);

            framebuffer_id framebuffer() const;
            renderbuffer_id renderbuffer() const;
            vbo_id vbo() const;
            vao_id vao() const;
            int width() const;
            int height() const;

        private:
            framebuffer_id m_Framebuffer;
            renderbuffer_id m_Renderbuffer;
            vbo_id m_VBO;
            vao_id m_VAO;
            int m_Width;
            int m_Height;

    };

    /*
	==============================
	framerenderbuffer::framebuffer
	==============================
	*/
    inline framebuffer_id framerenderbuffer::framebuffer() const
    {
        return m_Framebuffer;
    }

    /*
	===============================
	framerenderbuffer::renderbuffer
	===============================
	*/
    inline renderbuffer_id framerenderbuffer::renderbuffer() const
    {
        return m_Renderbuffer;
    }

    /*
	======================
	framerenderbuffer::vbo
	======================
	*/
    inline vbo_id framerenderbuffer::vbo() const
    {
        return m_VBO;
    }

    /*
	======================
	framerenderbuffer::vao
	======================
	*/
    inline vao_id framerenderbuffer::vao() const
    {
        return m_VAO;
    }

    /*
	========================
	framerenderbuffer::width
	========================
	*/
    inline int framerenderbuffer::width() const
    {
        return m_Width;
    }

    /*
	=========================
	framerenderbuffer::height
	=========================
	*/
    inline int framerenderbuffer::height() const
    {
        return m_Height;
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

    /*
	====================
	gl_renderer::context
	====================
	*/
	inline SDL_GLContext gl_renderer::context()
	{
		return m_Context;
	}

    /*
	======================
	gl_renderer::getWindow
	======================
	*/
	inline window *gl_renderer::getWindow()
	{
		return m_Window;
	}

    /*
	==========================
	gl_renderer::getClearColor
	==========================
	*/
	inline colora gl_renderer::getClearColor() const
	{
		return m_ClearColor;
	}

}


#endif