#include "DE/graphics/renderer.hpp"
#include "DE/core/window.hpp"

#include <glad/glad.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include <stdio.h>

namespace deep
{

    /*
    ==================
    renderer::renderer
    ==================
    */
    renderer::renderer()
        : m_Renderer(NULL)
    { }

    /*
    ================
    renderer::create
    ================
    */
    bool renderer::create(renderer &dest, window &window)
    {
        dest.m_Renderer = SDL_CreateRenderer(window.get_window(), -1, SDL_RENDERER_ACCELERATED);
        if(dest.m_Renderer == NULL)
        {
            return false;
        }

        return true;
    }

    /*
    ===============
    renderer::clear
    ===============
    */
    void renderer::clear() const
    {
        SDL_RenderClear(m_Renderer);
    }

    /*
    =====================
    renderer::swap_buffers
    =====================
    */
    void renderer::swapBuffers() const
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(m_Renderer);
    }

    /*
    =================
    renderer::destroy
    =================
    */
    void renderer::destroy()
    {
        SDL_DestroyRenderer(m_Renderer);
        m_Renderer = NULL;
    }

}

