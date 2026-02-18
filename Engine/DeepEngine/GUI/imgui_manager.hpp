#ifndef DEEP_ENGINE_IMGUI_MANAGER_HPP
#define DEEP_ENGINE_IMGUI_MANAGER_HPP

#include "DeepEngine/deep_engine_export.h"
#include "DeepEngine/GUI/imgui_drawable.hpp"
#include "DeepEngine/GUI/imgui_debug_panel.hpp"
#include "DeepEngine/GUI/imgui_chat.hpp"

#include <DeepLib/object.hpp>
#include <DeepLib/memory/ref_counted.hpp>
#include <DeepLib/collection/list.hpp>

namespace deep
{
    class DEEP_ENGINE_API imgui_manager : public object
    {
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

        ref<imgui_debug_panel> get_debug_panel() const noexcept;
        ref<imgui_chat> get_chat() const noexcept;

      protected:
        imgui_manager(const ref<ctx> &context, bool enabled) noexcept;

      private:
        bool m_enabled;
        list<ref<imgui_drawable>> m_drawables;
        ref<imgui_debug_panel> m_debug_panel;
        ref<imgui_chat> m_chat;

      public:
        friend memory_manager;
    };
} // namespace deep

#endif
