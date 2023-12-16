#ifndef __DEEP_ENGINE_RENDERER_HPP__
#define __DEEP_ENGINE_RENDERER_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/error.hpp>
#include <DE/color.hpp>
#include <DE/vec.hpp>

#include <SDL.h>

namespace deep
{

	// Cette classe est d�clar�e ici pour �viter une boucle infinie d'inclusion
	class window;

	class DE_API renderer
    {

		private:
			SDL_Renderer *m_Renderer;
			colora m_Color;

		public:
			renderer();
			
			/// @brief			Cr�e un contexte de rendu pour pouvoir dessiner dessus.
			/// @param dest		Le contexte de rendu � cr�er.
			/// @param window	La fen�tre sur laquelle dessiner.
			/// @return			Le code d'erreur.
			static error_status create(renderer &dest, window &window);
			
			/// @brief			D�fini la couleur utilis�e par les op�rations de dessins.
			/// @param color	
			void setColor(const colora &color);
			
			/// @brief Nettoie toute la surface de dessin en la remplissant de la couleur d�finie.
			void clear() const;
			
			/// @brief		Dessine un pixel aux coordonn�es indiqu�es avec la couleur d�finie.
			/// @param pos	Position du pixel.
			void drawPixel(const vec2 &pos) const;

			/// @brief		Dessine un pixel aux coordonn�es indiqu�es avec la couleur d�finie.
			/// @param pos	Position du pixel.
			void drawPixel(const fvec2 &pos) const;
			
			/// @brief		Dessine une ligne qui part des coordonn�es x1, y1 et qui va jusqu'aux coordonn�es x2, y2 avec la couleur d�finie.
			/// @param x1	Position X de d�part de la ligne.
			/// @param y1	Position Y de d�part de la ligne.
			/// @param x2	Position X de fin de la ligne.
			/// @param y2	Position Y de fin de la ligne.
			/// @return \c true si la fonction a r�ussie.
			bool drawLine(int x1, int y1, int x2, int y2) const;
			
			/// @brief			Dessine un rectangle avec la couleur d�finie. Peut �tre rempli ou non.
			/// @param rect		Position et taille du rectangle.
			/// @param fill		D�termine si le rectangle doit �tre rempli.
			// void drawRectangle(const rect &rect, bool fill = false) const;
			
			void drawShape(const SDL_Vertex *vertices, int verticesNumber) const;
			
			/// @brief Intervertie les buffers sur la surface de dessins.
			void swapBuffers() const;
			
			/// @brief D�truit le renderer.
			void destroy();

			//========== Getters ==========//

			SDL_Renderer *getRenderer() const;
			colora getColor() const;

	};

	/*
	=====================
	renderer::getRenderer
	=====================
	*/
	inline SDL_Renderer *renderer::getRenderer() const
	{
		return m_Renderer;
	}

	/*
	==================
	renderer::getColor
	==================
	*/
	inline colora renderer::getColor() const
	{
		return m_Color;
	}

}

#endif