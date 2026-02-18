#include "DeepEngine/GUI/imgui_chat.hpp"
#include "DeepEngine/engine.hpp"

#include <DeepLib/context.hpp>

namespace deep
{
    imgui_chat::imgui_chat(const ref<ctx> &context, bool enabled) noexcept
            : imgui_drawable(context, enabled),
              m_history(context)
    {
    }

    void deep::imgui_chat::draw()
    {
        static char input_buffer[256] = "";

        ref<engine> eng = m_context->get_object<engine>(DEEP_TEXT_UTF8("engine"));

        if (!eng.is_valid())
        {
            return;
        }

        ref<window> win = eng->get_window();

        if (!win.is_valid())
        {
            return;
        }

        float panel_height  = 300.0f;
        float footer_height = 35.0f;
        float height        = static_cast<float>(win->get_height());

        ImGui::SetNextWindowPos({ 0.0f, height - panel_height });
        ImGui::SetNextWindowSize({ 500.0f, panel_height });

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(46.0f / 255.0f, 57.0f / 255.0f, 68.0f / 255.0f, 1.0f));
        if (ImGui::Begin("Chat", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
        {
            float scrolling_region_height = ImGui::GetContentRegionAvail().y - footer_height;

            if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, scrolling_region_height), 0, ImGuiWindowFlags_HorizontalScrollbar))
            {
                usize index;
                usize count = m_history.count();

                for (index = 0; index < count; ++index)
                {
                    message &msg = m_history[index];

                    ImGui::PushStyleColor(ImGuiCol_Text, msg.color);
                    ImGui::TextWrapped(*msg.text);
                    ImGui::PopStyleColor();
                }

                ImGui::EndChild();
            }

            ImGui::Separator();

            ImGui::SetCursorPosY(ImGui::GetWindowHeight() - footer_height);

            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::InputText("##ChatInput", input_buffer, IM_ARRAYSIZE(input_buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                // Code exécuté lors de l'appui sur la touche 'Entrée'.

                if (input_buffer[0] != '\0')
                {
                    message msg = {
                        string(m_context, input_buffer),
                        ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
                    };

                    m_history.add(msg);
                }

                input_buffer[0] = '\0';          // On vide le buffer.
                ImGui::SetKeyboardFocusHere(-1); // On garde le focus sur l'input.
            }
            ImGui::PopItemWidth();

            ImGui::End();
        }
        ImGui::PopStyleColor();
    }
} // namespace deep
