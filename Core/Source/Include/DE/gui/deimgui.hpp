#ifndef __DEEP_ENGINE_IMGUI_HPP__
#define __DEEP_ENGINE_IMGUI_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/list.hpp>

namespace deep
{

    class window;

    class im_gui_window
    {

        public:
            DE_API static void init(window &window);
            DE_API static void shutdown();

        private:
            static bool m_Initialized;

        public:
            im_gui_window() = delete;

    };

    using im_gui_debug_menu_view = uint32_t;

    constexpr im_gui_debug_menu_view ImGuiDebugMenuHomeView     = (1 << 0);
    constexpr im_gui_debug_menu_view ImGuiDebugMenuScenesView   = (1 << 1);
    constexpr im_gui_debug_menu_view ImGuiDebugMenuEntitiesView = (1 << 2);
    constexpr im_gui_debug_menu_view ImGuiDebugMenuCPUView      = (1 << 3);

    struct im_gui_debug_panel_options
    {
        im_gui_debug_menu_view view;

        DE_API im_gui_debug_panel_options(im_gui_debug_menu_view view = ImGuiDebugMenuHomeView);
    };

    class im_gui_debug_menu
    {

        public:
            DE_API static void render(const window *window);
            DE_API static void render_info(const window *window);
            DE_API static void add_window(const window *window);
            DE_API static void remove_window(const window *window);

        public:
            im_gui_debug_menu() = delete;

    };

}

#endif