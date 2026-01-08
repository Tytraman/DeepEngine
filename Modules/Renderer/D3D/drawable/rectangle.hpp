#ifndef DEEP_ENGINE_D3D_SQUARE_HPP
#define DEEP_ENGINE_D3D_SQUARE_HPP

#include "deep_d3d_export.h"
#include "D3D/drawable/drawable.hpp"

namespace deep
{
    namespace D3D
    {
        class DEEP_D3D_API rectangle : public drawable
        {
          public:
            virtual void draw(device_context &dc) override;

          protected:
            using drawable::drawable;

          public:
            friend class drawable_factory;
        };
    } // namespace D3D
} // namespace deep

#endif
