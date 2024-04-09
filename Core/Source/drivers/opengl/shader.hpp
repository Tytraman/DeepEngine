#ifndef __DEEP_ENGINE_OPENGL_SHADER_HPP__
#define __DEEP_ENGINE_OPENGL_SHADER_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/templates/hash_table.hpp"
#include "core/templates/pair.hpp"
#include "maths/vec.hpp"
#include "maths/mat.hpp"
#include "drivers/opengl/def.hpp"

namespace deep
{

    namespace GL3
    {

        class program_manager;

        class shader_manager
        {

            public:
                enum class gl_shader_type : GLenum
                {
                    Fragment = GL_FRAGMENT_SHADER,
                    Vertex   = GL_VERTEX_SHADER
                };

            public:
                DE_API static shader_manager *get_singleton();

                DE_API gl_id create(const char *name, gl_shader_type shaderType);

                DE_API void raw_load(GLuint shader, memory_chunk &program);
                DE_API void raw_load(GLuint shader, mem_ptr buffer, size_t size);

                DE_API bool load(gl_id shader, memory_chunk &program);
                DE_API bool load(const char *name, memory_chunk &program);
                DE_API bool load(gl_id shader, mem_ptr buffer, size_t size);

                DE_API bool raw_compile(GLuint shader);
                DE_API bool compile(gl_id shader);
                DE_API bool compile(const char *name);

                DE_API void raw_destroy(GLuint shader);
                DE_API bool destroy(gl_id shader);
                DE_API bool destroy(const char *name);

            private:
                shader_manager();

                hash_table<GLuint> m_Shaders;

            public:
                friend program_manager;

                shader_manager(const shader_manager &) = delete;
                shader_manager(shader_manager &&) = delete;

        };

        class program_manager
        {
            public:
                struct program_item
                {
                    string name;
                    GLuint program;
                    hash_table<pair<int, float>> fUniforms;
                    hash_table<pair<int, int>> iUniforms;
                    hash_table<pair<int, vec3<float>>> fv3Uniforms;
                    hash_table<pair<int, vec4<float>>> fv4Uniforms;
                    hash_table<pair<int, mat4x4<float>>> fm4Uniforms;

                    DE_API program_item(const char *name, GLuint program);
                };

                using enum_callback = void (*)(gl_id programID, program_item *program, mem_ptr args);

            public:
                DE_API static program_manager *get_singleton();

                DE_API gl_id create(const char *name);

                DE_API void raw_attach_shader(GLuint program, GLuint shader);
                DE_API bool attach_shader(gl_id program, gl_id shader);
                DE_API bool attach_shader(const char *progName, const char *shadName);
                DE_API bool attach_shader(const char *progName, gl_id shader);
                DE_API bool attach_shader(gl_id program, const char *shadName);

                /// @brief Ajoute une entrée d'uniforme dans la liste interne liée au programme actuellement attaché.
                /// @param uniformName Le nom de l'uniforme à ajouter.
                /// @param location La localisation de l'uniforme dans le programme du shader.
                /// @param value La valeur à attribuer.
                /// @return \c true en cas de succès, \c dans le cas inverse.
                DE_API bool add_uniform(const char *uniformName, int location, float value);
                /// @brief Ajoute une entrée d'uniforme dans la liste interne liée au programme actuellement attaché.
                /// @param uniformName Le nom de l'uniforme à ajouter.
                /// @param location La localisation de l'uniforme dans le programme du shader.
                /// @param value La valeur à attribuer.
                /// @return \c true en cas de succès, \c dans le cas inverse.
                DE_API bool add_uniform(const char *uniformName, int location, int value);
                /// @brief Ajoute une entrée d'uniforme dans la liste interne liée au programme actuellement attaché.
                /// @param uniformName Le nom de l'uniforme à ajouter.
                /// @param location La localisation de l'uniforme dans le programme du shader.
                /// @param value La valeur à attribuer.
                /// @return \c true en cas de succès, \c dans le cas inverse.
                DE_API bool add_uniform(const char *uniformName, int location, const mat4x4<float> &value);
                /// @brief Ajoute une entrée d'uniforme dans la liste interne liée au programme actuellement attaché.
                /// @param uniformName Le nom de l'uniforme à ajouter.
                /// @param location La localisation de l'uniforme dans le programme du shader.
                /// @param value La valeur à attribuer.
                /// @return \c true en cas de succès, \c dans le cas inverse.
                DE_API bool add_uniform(const char *uniformName, int location, const vec3<float> &value);
                /// @brief Ajoute une entrée d'uniforme dans la liste interne liée au programme actuellement attaché.
                /// @param uniformName Le nom de l'uniforme à ajouter.
                /// @param location La localisation de l'uniforme dans le programme du shader.
                /// @param value La valeur à attribuer.
                /// @return \c true en cas de succès, \c dans le cas inverse.
                DE_API bool add_uniform(const char *uniformName, int location, const vec4<float> &value);


