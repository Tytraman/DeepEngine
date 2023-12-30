#include "DE/renderer.hpp"
#include "DE/window.hpp"

#include <glad/glad.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include <stdio.h>

namespace deep {

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
    error_status renderer::create(renderer &dest, window &window)
    {
        dest.m_Renderer = SDL_CreateRenderer(window.getWindow(), -1, SDL_RENDERER_ACCELERATED);
        if(dest.m_Renderer == NULL)
            return error_status::CreateRendererSDL;

        return error_status::NoError;
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
    ==================
    renderer::setColor
    ==================
    */
    void renderer::setColor(const colora &color)
    {
        m_Color = color;
        SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);
    }

    /*
    ===================
    renderer::drawPixel
    ===================
    */
    void renderer::drawPixel(const vec2 &pos) const
    {
        SDL_RenderDrawPoint(m_Renderer, pos.x, pos.y);
    }

    /*
    ===================
    renderer::drawPixel
    ===================
    */
    void renderer::drawPixel(const fvec2 &pos) const
    {
        SDL_RenderDrawPoint(m_Renderer, (int) pos.x, (int) pos.y);
    }

    /*
    ==================
    renderer::drawLine
    ==================
    */
    bool renderer::drawLine(int x1, int y1, int x2, int y2) const
    {
        if(SDL_RenderDrawLine(m_Renderer, x1, y1, x2, y2) != 0) {
            fprintf(stderr, "SDL_RenderDrawLine error: %s", SDL_GetError());

            return false;
        }
    
        return true;
    }

    /*
    ===================
    renderer::drawShape
    ===================
    */
    void renderer::drawShape(const SDL_Vertex *vertices, int verticesNumber) const
    {
        SDL_RenderGeometry(m_Renderer, nullptr, vertices, verticesNumber, nullptr, 0);
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

