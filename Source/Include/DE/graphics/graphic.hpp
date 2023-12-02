#ifndef __DEEP_ENGINE_GRAPHIC_HPP__
#define __DEEP_ENGINE_GRAPHIC_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/ecs/scene.hpp>
#include <DE/color.hpp>
#include <DE/vec.hpp>
#include <DE/graphics/shape.hpp>
#include <DE/rendering/opengl_utils.hpp>

namespace de
{

	class DE_API graphic
    {

		public:
			/// @brief                 Crée un rectangle dans la collection spécifiée.
			/// @param collectionID    ID de la collection dans laquelle créer le rectangle.
			/// @param position        Position à laquelle se situera le rectangle.
			/// @param width           Largeur du rectangle.
			/// @param height          Hauteur du rectangle.
			/// @param color           Couleur du rectangle.
			/// @return                L'entité nouvellement créée ou \ref entity::bad() si une erreur est survenue.
			static entity createRectangle(
                const char *name,
                program_id program,
                entity_collection_id collectionID,
                const fvec3 &position,
                float width,
                float height,
                const colora &color,
                bool collidable = false);

			static entity createRectangleTexture(
                const char *name,
                program_id program,
                entity_collection_id collectionID,
                const fvec3 &position,
                float width,
                float height,
                const colora &color,
                texture_id texture,
                uint8_t textureUnit,
                bool collidable = false);

			static entity create3DRectangle(
                const char *name,
                program_id program,
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
			
			static entity create3DRectangleTexture(
                const char *name,
                program_id program,
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
                texture_id texture,
				uint8_t textureUnit,
				bool collidable = false
			);

			static entity createCubemap(
                const char *name,
				program_id program,
				entity_collection_id collectionID,
				const fvec3 &position,
				float width,
				float height,
				float length,
				texture_id texture,
				uint8_t textureUnit,
				bool collidable = false
			);

        public:
            graphic() = delete;

	};

}

#endif