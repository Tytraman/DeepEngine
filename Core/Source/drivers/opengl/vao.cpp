#include "DE/drivers/opengl/vao.hpp"

namespace deep
{

    namespace GL3
    {

        /*
        ==========================
        vao_manager::get_singleton
        ==========================
        */
        vao_manager *vao_manager::get_singleton()
        {
            static vao_manager singleton;

            return &singleton;
        }

        /*
        ========================
        vao_manager::vao_manager
        ========================
        */
        vao_manager::vao_manager()
            : m_CurrentlyBound(0),
              m_CurrentID(0)
        { }

        /*
        ===================
        vao_manager::create
        ===================
        */
        gl_id vao_manager::create(const char *name)
        {
            GLuint vao;

            DE_GL_CALL(glGenVertexArrays(1, &vao));

            auto &el = m_VAOs.insert(name, vao);

            return el.key;
        }

        /*
        =====================
        vao_manager::raw_bind
        =====================
        */
        void vao_manager::raw_bind(GLuint vao)
        {
            DE_GL_CALL(glBindVertexArray(vao));

            m_CurrentlyBound = vao;
        }

        /*
        =================
        vao_manager::bind
        =================
        */
        bool vao_manager::bind(gl_id vao)
        {
            auto el = m_VAOs[vao];
            if(el == nullptr)
                return false;

            raw_bind(el->value);

            m_CurrentID = el->key;

            return true;
        }

        /*
        =================
        vao_manager::bind
        =================
        */
        bool vao_manager::bind(const char *name)
        {
            auto el = m_VAOs[name];
            if(el == nullptr)
                return false;

            raw_bind(el->value);

            m_CurrentID = el->key;

            return true;
        }

        /*
        ========================
        vao_manager::bind_default
        ========================
        */
        void vao_manager::bind_default()
        {
            raw_bind(0);
            m_CurrentID = 0;
        }

        /*
        =======================
        vao_manager::raw_destroy
        =======================
        */
        void vao_manager::raw_destroy(GLuint vao)
        {
            DE_GL_CALL(glDeleteVertexArrays(1, &vao));
        }

        /*
        ====================
        vao_manager::destroy
        ====================
        */
        bool vao_manager::destroy(gl_id vao)
        {
            auto el = m_VAOs[vao];
            if(el == nullptr)
                return false;

            raw_destroy(el->value);
            m_VAOs.remove(el->key);

            return true;
        }

        /*
        ====================
        vao_manager::destroy
        ====================
        */
        bool vao_manager::destroy(const char *name)
        {
            auto el = m_VAOs[name];
            if(el == nullptr)
                return false;

            raw_destroy(el->value);
            m_VAOs.remove(el->key);

            return true;
        }

    }

}
