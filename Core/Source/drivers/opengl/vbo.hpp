#ifndef __DEEP_ENGINE_OPENGL_VBO_HPP__
#define __DEEP_ENGINE_OPENGL_VBO_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/templates/hash_table.hpp"
#include "core/templates/pair.hpp"
#include "drivers/opengl/def.hpp"

namespace deep
{

    namespace GL3
    {

        /// @brief Vertex Buffer Object
        class vbo_manager
        {

            public:
                DE_API static vbo_manager *get_singleton();

                DE_API gl_id create(const char *name);

                DE_API void raw_bind(GLuint vbo);
                DE_API bool bind(gl_id vbo);
                DE_API bool bind(const char *name);

                DE_API void bind_default();

                DE_API void raw_destroy(GLuint vbo);
                DE_API bool destroy(gl_id vbo);
                DE_API bool destroy(const char *name);

                DE_API void add_attribute(unsigned int index, gl_attrib_components_number componentsNumber, gl_type type, int stride, int offset);
                DE_API void set_vertices_number(unsigned int number);

                DE_API void transmit_data(memory_chunk &data);

                /// @brief  Récupère le nombre de sommets que le VBO possède.
                /// @return Le nombre de sommets que le VBO possède.
                DE_API unsigned int get_vertices_number();

                DE_API unsigned int get_vertices_number(gl_id vbo);

                DE_API GLuint currently_bound();
                DE_API gl_id current_id();

            private:
                vbo_manager();

                GLuint m_CurrentlyBound;
                gl_id m_CurrentID;
                hash_table<pair<GLuint, unsigned int>> m_VBOs;

            public:
                vbo_manager(const vbo_manager &) = delete;
                vbo_manager(vbo_manager &&) = delete;

        };

        /*
        ============================
        vbo_manager::currently_bound
        ============================
        */
        inline GLuint vbo_manager::currently_bound()
        {
            return m_CurrentlyBound;
        }

        /*
        =======================
        vbo_manager::current_id
        =======================
        */
        inline gl_id vbo_manager::current_id()
        {
            return m_CurrentID;
        }

    }

}


#endif