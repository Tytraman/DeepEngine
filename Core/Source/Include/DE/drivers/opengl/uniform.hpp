#ifndef __DEEP_ENGINE_OPENGL_UNIFORM_HPP__
#define __DEEP_ENGINE_OPENGL_UNIFORM_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/maths/vec.hpp"
#include "DE/maths/mat.hpp"
#include "DE/drivers/opengl/def.hpp"

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
                DE_API static void send(int location, fmat3x3 &mat);
                DE_API static void send(int location, fmat4x4 &mat);

            public:
                uniform_manager() = delete;
                uniform_manager(const uniform_manager &) = delete;
                uniform_manager(uniform_manager &&) = delete;

        };

    }

}

#endif