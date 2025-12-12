#include "drivers/opengl/renderer.hpp"
#include "os/window.hpp"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

namespace deep
{

    namespace GL3
    {

        gl_renderer::gl_renderer()
            : m_Context(NULL),
            m_Window(NULL)
        { }

        bool gl_renderer::create(gl_renderer &dest, window *window)
        {
            // Spécifie le profil "Core" d'OpenGL.
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

            // Spécifie l'utilisation de la version 3.3 d'OpenGL.
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

            dest.m_Context = SDL_GL_CreateContext(window->get_window());

            if (dest.m_Context == NULL)
                return false;

            dest.m_Window = window;

            return true;
        }

        void gl_renderer::clear() const
        {
            DE_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        }

        void gl_renderer::clear_color() const
        {
            DE_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
        }

        void gl_renderer::draw(unsigned int numberOfVertices) const
        {
            DE_GL_CALL(glDrawArrays(GL_TRIANGLES, 0, numberOfVertices));
        }

        void gl_renderer::swap_buffers() const
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(m_Window->get_window());
        }

        void gl_renderer::set_clear_color(const vec4<uint8_t> &color)
        {
            m_ClearColor = color;
            DE_GL_CALL(glClearColor(color.x / 255.0f, color.y / 255.0f, color.z / 255.0f, color.w / 255.0f));
        }

    }

}
