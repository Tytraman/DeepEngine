#ifndef __DEEP_ENGINE_GRAPHIC_HPP__
#define __DEEP_ENGINE_GRAPHIC_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "ecs/scene.hpp"
#include "maths/vec.hpp"
#include "graphics/shape.hpp"

namespace deep
{

    class graphic
    {

        public:

            struct cube_texture_points
            {
                vec2<float> frontTopLeft;
                vec2<float> frontBottomRight;
                vec2<float> frontTopRight;
                vec2<float> frontBottomLeft;

                vec2<float> backTopLeft;
                vec2<float> backBottomRight;
                vec2<float> backTopRight;
                vec2<float> backBottomLeft;

                vec2<float> rightBottomLeft;
                vec2<float> rightBottomRight;
                vec2<float> rightTopRight;
                vec2<float> rightTopLeft;

                vec2<float> leftBottomRight;
                vec2<float> leftTopLeft;
                vec2<float> leftBottomLeft;
                vec2<float> leftTopRight;

                vec2<float> bottomBottomRight;
                vec2<float> bottomTopLeft;
                vec2<float> bottomTopRight;
                vec2<float> bottomBottomLeft;

                vec2<float> topTopLeft;
                vec2<float> topBottomRight;
                vec2<float> topTopRight;
                vec2<float> topBottomLeft;

                DE_API cube_texture_points();
            };
            
            DE_API static entity_manager::entity create_3D_rectangle_texture(
                const char *name,
                entity_collection_id collectionID,
                const vec3<float> &position,
                float width,
                float height,
                float length,
                const vec4<uint8_t> &color1,
                const vec4<uint8_t> &color2,
                const vec4<uint8_t> &color3,
                const vec4<uint8_t> &color4,
                const vec4<uint8_t> &color5,
                const vec4<uint8_t> &color6,
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