#ifndef __DEEP_ENGINE_IMGUI_HPP__
#define __DEEP_ENGINE_IMGUI_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/memory/list.hpp>

namespace de {

	class Window;

	class DE_API ImGuiWindow {

		public:
			static void init(Window &window);
			static void shutdown();

	};

	using ImGuiDebugMenuView = uint32_t;

	constexpr ImGuiDebugMenuView ImGuiDebugMenuHomeView     = (1 << 0);
	constexpr ImGuiDebugMenuView ImGuiDebugMenuScenesView   = (1 << 1);
	constexpr ImGuiDebugMenuView ImGuiDebugMenuEntitiesView = (1 << 2);

	struct DE_API ImGuiDebugPanelOptions {
		ImGuiDebugMenuView view;

		ImGuiDebugPanelOptions(ImGuiDebugMenuView view = ImGuiDebugMenuHomeView);
	};

	class DE_API ImGuiDebugMenu {

		public:
			static void render(const Window *window);
			static void addWindow(const Window *window);
			static void removeWindow(const Window *window);

	};

}

#endif