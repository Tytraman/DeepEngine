#ifndef __DEEP_ENGINE_OPENGL_FRAMERENDERBUFFER_HPP__
#define __DEEP_ENGINE_OPENGL_FRAMERENDERBUFFER_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/drivers/opengl/def.hpp"

namespace deep
{

    namespace GL3
    {

        class framerenderbuffer
        {

            public:
                DE_API bool create(const char *name, int width, int height);

                DE_API void destroy();

                DE_API bool saveTextureAsImage(const char *destpath);

                DE_API bool resize(const char *newName, int width, int height);

                DE_API gl_id framebuffer() const;
                DE_API gl_id renderbuffer() const;
                DE_API gl_id vbo() const;
                DE_API gl_id vao() const;
                DE_API int width() const;
                DE_API int height() const;

            private:
                gl_id m_Framebuffer;
                gl_id m_Renderbuffer;
                gl_id m_VBO;
                gl_id m_VAO;
                int m_Width;
                int m_Height;

        };

        /*
        ==============================
        framerenderbuffer::framebuffer
        ==============================
        */
        inline gl_id framerenderbuffer::framebuffer() const
        {
            return m_Framebuffer;
        }

        /*
        ===============================
        framerenderbuffer::renderbuffer
        ===============================
        */
        inline gl_id framerenderbuffer::renderbuffer() const
        {
            return m_Renderbuffer;
        }

        /*
        ======================
        framerenderbuffer::vbo
        ======================
        */
        inline gl_id framerenderbuffer::vbo() const
        {
            return m_VBO;
        }

        /*
        ======================
        framerenderbuffer::vao
        ======================
        */
        inline gl_id framerenderbuffer::vao() const
        {
            return m_VAO;
        }

        /*
        ========================
        framerenderbuffer::width
        ========================
        */
        inline int framerenderbuffer::width() const
        {
            return m_Width;
        }

        /*
        =========================
        framerenderbuffer::height
        =========================
        */
        inline int framerenderbuffer::height() const
        {
            return m_Height;
        }

    }

}

#endif