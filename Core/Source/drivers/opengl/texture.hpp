#ifndef __DEEP_ENGINE_OPENGL_TEXTURE_HPP__
#define __DEEP_ENGINE_OPENGL_TEXTURE_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/string.hpp"
#include "core/memory.hpp"
#include "core/hash_table.hpp"
#include "image/image.hpp"
#include "maths/vec.hpp"
#include "drivers/opengl/def.hpp"

namespace deep
{

    namespace GL3
    {

        class texture_manager
        {

            public:

                enum class gl_texture_wrap : GLint
                {
                    Repeat         = GL_REPEAT,
                    MirroredRepeat = GL_MIRRORED_REPEAT,
                    ClampToEdge    = GL_CLAMP_TO_EDGE,
                    ClampToBorder  = GL_CLAMP_TO_BORDER
                };

                enum class gl_texture_filter : GLint
                {
                    Nearest = GL_NEAREST,
                    Linear  = GL_LINEAR
                };

                enum class gl_texture_type : GLenum
                {
                    texture_2D      = GL_TEXTURE_2D,
                    texture_cubemap = GL_TEXTURE_CUBE_MAP
                };

                struct texture_2D_mapping_point
                {
                    vec2<float> screenPos;
                    vec2<float> texturePos;
                };

                struct texture_item
                {
                    string name;
                    GLuint glTexture;
                    gl_texture_type type;

                    DE_API texture_item(const char *name, GLuint texture, gl_texture_type type);
                };

                using enum_callback = void (*)(gl_id textureID, texture_item *texture, mem_ptr args);

            public:
                DE_API static texture_manager *get_singleton();

                DE_API gl_id create_2D(const char *name, gl_texture_type type = gl_texture_type::texture_2D);

                DE_API void raw_bind(GLuint texture, uint8_t unit, gl_texture_type type);
                DE_API bool bind(gl_id texture, uint8_t unit = 0);
                DE_API bool bind(const char *name, uint8_t unit = 0);

                DE_API void raw_destroy(GLuint texture);
                DE_API bool destroy(gl_id texture);
                DE_API bool destroy(const char *name);

                DE_API void set_texture_wrapping_s(gl_texture_wrap mode);
                DE_API void set_texture_wrapping_t(gl_texture_wrap mode);
                DE_API void set_texture_wrapping_r(gl_texture_wrap mode);
                DE_API void set_texture_filtering(gl_texture_filter mode);

                DE_API void set_texture_wrapping_s_cubemaps(gl_texture_wrap mode);
                DE_API void set_texture_wrapping_t_cubemaps(gl_texture_wrap mode);
                DE_API void set_texture_wrapping_r_cubemaps(gl_texture_wrap mode);
                DE_API void set_texture_filtering_cubemaps(gl_texture_filter mode);

                DE_API void alloc_space(int width, int height);

                DE_API void transmit_texture(mem_ptr data, int width, int height, image_color_space colorType);
                DE_API void transmit_texture_cubemaps(mem_ptr left, mem_ptr front, mem_ptr right, mem_ptr back, mem_ptr bottom, mem_ptr top, int width, int height, image_color_space colorType);

                DE_API gl_id get_white_texture();
                DE_API void set_white_texture(gl_id texture);

                DE_API void enum_textures(enum_callback callback, mem_ptr args);

                DE_API texture_item *get(gl_id texture);
                DE_API texture_item *get(const char *name);

                DE_API GLuint currently_bound();
                DE_API gl_id current_id();
                DE_API gl_id current_cubemaps_id();
                DE_API uint8_t current_unit();

            private:
                texture_manager();

                gl_id m_WhiteTex;

                GLuint m_CurrentlyBound;
                gl_id m_CurrentID;
                gl_id m_CurrentCubemapsID;
                uint8_t m_CurrentUnit;

                hash_table<texture_item> m_Textures;

            public:
                texture_manager(const texture_manager &) = delete;
                texture_manager(texture_manager &&) = delete;

        };

        /*
        =====================
        texture_manager::bind
        =====================
        */
        inline bool texture_manager::bind(const char *name, uint8_t unit)
        {
            return bind(m_Textures.getHashFunction()(name), unit);
        }

        /*
        ====================
        texture_manager::get
        ====================
        */
        inline texture_manager::texture_item *texture_manager::get(gl_id texture)
        {
            hash_entry<texture_item> *hs = m_Textures[texture];
            if(hs == nullptr)
                return nullptr;

            return &hs->value;
        }

        /*
        ====================
        texture_manager::get
        ====================
        */
        inline texture_manager::texture_item *texture_manager::get(const char *name)
        {
            return get(m_Textures.getHashFunction()(name));
        }

        /*
        ===============================
        texture_manager::currently_bound
        ===============================
        */
        inline GLuint texture_manager::currently_bound()
        {
            return m_CurrentlyBound;
        }

        /*
        ==========================
        texture_manager::current_id
        ==========================
        */
        inline gl_id texture_manager::current_id()
        {
            return m_CurrentID;
        }

        /*
        ==================================
        texture_manager::current_cubemaps_id
        ==================================
        */
        inline gl_id texture_manager::current_cubemaps_id()
        {
            return m_CurrentCubemapsID;
        }

        /*
        ============================
        texture_manager::current_unit
        ============================
        */
        inline uint8_t texture_manager::current_unit()
        {
            return m_CurrentUnit;
        }

        /*
        ================================
        texture_manager::get_white_texture
        ================================
        */
        inline gl_id texture_manager::get_white_texture()
        {
            return m_WhiteTex;
        }

        /*
        ================================
        texture_manager::set_white_texture
        ================================
        */
        inline void texture_manager::set_white_texture(gl_id texture)
        {
            m_WhiteTex = texture;
        }

        }

}


#endif