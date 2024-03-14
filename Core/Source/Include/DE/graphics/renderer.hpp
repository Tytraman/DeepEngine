#ifndef __DEEP_ENGINE_RENDERER_HPP__
#define __DEEP_ENGINE_RENDERER_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/core/color.hpp"
#include "DE/maths/vec.hpp"

#include <SDL.h>

namespace deep
{

    // Cette classe est déclarée ici pour éviter une boucle infinie d'inclusion
    class window;

    class DE_API renderer
    {

        private:
            SDL_Renderer *m_Renderer;

        public:
            renderer();
            
            /// @brief			Crée un contexte de rendu pour pouvoir dessiner dessus.
            /// @param dest		Le contexte de rendu à créer.
            /// @param window	La fenêtre sur laquelle dessiner.
            /// @return			Le code d'erreur.
            static bool create(renderer &dest, window &window);
            
            /// @brief Nettoie toute la surface de dessin en la remplissant de la couleur définie.
            void clear() const;
            
            /// @brief Intervertie les buffers sur la surface de dessins.
            void swapBuffers() const;
            
            /// @brief Détruit le renderer.
            void destroy();

            //========== Getters ==========//

            SDL_Renderer *getRenderer() const;

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

}

#endif