#ifndef __DEEP_ENGINE_IMGUI_HPP__
#define __DEEP_ENGINE_IMGUI_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/list.hpp>

namespace de
{

	class window;

	class DE_API im_gui_window
    {

		public:
			static void init(window &window);
			static void shutdown();

		private:
			static bool m_Initialized;

        public:
            im_gui_window() = delete;

	};

	using im_gui_debug_menu_view = uint32_t;

	constexpr im_gui_debug_menu_view ImGuiDebugMenuHomeView     = (1 << 0);
	constexpr im_gui_debug_menu_view ImGuiDebugMenuScenesView   = (1 << 1);
	constexpr im_gui_debug_menu_view ImGuiDebugMenuEntitiesView = (1 << 2);

	struct DE_API im_gui_debug_panel_options
    {
		im_gui_debug_menu_view view;

		im_gui_debug_panel_options(im_gui_debug_menu_view view = ImGuiDebugMenuHomeView);
	};

	class DE_API im_gui_debug_menu
    {

		public:
			static void render(const window *window);
			static void addWindow(const window *window);
			static void removeWindow(const window *window);

        public:
            im_gui_debug_menu() = delete;

	};

}

#endif