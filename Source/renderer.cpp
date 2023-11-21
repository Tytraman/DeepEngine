#include <DE/renderer.hpp>
#include <DE/window.hpp>
#include <DE/rendering/opengl_utils.hpp>

#include <glad/glad.h>

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

namespace de {

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
	renderer::swapBuffers
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

		dest.m_Context = SDL_GL_CreateContext(window->getWindow());

		if(dest.m_Context == NULL)
			return false;

		dest.m_Window = window;

		return true;
	}

	void gl_renderer::clear() const
	{
		DE_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

    void gl_renderer::clearColor() const
    {
        DE_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }

	void gl_renderer::draw(unsigned int numberOfVertices) const
	{
		DE_GL_CALL(glDrawArrays(GL_TRIANGLES, 0, numberOfVertices));
	}

	void gl_renderer::swapBuffers() const
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(m_Window->getWindow());
	}

	void gl_renderer::setClearColor(const colora color)
	{
		m_ClearColor = color;
		DE_GL_CALL(glClearColor(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f));
	}

}

