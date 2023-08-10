#ifndef __DEEP_ENGINE_GRAPHIC_HPP__
#define __DEEP_ENGINE_GRAPHIC_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/scene.hpp>
#include <DE/color.hpp>
#include <DE/vec.hpp>
#include <DE/graphic/shape.hpp>

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
			static Entity createRectangle(entity_collection_id collectionID, const fvec2 &position, float width, float height, const colora &color, bool collidable = false);

			/// @brief          Crée un triangle dans la scène spécifiée.
			/// @param scene    ID de la scène dans laquelle créer le triangle.
			/// @param position Position à laquelle se situera le triangle.
			/// @param radius   Taille du triangle.
			/// @param color    Couleur du triangle.
			/// @return         L'ID de l'entité nouvellement créée ou \ref badID si une erreur est survenue.
			static Entity createTriangle(entity_collection_id collectionID, const fvec2 &position, float radius, const colora &color);

	};

}

#endif