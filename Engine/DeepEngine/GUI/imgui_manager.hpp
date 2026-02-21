#ifndef DEEP_ENGINE_IMGUI_MANAGER_HPP
#define DEEP_ENGINE_IMGUI_MANAGER_HPP

#include "DeepEngine/deep_engine_export.h"
#include "DeepEngine/GUI/imgui_drawable.hpp"
#include "DeepEngine/GUI/imgui_debug_panel.hpp"
#include "DeepEngine/GUI/imgui_chat.hpp"

#include <DeepLib/object.hpp>
#include <DeepLib/memory/ref_counted.hpp>
#include <DeepLib/collection/list.hpp>

#include <imgui.h>

namespace deep
{
    class DEEP_ENGINE_API imgui_manager : public object
    {
      public:
        static constexpr ImVec4 DefaultGlobalBackgroundColor = ImVec4(0.05f, 0.05f, 0.05f, 0.6f);
        static constexpr ImVec4 DefaultGlobalBorderColor     = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        static constexpr ImVec4 DefaultGlobalTextColor       = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        static constexpr float DefaultBorderSize             = 1.0f;

      public:
        imgui_manager()                                 = delete;
        imgui_manager(const imgui_manager &)            = delete;
        imgui_manager &operator=(const imgui_manager &) = delete;

        static ref<imgui_manager> create(const ref<ctx> &context) noexcept;

        void init(window_handle win) noexcept;
        void shutdown() noexcept;

        void draw_all() noexcept;

        bool is_enabled() const noexcept;
        void set_enabled(bool value) noexcept;

        void lose_focus() noexcept;

        ImVec4 get_global_background_color() const noexcept;
        ImVec4 get_global_border_color() const noexcept;
        ImVec4 get_global_text_color() const noexcept;
        float get_global_border_size() const noexcept;

        void set_global_background_color(const ImVec4 &value) noexcept;
        void set_global_border_color(const ImVec4 &value) noexcept;
        void set_global_text_color(const ImVec4 &value) noexcept;
        void set_global_border_size(float border_size) noexcept;

        ref<imgui_debug_panel> get_debug_panel() const noexcept;
        ref<imgui_chat> get_chat() const noexcept;

      protected:
        imgui_manager(const ref<ctx> &context, bool enabled) noexcept;

      private:
        bool m_enabled;
        ImVec4 m_global_background_color;
        ImVec4 m_global_border_color;
        ImVec4 m_global_text_color;
        float m_global_border_size;
        list<ref<imgui_drawable>> m_drawables;
        ref<imgui_debug_panel> m_debug_panel;
        ref<imgui_chat> m_chat;

      public:
        friend memory_manager;
    };
} // namespace deep

#endif
