#ifndef __DEEP_ENGINE_OPENGL_VAO_HPP__
#define __DEEP_ENGINE_OPENGL_VAO_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/string/string.hpp"
#include "core/templates/hash_table.hpp"
#include "drivers/opengl/def.hpp"

namespace deep
{

    namespace GL3
    {

        class vao_manager
        {

            public:
                struct vao_item
                {
                    string name;
                    GLuint glVao;
                    gl_id attachedVbo;

                    DE_API vao_item(const char *name, GLuint vao, gl_id vbo);
                };

                using enum_callback = void (*)(gl_id vaoID, vao_item *_vao, mem_ptr args);

            public:
                DE_API static vao_manager *get_singleton();

                DE_API gl_id create(const char *name);

                DE_API void raw_bind(GLuint vao);
                DE_API bool bind(gl_id vao);
                DE_API bool bind(const char *name);

                DE_API bool attach_vbo(gl_id vao, gl_id vbo);
                DE_API gl_id get_attached_vbo(gl_id vao);

                DE_API void bind_default();

                DE_API void raw_destroy(GLuint vao);
                DE_API bool destroy(gl_id vao);
                DE_API bool destroy(const char *name);

                DE_API void enum_vaos(enum_callback callback, mem_ptr args);

                DE_API vao_item *get(uint64_t keyName);

                DE_API GLuint currently_bound();
                DE_API gl_id current_id();

            private:
                vao_manager();

                GLuint m_CurrentlyBound;
                gl_id m_CurrentID;
                hash_table<vao_item> m_VAOs;

            public:
                 vao_manager(const vao_manager &) = delete;
                 vao_manager(vao_manager &&) = delete;
        };

        /*
        ================
        vao_manager::get
        ================
        */
        inline vao_manager::vao_item *vao_manager::get(uint64_t keyName)
        {
            hash_entry<vao_item> *hs = m_VAOs[keyName];
            if(hs == nullptr)
                return nullptr;

            return &hs->value;
        }

        /*
        ============================
        vao_manager::currently_bound
        ============================
        */
        inline GLuint vao_manager::currently_bound()
        {
            return m_CurrentlyBound;
        }

        /*
        =======================
        vao_manager::current_id
        =======================
        */
        inline gl_id vao_manager::current_id()
        {
            return m_CurrentID;
        }

    }

}

#endif