#include "DE/drivers/opengl/error.hpp"

#include <stdio.h>

namespace deep
{

    namespace GL3
    {

        /*
        ======================
        gl_error::clear_errors
        ======================
        */
        void gl_error::clear_errors()
        {
            while(glGetError() != GL_NO_ERROR);
        }

        /*
        ======================
        gl_error::check_errors
        ======================
        */
        bool gl_error::check_errors(const char *function, const char *file, unsigned int line)
        {
            GLenum error;
            bool ret = false;

            while((error = glGetError()) != GL_NO_ERROR)
            {
                fprintf(stderr, "[gl_error] (%d %s) %s %s: %u\n", error, get_name(error), function, file, line);
                ret = true;
            }

            return ret;	
        }

        /*
        ==================
        gl_error::get_name
        ==================
        */
        const char *gl_error::get_name(GLenum errorCode)
        {
            switch (errorCode)
            {
                default:                               return "UNKNOWN";
                case GL_NO_ERROR:                      return "NO_ERROR";
                case GL_INVALID_ENUM:                  return "INVALID_ENUM";
                case GL_INVALID_VALUE:                 return "INVALID_VALUE";
                case GL_INVALID_OPERATION:             return "INVALID_OPERATION";
                case GL_OUT_OF_MEMORY:                 return "OUT_OF_MEMORY";
                case GL_INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION";
            }
        }

    }

}