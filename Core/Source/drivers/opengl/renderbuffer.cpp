#include "DE/drivers/opengl/renderbuffer.hpp"

namespace deep
{

    namespace GL3
    {

        /*
        ==========================================
        renderbuffer_manager::renderbuffer_manager
        ==========================================
        */
        renderbuffer_manager::renderbuffer_manager()
            : m_CurrentlyBound(0),
              m_CurrentID(0)
        { }

        /*
        ===================================
        renderbuffer_manager::get_singleton
        ===================================
        */
        renderbuffer_manager *renderbuffer_manager::get_singleton()
        {
            static renderbuffer_manager singleton;

            return &singleton;
        }

        /*
        ===============================
        renderbuffer_manager::raw_create
        ===============================
        */
        GLuint renderbuffer_manager::raw_create()
        {
            GLuint rbo;
            DE_GL_CALL(glGenRenderbuffers(1, &rbo));

            return rbo;
        }

        /*
        ============================
        renderbuffer_manager::create
        ============================
        */
        gl_id renderbuffer_manager::create(const char *name)
        {
            GLuint rbo = raw_create();

            hash_entry<GLuint> &el = m_Renderbuffers.insert(name, rbo);

            return el.key;
        }

        /*
        =============================
        renderbuffer_manager::raw_bind
        =============================
        */
        void renderbuffer_manager::raw_bind(GLuint rbo)
        {
            DE_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
        }

        /*
        ==========================
        renderbuffer_manager::bind
        ==========================
        */
        bool renderbuffer_manager::bind(gl_id rbo)
        {
            hash_entry<GLuint> *el = m_Renderbuffers[rbo];
            if(el == nullptr)
                return false;

            raw_bind(el->value);
            m_CurrentID = rbo;

            return true;
        }

        /*
        ==========================
        renderbuffer_manager::bind
        ==========================
        */
        bool renderbuffer_manager::bind(const char *name)
        {
            hash_entry<GLuint> *el = m_Renderbuffers[name];
            if(el == nullptr)
                return false;

            raw_bind(el->value);
            m_CurrentID = el->key;

            return true;
        }

        /*
        =================================
        renderbuffer_manager::bind_default
        =================================
        */
        void renderbuffer_manager::bind_default()
        {
            DE_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

            m_CurrentlyBound = 0;
            m_CurrentID = 0;
        }

        /*
        ===========================
        renderbuffer_manager::store
        ===========================
        */
        void renderbuffer_manager::store(int width, int height)
        {
            DE_GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
        }

        /*
        ============================
        renderbuffer_manager::resize
        ============================
        */
        bool renderbuffer_manager::resize(int width, int height)
        {
            hash_entry<GLuint> *el = m_Renderbuffers[m_CurrentID];
            if(el == nullptr)
                return false;

            raw_destroy(m_CurrentlyBound);
            m_CurrentlyBound = 0;

            el->value = raw_create();

            return true;
        }

        /*
        ================================
        renderbuffer_manager::raw_destroy
        ================================
        */
        void renderbuffer_manager::raw_destroy(GLuint rbo)
        {
            DE_GL_CALL(glDeleteRenderbuffers(1, &rbo));
        }

        /*
        =============================
        renderbuffer_manager::destroy
        =============================
        */
        bool renderbuffer_manager::destroy(gl_id rbo)
        {
            hash_entry<GLuint> *el = m_Renderbuffers[rbo];
            if(el == nullptr)
                return false;

            raw_destroy(el->value);
            m_Renderbuffers.remove(el->key);

            return true;
        }

        /*
        =============================
        renderbuffer_manager::destroy
        =============================
        */
        bool renderbuffer_manager::destroy(const char *name)
        {
            hash_entry<GLuint> *el = m_Renderbuffers[name];
            if(el == nullptr)
                return false;

            raw_destroy(el->value);
            m_Renderbuffers.remove(el->key);

            return true;
        }

    }

}
