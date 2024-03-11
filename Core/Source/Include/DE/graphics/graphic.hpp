#ifndef __DEEP_ENGINE_GRAPHIC_HPP__
#define __DEEP_ENGINE_GRAPHIC_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/ecs/scene.hpp"
#include "DE/core/color.hpp"
#include "DE/maths/vec.hpp"
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
            
            DE_API static entity_manager::entity create_3D_rectangle_texture(
                const char *name,
                entity_collection_id collectionID,
                const vec3<float> &position,
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

            DE_API static polygon create_cube(
                const char *name,
                const vec4<float> &color1,
                const vec4<float> &color2,
                const vec4<float> &color3,
                const vec4<float> &color4,
                const vec4<float> &color5,
                const vec4<float> &color6,
                const cube_texture_points &texturePoints
            );

            DE_API static polygon create_cube(
                const char *name,
                const vec4<float> &color1,
                const vec4<float> &color2,
                const vec4<float> &color3,
                const vec4<float> &color4,
                const vec4<float> &color5,
                const vec4<float> &color6
            );

            DE_API static entity_manager::entity create_cubemap(
                const char *name,
                entity_collection_id collectionID,
                const vec3<float> &position,
                float width,
                float height,
                float length,
                bool collidable = false
            );

        public:
            graphic() = delete;

    };

}

#endif