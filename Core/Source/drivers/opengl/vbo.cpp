#include "DE/drivers/opengl/vbo.hpp"

namespace deep
{
    namespace GL3
    {

        /*
        ==========================
        vbo_manager::get_singleton
        ==========================
        */
        vbo_manager *vbo_manager::get_singleton()
        {
            static vbo_manager singleton;

            return &singleton;
        }

        /*
        ========================
        vbo_manager::vbo_manager
        ========================
        */
        vbo_manager::vbo_manager()
            : m_CurrentlyBound(0),
              m_CurrentID(0)
        { }

        /*
        ===================
        vbo_manager::create
        ===================
        */
        gl_id vbo_manager::create(const char *name)
        {
            GLuint vbo;
            DE_GL_CALL(glGenBuffers(1, &vbo));

            auto &el = m_VBOs.insert(name, pair(vbo, static_cast<unsigned int>(0)));

            return el.key;
        }

        /*
        ====================
        vbo_manager::raw_bind
        ====================
        */
        void vbo_manager::raw_bind(GLuint vbo)
        {
            DE_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

            m_CurrentlyBound = vbo;
        }

        /*
        =================
        vbo_manager::bind
        =================
        */
        bool vbo_manager::bind(gl_id vbo)
        {
            auto el = m_VBOs[vbo];
            if(el == nullptr)
                return false;

            raw_bind(el->value.value1());

            m_CurrentID = vbo;

            return true;
        }

        /*
        =================
        vbo_manager::bind
        =================
        */
        bool vbo_manager::bind(const char *name)
        {
            auto el = m_VBOs[name];
            if(el == nullptr)
                return false;

            raw_bind(el->value.value2());
        
            m_CurrentID = el->key;

            return true;
        }

        /*
        ========================
        vbo_manager::bind_default
        ========================
        */
        void vbo_manager::bind_default()
        {
            raw_bind(0);
            m_CurrentID = 0;
        }

        /*
        =======================
        vbo_manager::raw_destroy
        =======================
        */
        void vbo_manager::raw_destroy(GLuint vbo)
        {
            DE_GL_CALL(glDeleteBuffers(1, &vbo));
        }

        /*
        ====================
        vbo_manager::destroy
        ====================
        */
        bool vbo_manager::destroy(gl_id vbo)
        {
            auto el = m_VBOs[vbo];
            if(el == nullptr)
                return false;

            raw_destroy(el->value.value1());
            m_VBOs.remove(el->key);

            return true;
        }

        /*
        ====================
        vbo_manager::destroy
        ====================
        */
        bool vbo_manager::destroy(const char *name)
        {
            auto el = m_VBOs[name];
            if(el == nullptr)
                return false;

            raw_destroy(el->value.value1());
            m_VBOs.remove(el->key);

            return true;
        }

        /*
        ==============================
        vbo_manager::set_vertices_number
        ==============================
        */
        void vbo_manager::set_vertices_number(unsigned int number)
        {
            auto el = m_VBOs[m_CurrentID];
            if(el == nullptr)
                return;

            el->value.value2() = number;
        }

        /*
        ==============================
        vbo_manager::get_vertices_number
        ==============================
        */
        unsigned int vbo_manager::get_vertices_number()
        {
            return get_vertices_number(m_CurrentlyBound);
        }

        /*
        ==============================
        vbo_manager::get_vertices_number
        ==============================
        */
        unsigned int vbo_manager::get_vertices_number(gl_id vbo)
        {
            auto el = m_VBOs[vbo];
            if(el == nullptr)
                return 0;

            return el->value.value2();
        }

        /*
        =========================
        vbo_manager::transmit_data
        =========================
        */
        void vbo_manager::transmit_data(memory_chunk &data)
        {
            DE_GL_CALL(glBufferData(GL_ARRAY_BUFFER, data.size(), data.data(), GL_STATIC_DRAW));
        }

        /*
        =========================
        vbo_manager::add_attribute
        =========================
        */
        void vbo_manager::add_attribute(unsigned int index, gl_attrib_components_number componentsNumber, gl_type type, int stride, int offset)
        {
            uint64_t o = offset;

            DE_GL_CALL(glVertexAttribPointer(index, static_cast<GLint>(componentsNumber), static_cast<GLenum>(type), GL_FALSE, stride, (void *) o));
            DE_GL_CALL(glEnableVertexAttribArray(index));
        }

    }
}
