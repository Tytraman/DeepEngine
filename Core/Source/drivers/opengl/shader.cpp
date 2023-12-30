#include "DE/drivers/opengl/shader.hpp"
#include "DE/drivers/opengl/uniform.hpp"

#include <stdio.h>

namespace deep
{

    namespace GL3
    {

        /*
        ==============================
        shader_manager::shader_manager
        ==============================
        */
        shader_manager::shader_manager()
        { }

        /*
        =============================
        shader_manager::get_singleton
        =============================
        */
        shader_manager *shader_manager::get_singleton()
        {
            static shader_manager singleton;

            return &singleton;
        }

        /*
        ======================
        shader_manager::create
        ======================
        */
        gl_id shader_manager::create(const char *name, gl_shader_type shaderType)
        {
            GLuint shader = DE_GL_CALLV(glCreateShader(static_cast<GLenum>(shaderType)));

            auto &el = m_Shaders.insert(name, shader);

            return el.key;
        }

        /*
        ======================
        shader_manager::raw_load
        ======================
        */
        void shader_manager::raw_load(GLuint shader, memory_chunk &program)
        {
            char *source = (char *) program.data();
            DE_GL_CALL(glShaderSource(shader, 1, &source, NULL));
        }

        /*
        ====================
        shader_manager::load
        ====================
        */
        bool shader_manager::load(gl_id shader, memory_chunk &program)
        {
            auto el = m_Shaders[shader];
            if(el == nullptr)
                return false;
        
            raw_load(el->value, program);

            return true;
        }

        /*
        ====================
        shader_manager::load
        ====================
        */
        bool shader_manager::load(const char *name, memory_chunk &program)
        {
            auto el = m_Shaders[name];
            if(el == nullptr)
                return false;
        
            raw_load(el->value, program);

            return true;
        }

        /*
        ==========================
        shader_manager::raw_compile
        ==========================
        */
        bool shader_manager::raw_compile(GLuint shader)
        {
            int  success;
            char infoLog[512];

            DE_GL_CALL(glCompileShader(shader));
            DE_GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));

            if(!success)
            {
                DE_GL_CALL(glGetShaderInfoLog(shader, 512, NULL, infoLog));
                fprintf(stderr, "GLSL compilation error:\n%s\n", infoLog);

                return false;
            }

