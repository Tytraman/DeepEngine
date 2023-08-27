#ifndef __DEEP_ENGINE_RENDERER_HPP__
#define __DEEP_ENGINE_RENDERER_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/error.hpp>
#include <DE/color.hpp>
#include <DE/vec.hpp>

#include <SDL.h>

namespace de {

	// Cette classe est déclarée ici por éviter une boucle infinie d'inclusion
	class Window;

	class DE_API Renderer {

		private:
			SDL_Renderer *_renderer;
			colora _color;

		public:
			Renderer();
			
			/// @brief			Crée un contexte de rendu pour pouvoir dessiner dessus.
			/// @param dest		Le contexte de rendu à créer.
			/// @param window	La fenêtre sur laquelle dessiner.
			/// @return			Le code d'erreur.
			static ErrorStatus create(Renderer &dest, Window &window);
			
			/// @brief			Défini la couleur utilisée par les opérations de dessins.
			/// @param color	
			void setColor(const colora &color);
			
			/// @brief Nettoie toute la surface de dessin en la remplissant de la couleur définie.
			void clear() const;
			
			/// @brief		Dessine un pixel aux coordonnées indiquées avec la couleur définie.
			/// @param pos	Position du pixel.
			void drawPixel(const vec2 &pos) const;

			/// @brief		Dessine un pixel aux coordonnées indiquées avec la couleur définie.
			/// @param pos	Position du pixel.
			void drawPixel(const fvec2 &pos) const;
			
			/// @brief		Dessine une ligne qui part des coordonnées x1, y1 et qui va jusqu'aux coordonnées x2, y2 avec la couleur définie.
			/// @param x1	Position X de départ de la ligne.
			/// @param y1	Position Y de départ de la ligne.
			/// @param x2	Position X de fin de la ligne.
			/// @param y2	Position Y de fin de la ligne.
			/// @return \c true si la fonction a réussie.
			bool drawLine(int x1, int y1, int x2, int y2) const;
			
			/// @brief			Dessine un rectangle avec la couleur définie. Peut être rempli ou non.
			/// @param rect		Position et taille du rectangle.
			/// @param fill		Détermine si le rectangle doit être rempli.
			// void drawRectangle(const rect &rect, bool fill = false) const;
			
			void drawShape(const SDL_Vertex *vertices, int verticesNumber) const;
			
			/// @brief Intervertie les buffers sur la surface de dessins.
			void swapBuffers() const;
			
			/// @brief Détruit le renderer.
			void destroy();

			//========== Getters ==========//

			SDL_Renderer *getRenderer() const;
			colora getColor() const;

	};

	/*
	=====================
	Renderer::getRenderer
	=====================
	*/
	inline SDL_Renderer *Renderer::getRenderer() const
	{
		return _renderer;
	}

	/*
	==================
	Renderer::getColor
	==================
	*/
	inline colora Renderer::getColor() const
	{
		return _color;
	}

	class DE_API OpenGLRenderer {

		public:
			OpenGLRenderer();

			static bool create(OpenGLRenderer &dest, Window &window);

			void clear() const;
			void draw(unsigned int numberOfVertices) const;
			void swapBuffers() const;

			SDL_GLContext context();
			SDL_Window *window();

			colora clearColor() const;

			void setClearColor(const colora color);

		private:
			SDL_GLContext m_Context;
			SDL_Window *m_Window;

			colora m_ClearColor;

	};

	inline SDL_GLContext OpenGLRenderer::context()
	{
		return m_Context;
	}

	inline SDL_Window *OpenGLRenderer::window()
	{
		return m_Window;
	}

	inline colora OpenGLRenderer::clearColor() const
	{
		return m_ClearColor;
	}

}

#endif