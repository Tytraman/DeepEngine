#ifndef __DEEP_ENGINE_OPENGL_RENDERBUFFER_HPP__
#define __DEEP_ENGINE_OPENGL_RENDERBUFFER_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/memory/hash_table.hpp"
#include "DE/drivers/opengl/def.hpp"

namespace deep
{

    namespace GL3
    {

        class renderbuffer_manager
        {

            public:
                DE_API static renderbuffer_manager *get_singleton();

                DE_API GLuint rawCreate();
                DE_API gl_id create(const char *name);

                DE_API void rawBind(GLuint rbo);
                DE_API bool bind(gl_id rbo);
                DE_API bool bind(const char *name);

                DE_API void bindDefault();

                DE_API hash_entry<GLuint> *get(gl_id rbo);
                DE_API hash_entry<GLuint> *get(const char *name);

                DE_API void store(int width, int height);
                DE_API bool resize(int width, int height);

                DE_API void rawDestroy(GLuint rbo);
                DE_API bool destroy(gl_id rbo);
                DE_API bool destroy(const char *name);

                DE_API GLuint currentlyBound();
                DE_API gl_id currentID();

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
        renderbuffer_manager::currentlyBound
        ====================================
        */
        inline GLuint renderbuffer_manager::currentlyBound()
        {
            return m_CurrentlyBound;
        }

        /*
        ===============================
        renderbuffer_manager::currentID
        ===============================
        */
        inline gl_id renderbuffer_manager::currentID()
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