#ifndef __DEEP_ENGINE_OPENGL_DEF_HPP__
#define __DEEP_ENGINE_OPENGL_DEF_HPP__

#include "drivers/opengl/error.hpp"

#define DE_GL_CALLV(_x)                                                                     \
    [&]() {                                                                                 \
        deep::GL3::gl_error::clear_errors();                                                 \
        auto ret = _x;                                                                      \
        if(deep::GL3::gl_error::check_errors(#_x, DE_FILE_NAME, DE_LINE)) DE_DEBUG_BREAK;    \
        return ret;                                                                         \
    }()

#define DE_GL_CALL(_x)                                                                      \
    deep::GL3::gl_error::clear_errors();                                                     \
    _x;                                                                                     \
    if(deep::GL3::gl_error::check_errors(#_x, DE_FILE_NAME, DE_LINE)) DE_DEBUG_BREAK;

namespace deep
{

    namespace GL3
    {
        using gl_id = uint64_t;

        enum class gl_type
        {
            Byte  = GL_UNSIGNED_BYTE,
            Float = GL_FLOAT,
            Int   = GL_INT,
            UInt  = GL_UNSIGNED_INT
        };

        enum class gl_attrib_components_number : GLint
        {
            x1 = 1,
            x2 = 2,
            x3 = 3,
            x4 = 4
        };

    }

}

#endif