#ifndef DEEP_ENGINE_IMGUI_DEBUG_PANEL_HPP
#define DEEP_ENGINE_IMGUI_DEBUG_PANEL_HPP

#include "DeepEngine/deep_engine_export.h"
#include "DeepEngine/GUI/imgui_drawable.hpp"

#include "D3D/texture.hpp"

namespace deep
{
    class DEEP_ENGINE_API imgui_debug_panel : public imgui_drawable
    {
      public:
        enum class view
        {
            Main,
            Stats,
            About
        };

      public:
        imgui_debug_panel()                                     = delete;
        imgui_debug_panel(const imgui_debug_panel &)            = delete;
        imgui_debug_panel &operator=(const imgui_debug_panel &) = delete;

        virtual void draw() override;

        void set_icon(const ref<D3D::texture> &icon) noexcept;

      private:
        view m_view;
        ref<D3D::texture> m_icon;

      protected:
        imgui_debug_panel(const ref<ctx> &context, bool enabled) noexcept;

      public:
        friend memory_manager;
    };
} // namespace deep

#endif
