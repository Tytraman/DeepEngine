#ifndef __DEEP_ENGINE_OPENGL_ERROR_HPP__
#define __DEEP_ENGINE_OPENGL_ERROR_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"

#include <glad/glad.h>

namespace deep
{

    namespace GL3
    {

        class gl_error
        {

            public:
            
                /// @brief Nettoie toutes les erreurs qu'OpenGL stock en interne.
                DE_API static void clear_errors();

                /// @brief          V�rifie si OpenGL a d�tect� une ou plusieurs erreurs.
                /// @param function Nom de la fonction OpenGL qui a g�n�r� l'erreur.
                /// @param file     Nom du fichier dans lequel s'est produit l'erreur.
                /// @param line     Ligne � laquelle s'est produit l'erreur.
                /// @return         \c true si une ou plusieurs erreurs se sont produites.
                DE_API static bool check_errors(const char *function, const char *file, unsigned int line);

                /// @brief           Retourne une cha�ne de caract�res du nom de l'erreur OpenGL.
                /// @param errorCode code d'erreur OpenGL.
                /// @return          Une cha�ne de caract�res constante du nom de l'erreur.
                DE_API static const char *get_name(GLenum errorCode);

        };

    }

}

#endif