#ifndef __DEEP_ENGINE_IMGUI_HPP__
#define __DEEP_ENGINE_IMGUI_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/memory/list.hpp>

namespace de {

	using imgui_element_type = uint32_t;

	using imgui_element_id    = de_id;

	using imgui_window_id  = de_id;
	using imgui_element_id = de_id;

	constexpr imgui_element_type ImGuiTextType = (1 << 0);

	class DE_API ImGuiWindow {

		public:
			static imgui_window_id create(const char *title);
			static void destroy(imgui_window_id window);

			static bool isVisible(imgui_window_id window);
			static bool setVisible(imgui_window_id window, bool value);

			static bool addText(imgui_window_id window, const char *text);

			static void render();

			static void shutdown();

	};

}

#endif