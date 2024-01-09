#include "DE/drivers/opengl/vao.hpp"

namespace deep
{

    namespace GL3
    {

        /*
        =====================
        vao_manager::vao_item::vao_item
        =====================
        */
        vao_manager::vao_item::vao_item(const char *_name, GLuint vao, gl_id vbo)
            : name(_name),
              glVao(vao),
              attachedVbo(vbo)
        { }

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
            GLuint glVao;

            DE_GL_CALL(glGenVertexArrays(1, &glVao));

            hash_entry<vao_item> &el = m_VAOs.insert(name, vao_item(name, glVao, static_cast<gl_id>(0)));

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
        bool vao_manager::bind(gl_id _vao)
        {
            hash_entry<vao_item> *el = m_VAOs[_vao];
            if(el == nullptr)
                return false;

            raw_bind(el->value.glVao);

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
            hash_entry<vao_item> *el = m_VAOs[name];
            if(el == nullptr)
                return false;

            raw_bind(el->value.glVao);

            m_CurrentID = el->key;

            return true;
        }

        /*
        =======================
        vao_manager::attach_vbo
        =======================
        */
        bool vao_manager::attach_vbo(gl_id _vao, gl_id vbo)
        {
            hash_entry<vao_item> *el = m_VAOs[_vao];
            if(el == nullptr)
                return false;

            el->value.attachedVbo = vbo;

            return true;
        }

        /*
        =============================
        vao_manager::get_attached_vbo
        =============================
        */
        gl_id vao_manager::get_attached_vbo(gl_id _vao)
        {
            hash_entry<vao_item> *el = m_VAOs[_vao];
            if(el == nullptr)
                return false;

            return el->value.attachedVbo;
        }

        /*
        =========================
        vao_manager::bind_default
        =========================
        */
        void vao_manager::bind_default()
        {
            raw_bind(0);
            m_CurrentID = 0;
        }

        /*
        ========================
        vao_manager::raw_destroy
        ========================
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
        bool vao_manager::destroy(gl_id _vao)
        {
            hash_entry<vao_item> *el = m_VAOs[_vao];
            if(el == nullptr)
                return false;

            raw_destroy(el->value.glVao);
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
            hash_entry<vao_item> *el = m_VAOs[name];
            if(el == nullptr)
                return false;

            raw_destroy(el->value.glVao);
            m_VAOs.remove(el->key);

            return true;
        }

        /*
        ======================
        vao_manager::enum_vaos
        ======================
        */
        void vao_manager::enum_vaos(enum_callback callback, mem_ptr args)
        {
            if(callback == nullptr)
                return;

            hash_table_iterator begin = m_VAOs.begin();
            hash_table_iterator end = m_VAOs.end();

            for(; begin != end; ++begin)
                callback(begin->key, &begin->value, args);
        }

    }

}
