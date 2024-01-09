#include "DE/drivers/opengl/texture.hpp"


namespace deep
{

    namespace GL3
    {

        /*
        ===========================================
        texture_manager::texture_item::texture_item
        ===========================================
        */
        texture_manager::texture_item::texture_item(const char *_name, GLuint texture, gl_texture_type _type)
            : name(_name),
              glTexture(texture),
              type(_type)
        { }

        /*
        ================================
        texture_manager::texture_manager
        ================================
        */
        texture_manager::texture_manager()
            : m_WhiteTex(0),
              m_CurrentlyBound(0),
              m_CurrentID(0),
              m_CurrentCubemapsID(0),
              m_CurrentUnit(0)
        { }

        /*
        ==============================
        texture_manager::get_singleton
        ==============================
        */
        texture_manager *texture_manager::get_singleton()
        {
            static texture_manager singleton;

            return &singleton;
        }

        /*
        ==========================
        texture_manager::create_2D
        ==========================
        */
        gl_id texture_manager::create_2D(const char *name, gl_texture_type type)
        {
            GLuint texture;

            DE_GL_CALL(glGenTextures(1, &texture));

            hash_entry<texture_item> &el = m_Textures.insert(name, texture_item(name, texture, type));

            return el.key;
        }

        /*
        =========================
        texture_manager::raw_bind
        =========================
        */
        void texture_manager::raw_bind(GLuint texture, uint8_t unit, gl_texture_type type)
        {
            DE_GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
            DE_GL_CALL(glBindTexture(static_cast<GLenum>(type), texture));

            m_CurrentlyBound = texture;
            m_CurrentUnit = unit;
        }

        /*
        =====================
        texture_manager::bind
        =====================
        */
        bool texture_manager::bind(gl_id texture, uint8_t unit)
        {
            hash_entry<texture_item> *el = m_Textures[texture];
            if(el == nullptr)
                return false;

            raw_bind(el->value.glTexture, unit, el->value.type);

            m_CurrentID = el->key;

            return true;
        }

        /*
        ============================
        texture_manager::raw_destroy
        ============================
        */
        void texture_manager::raw_destroy(GLuint texture)
        {
            DE_GL_CALL(glDeleteTextures(1, &texture));
        }

        /*
        ========================
        texture_manager::destroy
        ========================
        */
        bool texture_manager::destroy(gl_id texture)
        {
            hash_entry<texture_item> *el = m_Textures[texture];
            if(el == nullptr)
                return false;

            raw_destroy(el->value.glTexture);
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
            hash_entry<texture_item> *el = m_Textures[name];
            if(el == nullptr)
                return false;

            raw_destroy(el->value.glTexture);
            m_Textures.remove(el->key);

            return true;
        }

        /*
        =======================================
        texture_manager::set_texture_wrapping_s
        =======================================
        */
        void texture_manager::set_texture_wrapping_s(gl_texture_wrap mode)
        {
            DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(mode)));
        }

        /*
        =======================================
        texture_manager::set_texture_wrapping_t
        =======================================
        */
        void texture_manager::set_texture_wrapping_t(gl_texture_wrap mode)
        {
            DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(mode)));
        }

        /*
        =======================================
        texture_manager::set_texture_wrapping_r
        =======================================
        */
        void texture_manager::set_texture_wrapping_r(gl_texture_wrap mode)
        {
            DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, static_cast<GLint>(mode)));
        }

        /*
        ======================================
        texture_manager::set_texture_filtering
        ======================================
        */
        void texture_manager::set_texture_filtering(gl_texture_filter mode)
        {
            DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(mode)));
            DE_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mode)));
        }

        /*
        ================================================
        texture_manager::set_texture_wrapping_s_cubemaps
        ================================================
        */
        void texture_manager::set_texture_wrapping_s_cubemaps(gl_texture_wrap mode)
        {
            DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<GLint>(mode)));
        }

        /*
        ================================================
        texture_manager::set_texture_wrapping_t_cubemaps
        ================================================
        */
        void texture_manager::set_texture_wrapping_t_cubemaps(gl_texture_wrap mode)
        {
            DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<GLint>(mode)));
        }

        /*
        ================================================
        texture_manager::set_texture_wrapping_r_cubemaps
        ================================================
        */
        void texture_manager::set_texture_wrapping_r_cubemaps(gl_texture_wrap mode)
        {
            DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<GLint>(mode)));
        }

        /*
        ===============================================
        texture_manager::set_texture_filtering_cubemaps
        ===============================================
        */
        void texture_manager::set_texture_filtering_cubemaps(gl_texture_filter mode)
        {
            DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(mode)));
            DE_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mode)));
        }

        /*
        ============================
        texture_manager::alloc_space
        ============================
        */
        void texture_manager::alloc_space(int width, int height)
        {
            DE_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
        }

        /*
        =================================
        texture_manager::transmit_texture
        =================================
        */
        void texture_manager::transmit_texture(mem_ptr data, int width, int height, image_color_space colorType)
        {
            GLenum internalFormat;
            GLenum format;
            switch(colorType)
            {
                default:
                    return;
                case image_color_space::RGB:
                {
                    internalFormat = GL_RGB8;
                    format = GL_RGB;
                } break;
                case image_color_space::RGBA:
                {
                    internalFormat = GL_RGBA8;
                    format = GL_RGBA;
                } break;
                case image_color_space::BGR:
                {
                    internalFormat = GL_RGB8;
                    format = GL_BGR;
                } break;
            }

            DE_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data));
        }

        /*
        ==========================================
        texture_manager::transmit_texture_cubemaps
        ==========================================
        */
        void texture_manager::transmit_texture_cubemaps(mem_ptr left, mem_ptr front, mem_ptr right, mem_ptr back, mem_ptr bottom, mem_ptr top, int width, int height, image_color_space colorType)
        {
            GLenum internalFormat;
            GLenum format;
            switch(colorType)
            {
                default:
                    return;
                case image_color_space::RGB:
                {
                    internalFormat = GL_RGB8;
                    format = GL_RGB;
                } break;
                case image_color_space::BGR:
                {
                    internalFormat = GL_RGB8;
                    format = GL_BGR;
                } break;
                case image_color_space::RGBA:
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
        ==============================
        texture_manager::enum_textures
        ==============================
        */
        void texture_manager::enum_textures(enum_callback callback, mem_ptr args)
        {
            if(callback == nullptr)
                return;

            hash_table_iterator begin = m_Textures.begin();
            hash_table_iterator end = m_Textures.end();

            for(; begin != end; ++begin)
                callback(begin->key, &begin->value, args);
        }

    }

}
