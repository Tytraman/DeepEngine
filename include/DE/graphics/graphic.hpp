#ifndef __DEEP_ENGINE_GRAPHIC_HPP__
#define __DEEP_ENGINE_GRAPHIC_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/ecs/scene.hpp>
#include <DE/color.hpp>
#include <DE/vec.hpp>
#include <DE/graphics/shape.hpp>
#include <DE/rendering/opengl_utils.hpp>

namespace de {

	class DE_API Graphic {

		public:
			/// @brief                 Crée un rectangle dans la collection spécifiée.
			/// @param collectionID    ID de la collection dans laquelle créer le rectangle.
			/// @param position        Position à laquelle se situera le rectangle.
			/// @param width           Largeur du rectangle.
			/// @param height          Hauteur du rectangle.
			/// @param color           Couleur du rectangle.
			/// @return                L'entité nouvellement créée ou \ref Entity::bad() si une erreur est survenue.
			static Entity createRectangle(entity_collection_id collectionID, const fvec3 &position, float width, float height, const colora &color, bool collidable = false);

			static Entity createRectangleTexture(entity_collection_id collectionID, const fvec3 &position, float width, float height, const colora &color, gl_texture texture, uint8_t textureUnit, bool collidable = false);

			static Entity create3DRectangle(entity_collection_id collectionID, const fvec3 &position, float width, float height, float length, const colora &color1, const colora &color2, const colora &color3, const colora &color4, const colora &color5, const colora &color6, bool collidable = false);

			/// @brief          Crée un triangle dans la scène spécifiée.
			/// @param scene    ID de la scène dans laquelle créer le triangle.
			/// @param position Position à laquelle se situera le triangle.
			/// @param radius   Taille du triangle.
			/// @param color    Couleur du triangle.
			/// @return         L'ID de l'entité nouvellement créée ou \ref badID si une erreur est survenue.
			static Entity createTriangle(entity_collection_id collectionID, const fvec3 &position, float radius, const colora &color);

	};

}

#endif