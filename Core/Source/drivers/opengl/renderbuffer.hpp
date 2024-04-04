#ifndef __DEEP_ENGINE_OPENGL_RENDERBUFFER_HPP__
#define __DEEP_ENGINE_OPENGL_RENDERBUFFER_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/templates/hash_table.hpp"
#include "drivers/opengl/def.hpp"

namespace deep
{

    namespace GL3
    {

        class renderbuffer_manager
        {

            public:
                DE_API static renderbuffer_manager *get_singleton();

                DE_API GLuint raw_create();
                DE_API gl_id create(const char *name);

                DE_API void raw_bind(GLuint rbo);
                DE_API bool bind(gl_id rbo);
                DE_API bool bind(const char *name);

                DE_API void bind_default();

                DE_API hash_entry<GLuint> *get(gl_id rbo);
                DE_API hash_entry<GLuint> *get(const char *name);

                DE_API void store(int width, int height);
                DE_API bool resize();

                DE_API void raw_destroy(GLuint rbo);
                DE_API bool destroy(gl_id rbo);
                DE_API bool destroy(const char *name);

                DE_API GLuint currently_bound();
                DE_API gl_id current_id();

                DE_API void set_currently_bound(GLuint rbo);
                DE_API void set_current_id(gl_id rbo);

            private:
                renderbuffer_manager();

                hash_table<GLuint> m_Renderbuffers;
                GLuint m_CurrentlyBound;
                gl_id m_CurrentID;

        };

        /*
        =========================
        renderbuffer_manager::get
        =========================
        */
        inline hash_entry<GLuint> *renderbuffer_manager::get(gl_id rbo)
        {
            return m_Renderbuffers[rbo];
        }

        /*
        =========================
        renderbuffer_manager::get
        =========================
        */
        inline hash_entry<GLuint> *renderbuffer_manager::get(const char *name)
        {
            return m_Renderbuffers[name];
        }

        /*
        ====================================
        renderbuffer_manager::currently_bound
        ====================================
        */
        inline GLuint renderbuffer_manager::currently_bound()
        {
            return m_CurrentlyBound;
        }

        /*
        ===============================
        renderbuffer_manager::current_id
        ===============================
        */
        inline gl_id renderbuffer_manager::current_id()
        {
            return m_CurrentID;
        }

        /*
        =========================================
        renderbuffer_manager::set_currently_bound
        =========================================
        */
        inline void renderbuffer_manager::set_currently_bound(GLuint rbo)
        {
            m_CurrentlyBound = rbo;
        }

        /*
        ====================================
        renderbuffer_manager::set_current_id
        ====================================
        */
        inline void renderbuffer_manager::set_current_id(gl_id rbo)
        {
            m_CurrentID = rbo;
        }

    }

}

#endif