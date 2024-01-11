#ifndef __DEEP_ENGINE_GRAPHIC_HPP__
#define __DEEP_ENGINE_GRAPHIC_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/ecs/scene.hpp"
#include "DE/color.hpp"
#include "DE/vec.hpp"
#include "DE/graphics/shape.hpp"

namespace deep
{

    class graphic
    {

        public:

            struct cube_texture_points
            {
                fvec2 frontTopLeft;
                fvec2 frontBottomRight;
                fvec2 frontTopRight;
                fvec2 frontBottomLeft;

                fvec2 backTopLeft;
                fvec2 backBottomRight;
                fvec2 backTopRight;
                fvec2 backBottomLeft;

                fvec2 rightBottomLeft;
                fvec2 rightBottomRight;
                fvec2 rightTopRight;
                fvec2 rightTopLeft;

                fvec2 leftBottomRight;
                fvec2 leftTopLeft;
                fvec2 leftBottomLeft;
                fvec2 leftTopRight;

                fvec2 bottomBottomRight;
                fvec2 bottomTopLeft;
                fvec2 bottomTopRight;
                fvec2 bottomBottomLeft;

                fvec2 topTopLeft;
                fvec2 topBottomRight;
                fvec2 topTopRight;
                fvec2 topBottomLeft;

                DE_API cube_texture_points();
            };


        public:
            /// @brief                 Crée un rectangle dans la collection spécifiée.
            /// @param collectionID    ID de la collection dans laquelle créer le rectangle.
            /// @param position        Position à laquelle se situera le rectangle.
            /// @param width           Largeur du rectangle.
            /// @param height          Hauteur du rectangle.
            /// @param color           Couleur du rectangle.
            /// @return                L'entité nouvellement créée ou \ref entity::bad() si une erreur est survenue.
            DE_API static entity_manager::entity createRectangle(
                const char *name,
                GL3::gl_id program,
                entity_collection_id collectionID,
                const fvec3 &position,
                float width,
                float height,
                const colora &color,
                bool collidable = false);

            DE_API static entity_manager::entity createRectangleTexture(
                const char *name,
                GL3::gl_id program,
                entity_collection_id collectionID,
                const fvec3 &position,
                float width,
                float height,
                const colora &color,
                GL3::gl_id texture,
                uint8_t textureUnit,
                bool collidable = false);

            DE_API static entity_manager::entity create3DRectangle(
                const char *name,
                GL3::gl_id program,
                entity_collection_id collectionID,
                const fvec3 &position,
                float width,
                float height,
                float length,
                const colora &color1,
                const colora &color2,
                const colora &color3,
                const colora &color4,
                const colora &color5,
                const colora &color6,
                bool collidable = false
            );
            
            DE_API static entity_manager::entity create_3D_rectangle_texture(
                const char *name,
                GL3::gl_id program,
                entity_collection_id collectionID,
                const fvec3 &position,
                float width,
                float height,
                float length,
                const colora &color1,
                const colora &color2,
                const colora &color3,
                const colora &color4,
                const colora &color5,
                const colora &color6,
                GL3::gl_id texture,
                uint8_t textureUnit,
                bool collidable = false
            );

            DE_API static polygon create_cube(
                const char *name,
                const colora &color1,
                const colora &color2,
                const colora &color3,
                const colora &color4,
                const colora &color5,
                const colora &color6,
                const cube_texture_points &texturePoints = cube_texture_points()
            );

            DE_API static polygon create_cube(const char *name);

            DE_API static entity_manager::entity create_cubemap(
                const char *name,
                GL3::gl_id program,
                entity_collection_id collectionID,
                const fvec3 &position,
                float width,
                float height,
                float length,
                GL3::gl_id texture,
                uint8_t textureUnit,
                bool collidable = false
            );

        public:
            graphic() = delete;

    };

}

#endif