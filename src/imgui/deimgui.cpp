#include <DE/imgui/deimgui.hpp>
#include <DE/memory/memory.hpp>
#include <DE/memory/list.hpp>
#include <DE/string_utils.hpp>
#include <DE/vec.hpp>

#include <unordered_map>
#include <string>

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

namespace de {

	struct ImGuiElement {
		imgui_element_type type;
		imgui_element_id elementID;
	};

	struct ImGuiText {
			char *text;

			ImGuiText(const char *text);
	};

	struct ImGuiWindowElement {
		List elementsOrder;
		std::unordered_map<imgui_element_id, ImGuiText> texts;
		imgui_element_id nextElementID;

		fvec2 pos;
		fvec2 size;

		std::string title;
		bool visible;

		ImGuiWindowElement(const char *title);
	};

	ImGuiText::ImGuiText(const char *_text)
		: text(StringUtils::copy(_text))
	{ }

	ImGuiWindowElement::ImGuiWindowElement(const char *_title)
		: elementsOrder(sizeof(ImGuiElement)),
		  nextElementID(0),
		  pos(fvec2(0.0f, 0.0f)),
		  size(fvec2(300.0f, 500.0f)),
		  title(_title),
		  visible(true)
	{ }

	static std::unordered_map<imgui_window_id, ImGuiWindowElement> m_Windows;
	static imgui_window_id m_NextWindowID = 0;

	static List m_WindowsOrder(sizeof(imgui_window_id));


	imgui_window_id ImGuiWindow::create(const char *title)
	{
		imgui_window_id windowID = m_NextWindowID;

		if(!m_WindowsOrder.addCopy(&windowID))
			return badID;

		m_Windows.emplace(windowID, ImGuiWindowElement(title));

		m_NextWindowID++;

		return windowID;
	}

	void ImGuiWindow::destroy(imgui_window_id window)
	{
		const auto &it = m_Windows.find(window);
		
		if(it == m_Windows.end())
			return;

		ImGuiWindowElement &win = it->second;

		win.elementsOrder.free();

		// Suppression de la mémoire allouée pour stocker le texte.
		for(auto &t : win.texts)
			mem::free(t.second.text);
	}

	bool ImGuiWindow::isVisible(imgui_window_id window)
	{
		const auto &it = m_Windows.find(window);
		
		if(it == m_Windows.end())
			return false;

		return it->second.visible;
	}

	bool ImGuiWindow::setVisible(imgui_window_id window, bool value)
	{
		const auto &it = m_Windows.find(window);
		
		if(it == m_Windows.end())
			return false;

		it->second.visible = value;

		return true;
	}

	bool ImGuiWindow::addText(imgui_window_id window, const char *text)
	{
		const auto &it = m_Windows.find(window);
		
		if(it == m_Windows.end())
			return false;

		ImGuiWindowElement &win = it->second;

		imgui_element_id elementID = win.nextElementID;

		ImGuiElement element = { ImGuiTextType, elementID };

		if(!win.elementsOrder.addCopy(&element))
			return false;

		win.texts.emplace(elementID, ImGuiText(text));

		win.nextElementID++;

		return true;
	}

	void ImGuiWindow::render()
	{
		imgui_window_id windowID;
		size_t i;
		size_t length = m_WindowsOrder.getNumberOfElements();

		for(i = 0; i < length; ++i) {
			if(!m_WindowsOrder.getCopy(i, &windowID))
				continue;

			const auto &it = m_Windows.find(windowID);
			if(it == m_Windows.end())
				continue;

			ImGuiWindowElement &window = it->second;

			if(window.visible) {
				ImGuiElement element;
				size_t numberOfElements = window.elementsOrder.getNumberOfElements();
				size_t j;

				ImGui::SetNextWindowPos({ window.pos.x, window.pos.y });
				ImGui::SetNextWindowSize({ window.size.x, window.size.y });
				ImGui::Begin(window.title.c_str(), nullptr, ImGuiWindowFlags_NoCollapse);

					for(j = 0; j < numberOfElements; ++j) {
						if(!window.elementsOrder.getCopy(j, &element))
							continue;

						switch(element.type) {
							default: break;
							case ImGuiTextType: {
								const auto &it = window.texts.find(element.elementID);
								if(it == window.texts.end())
									continue;

								ImGui::Text(it->second.text);
							} break;
						}
					}

				ImGui::End();
			}
		}
	}

	void ImGuiWindow::shutdown()
	{
		m_WindowsOrder.free();
	}

}
