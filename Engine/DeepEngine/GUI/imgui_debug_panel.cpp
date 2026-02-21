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
        ref<engine> eng = m_context->get_object<engine>(DEEP_TEXT_UTF8("engine"));
        if (!eng.is_valid())
        {
            return;
        }

        ref<D3D::graphics> graph = eng->get_graphics();
        if (!graph.is_valid())
        {
            return;
        }

        ImGui::SetNextWindowPos({ 5.0f, 5.0f });
        ImGui::SetNextWindowSize({ 450.0f, 600.0f });

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

                    ImGui::Separator();

                    if (ImGui::MenuItem("Exit"))
                    {
                        eng->set_should_close(true);
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::MenuItem("World"))
                    {
                        m_view = view::EditWorld;
                    }

                    if (ImGui::MenuItem("Camera"))
                    {
                        m_view = view::EditCamera;
                    }

                    if (ImGui::MenuItem("ImGui"))
                    {
                        m_view = view::EditImGui;
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
                        ImGui::SeparatorText("Global hotkeys :");
                        ImGui::BulletText("Escape: exit program.");
                        ImGui::BulletText("F1: toggle between GUI / Viewport mode.");

                        ImGui::Spacing();

                        ImGui::SeparatorText("GUI hotkeys :");

                        ImGui::Spacing();

                        ImGui::SeparatorText("Viewport hotkeys :");
                        ImGui::BulletText("H: toggle GUI visibility.");
                    }
                }
                break;
                case view::Stats:
                {
                    uint32 FPS = eng->get_FPS();

                    ImGui::Text("FPS: %u", FPS);
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
                case view::EditWorld:
                {
                    ImGui::Text("World");
                    ImGui::Spacing();

                    static fvec4 world_color = graph->get_background_color();

                    if (ImGui::BeginTable("WorldEdit", 2, ImGuiTableFlags_Borders))
                    {
                        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                        ImGui::TableSetupColumn("Widget", ImGuiTableColumnFlags_WidthStretch);

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Background color");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::ColorEdit4("##WorldBackgroundColor", &world_color.x))
                        {
                            graph->set_background_color(world_color);
                        }

                        ImGui::EndTable();
                    }
                }
                break;
                case view::EditCamera:
                {
                    ImGui::Text("Camera");
                    ImGui::Spacing();

                    ref<camera> cam = eng->get_camera();
                    if (!cam.is_valid())
                    {
                        break;
                    }

                    if (ImGui::BeginTable("CameraTransform", 2, ImGuiTableFlags_Borders))
                    {
                        static fvec3 location = cam->get_location();
                        static float yaw      = cam->get_yaw();
                        static float pitch    = cam->get_pitch();

                        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                        ImGui::TableSetupColumn("Widget", ImGuiTableColumnFlags_WidthStretch);

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Location");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::DragFloat3("##DeepEngineCameraLocation", &location.x, 0.1f))
                        {
                            cam->set_location(location);
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Yaw");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::DragFloat("##DeepEngineCameraYaw", &yaw, 0.1f))
                        {
                            cam->set_yaw(yaw);
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Pitch");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::DragFloat("##DeepEngineCameraPitch", &pitch, 0.1f))
                        {
                            cam->set_pitch(pitch);
                        }

                        ImGui::EndTable();
                    }

                    if (ImGui::BeginTable("CameraInfo", 2, ImGuiTableFlags_Borders))
                    {
                        float vertical_fov   = cam->get_vertical_fov();
                        float horizontal_fov = cam->get_horizontal_fov();
                        float aspect_ratio   = cam->get_aspect_ratio();
                        float z_near         = cam->get_z_near();
                        float z_far          = cam->get_z_far();

                        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                        ImGui::TableSetupColumn("Widget", ImGuiTableColumnFlags_WidthStretch);

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Vertical FOV");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::DragFloat("##DeepEngineCameraVerticalFOV", &vertical_fov, 0.1f))
                        {
                            cam->set_lens(vertical_fov, aspect_ratio, z_near, z_far);
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Horizontal FOV");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);
                        ImGui::Text("%.3f", horizontal_fov);

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Aspect ratio");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);
                        ImGui::Text("%.3f", aspect_ratio);

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Z near");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::DragFloat("##DeepEngineCameraZNear", &z_near, 0.1f))
                        {
                            cam->set_lens(vertical_fov, aspect_ratio, z_near, z_far);
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Z far");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::DragFloat("##DeepEngineCameraZFar", &z_far, 0.1f))
                        {
                            cam->set_lens(vertical_fov, aspect_ratio, z_near, z_far);
                        }

                        ImGui::EndTable();
                    }
                }
                break;
                case view::EditImGui:
                {
                    ImGui::Text("ImGui");
                    ImGui::Spacing();

                    ref<imgui_manager> im = eng->get_imgui_manager();
                    if (!im.is_valid())
                    {
                        break;
                    }

                    static ImVec4 background_color = im->get_global_background_color();
                    static ImVec4 border_color     = im->get_global_border_color();
                    static ImVec4 text_color       = im->get_global_text_color();
                    static float border_size       = im->get_global_border_size();

                    if (ImGui::BeginTable("ImGuiGlobalStyle", 2, ImGuiTableFlags_Borders))
                    {
                        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                        ImGui::TableSetupColumn("Widget", ImGuiTableColumnFlags_WidthStretch);

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Background color");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::ColorEdit4("##ImGuiGlobalBackgroundColor", &background_color.x))
                        {
                            im->set_global_background_color(background_color);
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Border color");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::ColorEdit4("##ImGuiGlobalBorderColor", &border_color.x))
                        {
                            im->set_global_border_color(border_color);
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Text color");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::ColorEdit4("##ImGuiGlobalTextColor", &text_color.x))
                        {
                            im->set_global_text_color(text_color);
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Border size");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::DragFloat("##ImGuiGlobalBorderSize", &border_size, 0.1f))
                        {
                            im->set_global_border_size(border_size);
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::TableNextColumn();

                        if (ImGui::Button("Reset to default", ImVec2(-1.0f, 0.0f)))
                        {
                            background_color = imgui_manager::DefaultGlobalBackgroundColor;
                            border_color     = imgui_manager::DefaultGlobalBorderColor;
                            text_color       = imgui_manager::DefaultGlobalTextColor;
                            border_size      = imgui_manager::DefaultBorderSize;

                            im->set_global_background_color(background_color);
                            im->set_global_border_color(border_color);
                            im->set_global_text_color(text_color);
                            im->set_global_border_size(border_size);
                        }

                        ImGui::EndTable();
                    }
                }
                break;
            }

            ImGui::End();
        }
    }

    void imgui_debug_panel::set_icon(const ref<D3D::texture> &icon) noexcept
    {
        m_icon = icon;
    }

} // namespace deep
