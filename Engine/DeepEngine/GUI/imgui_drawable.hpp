#ifndef DEEP_ENGINE_IMGUI_DRAWABLE_HPP
#define DEEP_ENGINE_IMGUI_DRAWABLE_HPP

#include "DeepEngine/deep_engine_export.h"

#include <DeepLib/object.hpp>

namespace deep
{
    class DEEP_ENGINE_API imgui_drawable : public object
    {
      public:
        imgui_drawable()                                  = delete;
        imgui_drawable(const imgui_drawable &)            = delete;
        imgui_drawable &operator=(const imgui_drawable &) = delete;

        virtual void draw() = 0;

        bool is_enabled() const noexcept;
        void set_enabled(bool value) noexcept;

      protected:
        imgui_drawable(const ref<ctx> &context, bool enabled) noexcept;

      protected:
        bool m_enabled;
    };
} // namespace deep

#endif
