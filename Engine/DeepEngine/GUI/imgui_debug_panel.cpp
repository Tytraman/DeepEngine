#include "DeepEngine/GUI/imgui_debug_panel.hpp"
#include "DeepEngine/engine.hpp"

#include <DeepLib/context.hpp>
#include <imgui.h>

namespace deep
{
    imgui_debug_panel::imgui_debug_panel(const ref<ctx> &context, bool enabled) noexcept
            : imgui_drawable(context, enabled),
              m_view(view::Main)
    {
    }

    void deep::imgui_debug_panel::draw()
    {
        ImGui::SetNextWindowPos({ 0, 0 });
        ImGui::SetNextWindowSize({ 500, 700 });

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(46.0f / 255.0f, 57.0f / 255.0f, 68.0f / 255.0f, 1.0f));
        if (ImGui::Begin("DeepEngine Debug Panel", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("View"))
                {
                    if (ImGui::MenuItem("Main"))
                    {
                        m_view = view::Main;
                    }

                    if (ImGui::MenuItem("Stats"))
                    {
                        m_view = view::Stats;
                    }

                    if (ImGui::MenuItem("About"))
                    {
                        m_view = view::About;
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            // Affiche un panel différent selon la vue sélectionnée.
            switch (m_view)
            {
                default:
                    break;
                case view::Main:
                {
                    const char *title = "Welcome to DeepEngine!";
                    float icon_size   = 48.0f;

                    float panel_width = ImGui::GetContentRegionAvail().x;
                    ImVec2 title_size = ImGui::CalcTextSize(title);

                    float total_width = icon_size * 2.0f + title_size.x;

                    ImGui::SetCursorPosX((panel_width - total_width) / 2.0f);

                    if (m_icon.is_valid())
                    {
                        ImGui::Image((ImTextureID) m_icon->get(), ImVec2(icon_size, icon_size));
                        ImGui::SameLine();

                        float vertical_offset = (icon_size - title_size.y) / 2.0f;
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + vertical_offset);
                    }

                    ImGui::Text(title);

                    if (m_icon.is_valid())
                    {
                        ImGui::SameLine();
                        ImGui::Image((ImTextureID) m_icon->get(), ImVec2(icon_size, icon_size));
                    }

                    ImGui::Spacing();

                    if (ImGui::CollapsingHeader("How to use", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        ImGui::SeparatorText("Hotkeys :");
                        ImGui::BulletText("Escape: close program.");
                        ImGui::BulletText("F1: toggle cursor visibility.");
                        ImGui::BulletText("H: toggle GUI visibility.");
                    }
                }
                break;
                case view::Stats:
                {
                    ref<engine> eng = m_context->get_object<engine>(DEEP_TEXT_UTF8("engine"));

                    if (eng.is_valid())
                    {
                        uint32 FPS = eng->get_FPS();

                        ImGui::Text("FPS: %u", FPS);
                    }
                }
                break;
                case view::About:
                {
                    const char *text1 = "DeepEngine by Tytraman.";

                    ImGui::Text(text1);
                    ImGui::Spacing();

                    if (ImGui::CollapsingHeader("Libraries", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        ImGui::BulletText("DeepLib");
                        ImGui::SeparatorText("External:");
                        ImGui::BulletText("Direct3D 11");
                        ImGui::BulletText("libpng");
                    }
                }
                break;
            }

            ImGui::End();
        }
        ImGui::PopStyleColor();
    }

    void imgui_debug_panel::set_icon(const ref<D3D::texture> &icon) noexcept
    {
        m_icon = icon;
    }

} // namespace deep