            return true;
        }

        /*
        =======================
        shader_manager::compile
        =======================
        */
        bool shader_manager::compile(gl_id shader)
        {
            auto el = m_Shaders[shader];
            if(el == nullptr)
                return false;

            return raw_compile(el->value);
        }

        /*
        =======================
        shader_manager::compile
        =======================
        */
        bool shader_manager::compile(const char *name)
        {
            auto el = m_Shaders[name];
            if(el == nullptr)
                return false;

            return raw_compile(el->value);
        }

        /*
        ==========================
        shader_manager::raw_destroy
        ==========================
        */
        void shader_manager::raw_destroy(GLuint shader)
        {
            DE_GL_CALL(glDeleteShader(shader));
        }

        /*
        =======================
        shader_manager::destroy
        =======================
        */
        bool shader_manager::destroy(gl_id shader)
        {
            auto el = m_Shaders[shader];
            if(el == nullptr)
                return false;

            raw_destroy(el->value);
            m_Shaders.remove(el->key);

            return true;
        }

        /*
        =======================
        shader_manager::destroy
        =======================
        */
        bool shader_manager::destroy(const char *name)
        {
            auto el = m_Shaders[name];
            if(el == nullptr)
                return false;

            raw_destroy(el->value);
            m_Shaders.remove(el->key);

            return true;
        }

        void programs_hashtable_free_element_callback(program_manager::program_item &program)
        {
            program_manager *programManager = program_manager::get_singleton();

            programManager->raw_destroy(program.program);
        }

        /*
        ==========================
        program_item::program_item
        ==========================
        */
        program_manager::program_item::program_item(GLuint _program)
            : program(_program)
        { }

        /*
        ================================
        program_manager::program_manager
        ================================
        */
        program_manager::program_manager()
            : m_CurrentlyBound(0),
              m_CurrentID(0),
              m_Programs(100, string_utils::hash, programs_hashtable_free_element_callback)
        { }

        /*
        ==============================
        program_manager::get_singleton
        ==============================
        */
        program_manager *program_manager::get_singleton()
        {
            static program_manager singleton;

            return &singleton;
        }

        /*
        =======================
        program_manager::create
        =======================
        */
        gl_id program_manager::create(const char *name)
        {
            GLuint program = DE_GL_CALLV(glCreateProgram());

            auto &el = m_Programs.insert(name, program);

            return el.key;
        }

        /*
        ================================
        program_manager::raw_attach_shader
        ================================
        */
        void program_manager::raw_attach_shader(GLuint program, GLuint shader)
        {
            DE_GL_CALL(glAttachShader(program, shader));
        }

        /*
        =============================
        program_manager::attach_shader
        =============================
        */
        bool program_manager::attach_shader(gl_id program, gl_id shader)
        {
            auto prog = m_Programs[program];
            if(prog == nullptr)
                return false;

            shader_manager *shaderManager = shader_manager::get_singleton();

            auto shad = shaderManager->m_Shaders[shader];
            if(shad == nullptr)
                return false;

            raw_attach_shader(prog->value.program, shad->value);

            return true;
        }

        /*
        =============================
        program_manager::attach_shader
        =============================
        */
        bool program_manager::attach_shader(const char *progName, const char *shadName)
        {
            auto prog = m_Programs[progName];
            if(prog == nullptr)
                return false;

            shader_manager *shaderManager = shader_manager::get_singleton();

            auto shad = shaderManager->m_Shaders[shadName];
            if(shad == nullptr)
                return false;

            raw_attach_shader(prog->value.program, shad->value);

            return true;
        }

        /*
        =============================
        program_manager::attach_shader
        =============================
        */
        bool program_manager::attach_shader(const char *progName, gl_id shader)
        {
            auto prog = m_Programs[progName];
            if(prog == nullptr)
                return false;

            shader_manager *shaderManager = shader_manager::get_singleton();

            auto shad = shaderManager->m_Shaders[shader];
            if(shad == nullptr)
                return false;

            raw_attach_shader(prog->value.program, shad->value);

            return true;
        }

        /*
        =============================
        program_manager::attach_shader
        =============================
        */
        bool program_manager::attach_shader(gl_id program, const char *shadName)
        {
            auto prog = m_Programs[program];
            if(prog == nullptr)
                return false;

            shader_manager *shaderManager = shader_manager::get_singleton();

            auto shad = shaderManager->m_Shaders[shadName];
            if(shad == nullptr)
                return false;

            raw_attach_shader(prog->value.program, shad->value);

            return true;
        }

        /*
        ===========================
        program_manager::add_uniform
        ===========================
        */
        bool program_manager::add_uniform(const char *uniformName, int location, float value)
        {
            auto prog = m_Programs[m_CurrentID];
            if(prog == nullptr)
                return false;

            prog->value.fUniforms.insert(uniformName, pair(location, value));

            return true;
        }

        /*
        ===========================
        program_manager::add_uniform
        ===========================
        */
        bool program_manager::add_uniform(const char *uniformName, int location, int value)
        {
            auto prog = m_Programs[m_CurrentID];
            if(prog == nullptr)
                return false;

            prog->value.iUniforms.insert(uniformName, pair(location, value));

            return true;
        }

        /*
        ===========================
        program_manager::add_uniform
        ===========================
        */
        bool program_manager::add_uniform(const char *uniformName, int location, const fvec3 &value)
        {
            auto prog = m_Programs[m_CurrentID];
            if(prog == nullptr)
                return false;

            prog->value.fv3Uniforms.insert(uniformName, pair(location, value));

            return true;
        }

        /*
        ===========================
        program_manager::add_uniform
        ===========================
        */
        bool program_manager::add_uniform(const char *uniformName, int location, const fmat4x4 &value)
        {
            auto prog = m_Programs[m_CurrentID];
            if(prog == nullptr)
                return false;

            prog->value.fm4Uniforms.insert(uniformName, pair(location, value));

            return true;
        }

        /*
        ===========================
        program_manager::set_uniform
        ===========================
        */
        bool program_manager::set_uniform(const char *uniformName, float value)
        {
            auto prog = m_Programs[m_CurrentID];
            if(prog == nullptr)
                return false;

            auto uni = prog->value.fUniforms[uniformName];
            if(uni == nullptr)
                return false;

            uni->value.value2() = value;

            return true;
        }

        /*
        ===========================
        program_manager::set_uniform
        ===========================
        */
        bool program_manager::set_uniform(const char *uniformName, int value)
        {
            auto prog = m_Programs[m_CurrentID];
            if(prog == nullptr)
                return false;

            auto uni = prog->value.iUniforms[uniformName];
            if(uni == nullptr)
                return false;

            uni->value.value2() = value;

            return true;
        }

        /*
        ===========================
        program_manager::set_uniform
        ===========================
        */
        bool program_manager::set_uniform(const char *uniformName, const fvec3 &value)
        {
            auto prog = m_Programs[m_CurrentID];
            if(prog == nullptr)
                return false;

            auto uni = prog->value.fv3Uniforms[uniformName];
            if(uni == nullptr)
                return false;

            uni->value.value2() = value;

            return true;
        }

        /*
        ===========================
        program_manager::set_uniform
        ===========================
        */
        bool program_manager::set_uniform(const char *uniformName, const fmat4x4 &value)
        {
            auto prog = m_Programs[m_CurrentID];
            if(prog == nullptr)
                return false;

            auto uni = prog->value.fm4Uniforms[uniformName];
            if(uni == nullptr)
                return false;

            uni->value.value2() = value;

            return true;
        }

        /*
        ========================
        program_manager::raw_link
        ========================
        */
        bool program_manager::raw_link(GLuint program)
        {
            int  success;
            char infoLog[512];

            DE_GL_CALL(glLinkProgram(program));
            DE_GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &success));

            if(!success)
            {
                DE_GL_CALL(glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog));
                fprintf(stderr, "GLSL link error:\n%s\n", infoLog);

                return false;
            }

            return true;
        }

        /*
        =====================
        program_manager::link
        =====================
        */
        bool program_manager::link(gl_id program)
        {
            auto el = m_Programs[program];
            if(el == nullptr)
                return false;

            return raw_link(el->value.program);
        }

        /*
        =====================
        program_manager::link
        =====================
        */
        bool program_manager::link(const char *name)
        {
            auto el = m_Programs[name];
            if(el == nullptr)
                return false;

            return raw_link(el->value.program);
        }

        /*
        =======================
        program_manager::raw_use
        =======================
        */
        void program_manager::raw_use(GLuint program)
        {
            DE_GL_CALL(glUseProgram(program));
            m_CurrentlyBound = program;
        }

        /*
        ====================
        program_manager::use
        ====================
        */
        bool program_manager::use(gl_id program)
        {
            auto el = m_Programs[program];
            if(el == nullptr)
                return false;

            raw_use(el->value.program);
            m_CurrentID = program;

            return true;
        }

        /*
        ====================
        program_manager::use
        ====================
        */
        bool program_manager::use(const char *name)
        {
            auto el = m_Programs[name];
            if(el == nullptr)
                return false;

            raw_use(el->value.program);
            m_CurrentID = el->key;

            return true;
        }

        /*
        ===========================
        program_manager::raw_destroy
        ===========================
        */
        void program_manager::raw_destroy(GLuint program)
        {
            DE_GL_CALL(glDeleteProgram(program));
        }

        /*
        ========================
        program_manager::destroy
        ========================
        */
        bool program_manager::destroy(gl_id program)
        {
            auto el = m_Programs[program];
            if(el == nullptr)
                return false;

            raw_destroy(el->value.program);
            m_Programs.remove(el->key);

            return true;
        }

        /*
        ========================
        program_manager::destroy
        ========================
        */
        bool program_manager::destroy(const char *name)
        {
            auto el = m_Programs[name];
            if(el == nullptr)
                return false;

            raw_destroy(el->value.program);
            m_Programs.remove(el->key);

            return true;
        }

        /*
        =============================
        program_manager::send_uniforms
        =============================
        */
        bool program_manager::send_uniforms()
        {
            auto prog = m_Programs[m_CurrentID];
            if(prog == nullptr)
                return false;

            auto &fUniBeg = prog->value.fUniforms.begin();
            auto &fUniEnd = prog->value.fUniforms.end();
            for(; fUniBeg != fUniEnd; ++fUniBeg)
                uniform_manager::send(fUniBeg->value.value1(), fUniBeg->value.value2());

            auto &iUniBeg = prog->value.iUniforms.begin();
            auto &iUniEnd = prog->value.iUniforms.end();
            for(; iUniBeg != iUniEnd; ++iUniBeg)
                uniform_manager::send(iUniBeg->value.value1(), iUniBeg->value.value2());

            auto &fv3UniBeg = prog->value.fv3Uniforms.begin();
            auto &fv3UniEnd = prog->value.fv3Uniforms.end();
            for(; fv3UniBeg != fv3UniEnd; ++fv3UniBeg)
                uniform_manager::send(fv3UniBeg->value.value1(), fv3UniBeg->value.value2());

            auto &fm4UniBeg = prog->value.fm4Uniforms.begin();
            auto &fm4UniEnd = prog->value.fm4Uniforms.end();
            for(; fm4UniBeg != fm4UniEnd; ++fm4UniBeg)
                uniform_manager::send(fm4UniBeg->value.value1(), fm4UniBeg->value.value2());

            return true;
        }

        /*
        ===================================
        program_manager::destroy_all_programs
        ===================================
        */
        void program_manager::destroy_all_programs()
        {
            auto &beg = m_Programs.begin();
            auto &end = m_Programs.end();

            for(; beg != end; ++beg)
                raw_destroy(beg->value.program);

            m_Programs.clear();
        }

    }

}

