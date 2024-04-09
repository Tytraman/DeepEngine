#ifndef __DEEP_ENGINE_OPENGL_UNIFORM_HPP__
#define __DEEP_ENGINE_OPENGL_UNIFORM_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "maths/vec.hpp"
#include "maths/mat.hpp"
#include "drivers/opengl/def.hpp"

namespace deep
{

    namespace GL3
    {

        class uniform_manager
        {

            public:
                DE_API static int raw_find(GLuint program, const char *name);
                DE_API static int find(gl_id program, const char *name);
                DE_API static int find(const char *progName, const char *name);

                DE_API static void send(int location, float value);
                DE_API static void send(int location, int value);
                DE_API static void send(int location, const vec2<float> &vec);
                DE_API static void send(int location, const vec3<float> &vec);
                DE_API static void send(int location, const vec4<float> &vec);
                DE_API static void send(int location, const mat3x3<float> &mat);
                DE_API static void send(int location, const mat4x4<float> &mat);

            public:
                uniform_manager() = delete;
                uniform_manager(const uniform_manager &) = delete;
                uniform_manager(uniform_manager &&) = delete;

        };

    }

}

#endif