                /// @brief Modifie la valeur attribuée à un uniforme lié au programme actuellement attaché.
                /// @param uniformName Le nom de l'uniforme à modifier.
                /// @param value La valeur à attribuer.
                /// @return \c true en cas de succès, \c false dans le cas inverse.
                DE_API bool set_uniform(const char *uniformName, float value);
                /// @brief Modifie la valeur attribuée à un uniforme lié au programme actuellement attaché.
                /// @param uniformName Le nom de l'uniforme à modifier.
                /// @param value La valeur à attribuer.
                /// @return \c true en cas de succès, \c false dans le cas inverse.
                DE_API bool set_uniform(const char *uniformName, int value);
                /// @brief Modifie la valeur attribuée à un uniforme lié au programme actuellement attaché.
                /// @param uniformName Le nom de l'uniforme à modifier.
                /// @param value La valeur à attribuer.
                /// @return \c true en cas de succès, \c false dans le cas inverse.
                DE_API bool set_uniform(const char *uniformName, const vec3<float> &value);
                /// @brief Modifie la valeur attribuée à un uniforme lié au programme actuellement attaché.
                /// @param uniformName Le nom de l'uniforme à modifier.
                /// @param value La valeur à attribuer.
                /// @return \c true en cas de succès, \c false dans le cas inverse.
                DE_API bool set_uniform(const char *uniformName, const vec4<float> &value);
                /// @brief Modifie la valeur attribuée à un uniforme lié au programme actuellement attaché.
                /// @param uniformName Le nom de l'uniforme à modifier.
                /// @param value La valeur à attribuer.
                /// @return \c true en cas de succès, \c false dans le cas inverse.
                DE_API bool set_uniform(const char *uniformName, const mat4x4<float> &value);

                DE_API bool set_uniform(gl_id program, const char *uniformName, float value);
                DE_API bool set_uniform(gl_id program, const char *uniformName, const vec3<float> &value);
                

                DE_API bool raw_link(GLuint program);
                DE_API bool link(gl_id program);
                DE_API bool link(const char *name);

                DE_API void raw_use(GLuint program);
                DE_API bool use(gl_id program);
                DE_API bool use(const char *name);

                DE_API void raw_destroy(GLuint program);
                DE_API bool destroy(gl_id program);
                DE_API bool destroy(const char *name);

                /// @brief Envoie les valeurs de tous les uniformes liés au programme actuellement attaché.
                /// @return \c true en cas de succès, \c false dans le cas inverse.
                DE_API bool send_uniforms();
                DE_API void destroy_all_programs();

                DE_API bool exists(gl_id program);
                DE_API bool exists(const char *name);

                DE_API void enum_programs(enum_callback callback, mem_ptr args);

                DE_API program_item *get(gl_id program);
                DE_API program_item *get(const char *name);

                DE_API GLuint currently_bound();
                DE_API gl_id current_id();

                DE_API hash_function get_hash_function();

            private:
                program_manager();

                GLuint m_CurrentlyBound;
                gl_id m_CurrentID;
                hash_table<program_item> m_Programs;

            public:
                program_manager(const program_manager &) = delete;
                program_manager(program_manager &&) = delete;

        };

        /*
        =======================
        program_manager::exists
        =======================
        */
        inline bool program_manager::exists(gl_id program)
        {
            return m_Programs[program] != nullptr;
        }

        /*
        =======================
        program_manager::exists
        =======================
        */
        inline bool program_manager::exists(const char *name)
        {
            return m_Programs[name] != nullptr;
        }

        /*
        ====================
        program_manager::get
        ====================
        */
        inline program_manager::program_item *program_manager::get(gl_id program)
        {
            hash_entry<program_item> *hs = m_Programs[program];
            if(hs == nullptr)
                return nullptr;

            return &hs->value;
        }

        /*
        ====================
        program_manager::get
        ====================
        */
        inline program_manager::program_item *program_manager::get(const char *name)
        {
            return get(m_Programs.getHashFunction()(name));
        }

        /*
        ===============================
        program_manager::currently_bound
        ===============================
        */
        inline GLuint program_manager::currently_bound()
        {
            return m_CurrentlyBound;
        }

        /*
        ==========================
        program_manager::current_id
        ==========================
        */
        inline gl_id program_manager::current_id()
        {
            return m_CurrentID;
        }

        /*
        ================================
        program_manager::get_hash_function
        ================================
        */
        inline hash_function program_manager::get_hash_function()
        {
            return m_Programs.getHashFunction();
        }

    }

}


#endif