#ifndef __DEEP_ENGINE_OPENGL_TEXTURE_HPP__
#define __DEEP_ENGINE_OPENGL_TEXTURE_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/string.hpp"
#include "DE/memory/memory.hpp"
#include "DE/memory/hash_table.hpp"
#include "DE/image/image.hpp"
#include "DE/vec.hpp"
#include "DE/drivers/opengl/def.hpp"

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

                struct texture_2D_mapping_point
                {
                    fvec2 screenPos;
                    fvec2 texturePos;
                };

                struct texture_item
                {
                    string name;
                    GLuint glTexture;

                    DE_API texture_item(const char *name, GLuint texture);
                };

                using enum_callback = void (*)(gl_id textureID, texture_item *texture, mem_ptr args);

            public:
                DE_API static texture_manager *get_singleton();

                DE_API gl_id create_2D(const char *name);

                DE_API void raw_bind(GLuint texture, uint8_t unit);
                DE_API bool bind(gl_id texture, uint8_t unit);
                DE_API bool bind(const char *name, uint8_t unit);

                DE_API void raw_bind_cubemaps(GLuint texture);
                DE_API bool bind_cubemaps(gl_id texture);
                DE_API bool bind_cubemaps(const char *name);

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

                DE_API hash_entry<texture_item> *get(gl_id texture);
                DE_API hash_entry<texture_item> *get(const char *name);

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
        ====================
        texture_manager::get
        ====================
        */
        inline hash_entry<texture_manager::texture_item> *texture_manager::get(gl_id texture)
        {
            return m_Textures[texture];
        }

        /*
        ====================
        texture_manager::get
        ====================
        */
        inline hash_entry<texture_manager::texture_item> *texture_manager::get(const char *name)
        {
            return m_Textures[name];
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