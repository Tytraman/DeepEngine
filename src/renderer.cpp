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
	Renderer::Renderer
	==================
	*/
	Renderer::Renderer()
		: _renderer(NULL)
	{ }

	/*
	================
	Renderer::create
	================
	*/
	ErrorStatus Renderer::create(Renderer &dest, Window &window)
	{
		dest._renderer = SDL_CreateRenderer(window.getWindow(), -1, SDL_RENDERER_ACCELERATED);
		if(dest._renderer == NULL)
			return ErrorStatus::CreateRendererSDL;

		return ErrorStatus::NoError;
	}

	/*
	===============
	Renderer::clear
	===============
	*/
	void Renderer::clear() const
	{
			SDL_RenderClear(_renderer);
	}

	/*
	==================
	Renderer::setColor
	==================
	*/
	void Renderer::setColor(const colora &color)
	{
		_color = color;
		SDL_SetRenderDrawColor(_renderer, color.R, color.G, color.B, color.A);
	}

	/*
	===================
	Renderer::drawPixel
	===================
	*/
	void Renderer::drawPixel(const vec2 &pos) const
	{
		SDL_RenderDrawPoint(_renderer, pos.x, pos.y);
	}

	/*
	===================
	Renderer::drawPixel
	===================
	*/
	void Renderer::drawPixel(const fvec2 &pos) const
	{
		SDL_RenderDrawPoint(_renderer, (int) pos.x, (int) pos.y);
	}

	/*
	==================
	Renderer::drawLine
	==================
	*/
	bool Renderer::drawLine(int x1, int y1, int x2, int y2) const
	{
		if(SDL_RenderDrawLine(_renderer, x1, y1, x2, y2) != 0) {
			fprintf(stderr, "SDL_RenderDrawLine error: %s", SDL_GetError());

			return false;
		}
	
		return true;
	}

	/*
	===================
	Renderer::drawShape
	===================
	*/
	void Renderer::drawShape(const SDL_Vertex *vertices, int verticesNumber) const
	{
		SDL_RenderGeometry(_renderer, nullptr, vertices, verticesNumber, nullptr, 0);
	}

	/*
	=====================
	Renderer::swapBuffers
	=====================
	*/
	void Renderer::swapBuffers() const
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(_renderer);
	}

	/*
	=================
	Renderer::destroy
	=================
	*/
	void Renderer::destroy()
	{
		SDL_DestroyRenderer(_renderer);
		_renderer = NULL;
	}

	OpenGLRenderer::OpenGLRenderer()
		: m_Context(NULL),
		  m_Window(NULL)
	{ }

	bool OpenGLRenderer::create(OpenGLRenderer &dest, Window &window)
	{
		// Spécifie le profil "Core" d'OpenGL.
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// Spécifie l'utilisation de la version 3.3 d'OpenGL.
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		dest.m_Context = SDL_GL_CreateContext(window.getWindow());

		if(dest.m_Context == NULL)
			return false;

		dest.m_Window = window.getWindow();

		return true;
	}

	void OpenGLRenderer::clear() const
	{
		DE_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	}

	void OpenGLRenderer::draw(unsigned int numberOfVertices) const
	{
		DE_GL_CALL(glDrawArrays(GL_TRIANGLES, 0, numberOfVertices));
	}

	void OpenGLRenderer::swapBuffers() const
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(m_Window);
	}

	void OpenGLRenderer::setClearColor(const colora color)
	{
		m_ClearColor = color;
		DE_GL_CALL(glClearColor(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f));
	}

}

