#ifndef __DEEP_ENGINE_OPENGL_RENDERER_HPP__
#define __DEEP_ENGINE_OPENGL_RENDERER_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/drivers/opengl/def.hpp"

namespace deep
{
    class window;

    namespace GL3
    {

        class gl_renderer
        {

            public:
                DE_API gl_renderer();

                DE_API static bool create(gl_renderer &dest, window *window);

                DE_API void clear() const;
                DE_API void clear_color() const;
                DE_API void draw(unsigned int numberOfVertices) const;
                DE_API void swap_buffers() const;

                DE_API SDL_GLContext get_context();
                DE_API window *get_window();

                DE_API colora get_clear_color() const;
                DE_API void set_clear_color(const colora color);

            private:
                SDL_GLContext m_Context;
                window *m_Window;

                colora m_ClearColor;

        };

        /*
        ========================
        gl_renderer::get_context
        ========================
        */
        inline SDL_GLContext gl_renderer::get_context()
        {
            return m_Context;
        }

        /*
        =======================
        gl_renderer::get_window
        =======================
        */
        inline window *gl_renderer::get_window()
        {
            return m_Window;
        }

        /*
        ============================
        gl_renderer::get_clear_color
        ============================
        */
        inline colora gl_renderer::get_clear_color() const
        {
            return m_ClearColor;
        }

    }

}


#endif