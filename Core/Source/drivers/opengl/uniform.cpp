#include "drivers/opengl/uniform.hpp"
#include "drivers/opengl/shader.hpp"

namespace deep
{

    namespace GL3
    {

        /*
        ========================
        uniform_manager::raw_find
        ========================
        */
        int uniform_manager::raw_find(GLuint program, const char *name)
        {
            int uni = DE_GL_CALLV(glGetUniformLocation(program, name));
            return uni;
        }

        /*
        =====================
        uniform_manager::find
        =====================
        */
        int uniform_manager::find(gl_id program, const char *name)
        {
            program_manager *programManager = program_manager::get_singleton();

            program_manager::program_item *el = programManager->get(program);
            if(el == nullptr)
                return -1;

            return raw_find(el->program, name);
        }

        /*
        =====================
        uniform_manager::find
        =====================
        */
        int uniform_manager::find(const char *progName, const char *name)
        {
            program_manager *programManager = program_manager::get_singleton();

            program_manager::program_item *el = programManager->get(progName);
            if(el == nullptr)
                return -1;

            return raw_find(el->program, name);
        }

        /*
        =====================
        uniform_manager::send
        =====================
        */
        void uniform_manager::send(int location, float value)
        {
            DE_GL_CALL(glUniform1f(location, value));
        }

        /*
        =====================
        uniform_manager::send
        =====================
        */
        void uniform_manager::send(int location, int value)
        {
            DE_GL_CALL(glUniform1i(location, value));
        }

        /*
        =====================
        uniform_manager::send
        =====================
        */
        void uniform_manager::send(int location, const vec2<float> &vec)
        {
            DE_GL_CALL(glUniform2f(location, vec.x, vec.y));
        }

        /*
        =====================
        uniform_manager::send
        =====================
        */
        void uniform_manager::send(int location, const vec3<float> &vec)
        {
            DE_GL_CALL(glUniform3f(location, vec.x, vec.y, vec.z));
        }

        /*
        =====================
        uniform_manager::send
        =====================
        */
        void uniform_manager::send(int location, const vec4<float> &vec)
        {
            DE_GL_CALL(glUniform4f(location, vec.x, vec.y, vec.z, vec.w));
        }

        /*
        =====================
        uniform_manager::send
        =====================
        */
        void uniform_manager::send(int location, fmat3x3 &mat)
        {
            DE_GL_CALL(glUniformMatrix3fv(location, 1, GL_TRUE, mat.ptr()));
        }

        /*
        =====================
        uniform_manager::send
        =====================
        */
        void uniform_manager::send(int location, fmat4x4 &mat)
        {
            DE_GL_CALL(glUniformMatrix4fv(location, 1, GL_TRUE, mat.ptr()));
        }

    }

}
