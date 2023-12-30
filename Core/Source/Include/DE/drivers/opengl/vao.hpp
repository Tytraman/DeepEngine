#ifndef __DEEP_ENGINE_OPENGL_VAO_HPP__
#define __DEEP_ENGINE_OPENGL_VAO_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/memory/hash_table.hpp"
#include "DE/drivers/opengl/def.hpp"

namespace deep
{

    namespace GL3
    {

        class vao_manager
        {

            public:
                DE_API static vao_manager *get_singleton();

                DE_API gl_id create(const char *name);

                DE_API void raw_bind(GLuint vao);
                DE_API bool bind(gl_id vao);
                DE_API bool bind(const char *name);

                DE_API void bind_default();

                DE_API void raw_destroy(GLuint vao);
                DE_API bool destroy(gl_id vao);
                DE_API bool destroy(const char *name);

                DE_API GLuint currently_bound();
                DE_API gl_id current_id();

            private:
                vao_manager();

                GLuint m_CurrentlyBound;
                gl_id m_CurrentID;
                hash_table<GLuint> m_VAOs;

            public:
                 vao_manager(const vao_manager &) = delete;
                 vao_manager(vao_manager &&) = delete;
        };

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