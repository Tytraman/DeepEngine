#ifndef __DEEP_ENGINE_OPENGL_CORE_HPP__
#define __DEEP_ENGINE_OPENGL_CORE_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "drivers/opengl/def.hpp"

namespace deep
{

    namespace GL3
    {

        class core
        {

            public:
                enum class gl_depth_function
                {
                    Always   = GL_ALWAYS,
                    Never    = GL_NEVER,
                    Less     = GL_LESS,
                    Equal    = GL_EQUAL,
                    Lequal   = GL_LEQUAL,
                    Greater  = GL_GREATER,
                    NotEqual = GL_NOTEQUAL,
                    Gequal   = GL_GEQUAL
                };

                enum class gl_cull_face
                {
                    Back         = GL_BACK,
                    Front        = GL_FRONT,
                    FrontAndBack = GL_FRONT_AND_BACK
                };

            public:
                /// @brief  R�cup�re le nom de la version OpenGL actuellement utilis�e.
                /// @return Une cha�ne de caract�res constante du nom de la version OpenGL utilis�e.
                DE_API static const char *query_version();

                /// @brief  R�cup�re le nombre d'attributs de vertex maximal g�r�e par la carte graphique.
                /// @return Le nombre d'attributs de vertex maximal g�r�e par la carte graphique.
                DE_API static int query_max_vertex_attribs();

                DE_API static int query_max_texture_image_units();

                DE_API static int query_max_texture_size();
                DE_API static int query_max_3D_texture_size();
                DE_API static int query_max_array_texture_layers();

                /// @brief        Met � jour le viewport d'OpenGL.
                /// @param width  Largeur du viewport.
                /// @param height Hauteur du viewport.
                DE_API static void update_viewport(int width, int height);

                DE_API static void enable_depth_mask();
                DE_API static void disable_depth_mask();

                DE_API static void enable_depth_test();
                DE_API static void disable_depth_test();

                DE_API static void set_depth_function(gl_depth_function func);

                DE_API static void set_cull_face(gl_cull_face cullFace);

        };

    }

}

#endif