#include "DeepEngine/GUI/imgui_debug_panel.hpp"
#include "DeepEngine/engine.hpp"
#include "DeepEngine/GUI/imgui_helper.hpp"
#include "DeepEngine/project.hpp"
#include "D3D/drawable/drawable_factory.hpp"

#include <DeepLib/context.hpp>
#include <DeepLib/maths/math.hpp>
#include <DeepLib/filesystem/filesystem.hpp>

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

        ref<imgui_manager> manager = eng->get_imgui_manager();
        if (!manager.is_valid())
        {
            return;
        }

        ImGui::SetNextWindowPos({ 5.0f, 5.0f });
        ImGui::SetNextWindowSize({ 450.0f, 600.0f });

        if (ImGui::Begin("DeepEngine Debug Panel", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Create new project..."))
                    {
                        string_native project_folder = fs::select_folder(m_context, DEEP_TEXT_NATIVE("Select a project folder"));

                        if (project_folder.is_valid())
                        {
                            m_context->out() << "Creating new project in '" << *project_folder << "' folder...\r\n";

                            ref<project> proj = m_context->get_object<project>("project");

                            proj = project::create(m_context, project_folder, nullptr, proj);

                            if (proj.is_valid())
                            {
                                m_context->set_object<project>("project", proj);

                                m_context->out() << "New project created!\r\n";
                            }
                            else
                            {
                                m_context->err() << "[ERROR] Cannot create project.\r\n";
                            }
                        }
                    }

                    if (ImGui::MenuItem("Open project..."))
                    {
                        string_native project_folder = fs::select_folder(m_context, DEEP_TEXT_NATIVE("Select a project folder"));

                        if (project_folder.is_valid())
                        {
                            m_context->out() << "Opening project in '" << *project_folder << "' folder...\r\n";

                            ref<project> proj = m_context->get_object<project>("project");

                            proj = project::open(m_context, project_folder, proj);

                            if (proj.is_valid())
                            {
                                m_context->set_object<project>("project", proj);

                                m_context->out() << "Project opened!\r\n";

                                proj->load_settings(eng);
                            }
                            else
                            {
                                m_context->err() << "[ERROR] Cannot open project.\r\n";
                            }
                        }
                    }

                    ImGui::Separator();

                    if (ImGui::MenuItem("Exit"))
                    {
                        eng->set_should_close(true);
                    }

                    ImGui::EndMenu();
                }

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

                if (ImGui::BeginMenu("Add"))
                {
                    if (ImGui::BeginMenu("Basic shapes"))
                    {
                        if (ImGui::MenuItem("Cube"))
                        {
                            ref<camera> cam = eng->get_camera();
                            if (cam.is_valid())
                            {
                                fvec3 location            = cam->get_location();
                                ref<D3D::cube> basic_cube = eng->get_basic_shapes().cube;

                                ref<D3D::cube> add_cube = D3D::drawable_factory::from(m_context,
                                                                                      basic_cube,
                                                                                      basic_cube->get_vertex_shader(),
                                                                                      basic_cube->get_pixel_shader(),
                                                                                      location,
                                                                                      fvec3(),
                                                                                      fvec3(1.0f, 1.0f, 1.0f),
                                                                                      graph->get_device());

                                if (add_cube.is_valid())
                                {
                                    graph->add_drawable(ref_cast<D3D::drawable>(add_cube));
                                }
                                else
                                {
                                    m_context->err() << "[ERROR] Cannot add 'basic_cube'\r\n";
                                }
                            }
                        }

                        if (ImGui::MenuItem("Plane"))
                        {
                            ref<camera> cam = eng->get_camera();
                            if (cam.is_valid())
                            {
                                fvec3 location              = cam->get_location();
                                ref<D3D::plane> basic_plane = eng->get_basic_shapes().plane;

                                ref<D3D::plane> add_plane = D3D::drawable_factory::from(m_context,
                                                                                        basic_plane,
                                                                                        basic_plane->get_vertex_shader(),
                                                                                        basic_plane->get_pixel_shader(),
                                                                                        location,
                                                                                        fvec3(),
                                                                                        fvec3(1.0f, 1.0f, 1.0f),
                                                                                        graph->get_device());

                                if (add_plane.is_valid())
                                {
                                    graph->add_drawable(ref_cast<D3D::drawable>(add_plane));
                                }
                                else
                                {
                                    m_context->err() << "[ERROR] Cannot add 'basic_plane'\r\n";
                                }
                            }
                        }

                        ImGui::EndMenu();
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("DirectX 11"))
                {
                    if (ImGui::MenuItem("Framebuffer"))
                    {
                        m_view = view::DirectX11Framebuffer;
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
                        imgui_helper::image(m_icon, icon_size, icon_size);
                        imgui_helper::same_line();

                        float vertical_offset = (icon_size - title_size.y) / 2.0f;
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + vertical_offset);
                    }

                    imgui_helper::print(title);

                    if (m_icon.is_valid())
                    {
                        imgui_helper::same_line();
                        imgui_helper::image(m_icon, icon_size, icon_size);
                    }

                    imgui_helper::spacing();

                    if (ImGui::CollapsingHeader("How to use", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        imgui_helper::print_separator("Global hotkeys :");
                        imgui_helper::print_bullet("Escape: exit program.");
                        imgui_helper::print_bullet("F1: toggle between GUI / Viewport mode.");

                        imgui_helper::spacing();

                        imgui_helper::print_separator("GUI hotkeys :");

                        imgui_helper::spacing();

                        imgui_helper::print_separator("Viewport hotkeys :");
                        imgui_helper::print_bullet("ZQSD: move.");
                        imgui_helper::print_bullet("Space: go up.");
                        imgui_helper::print_bullet("Control: go down.");
                        imgui_helper::print_bullet("H: toggle GUI visibility.");
                    }
                }
                break;
                case view::Stats:
                {
                    uint32 FPS = eng->get_FPS();

                    imgui_helper::print("FPS: %u", FPS);
                }
                break;
                case view::About:
                {
                    const char *text1 = "DeepEngine by Tytraman.";

                    imgui_helper::print(text1);
                    imgui_helper::spacing();

                    if (ImGui::CollapsingHeader("Libraries", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        imgui_helper::print_bullet("DeepLib");
                        imgui_helper::print_separator("External:");
                        imgui_helper::print_bullet("Direct3D 11");
                        imgui_helper::print_bullet("libpng");
                        imgui_helper::print_bullet("nlohmann_json");
                    }
                }
                break;
                case view::EditWorld:
                {
                    imgui_helper::print("World");
                    imgui_helper::spacing();

                    static fvec4 world_color = graph->get_background_color();

                    if (ImGui::BeginTable("WorldEdit", 2, ImGuiTableFlags_Borders))
                    {
                        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                        ImGui::TableSetupColumn("Widget", ImGuiTableColumnFlags_WidthStretch);

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        imgui_helper::print("Background color");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::ColorEdit4("##WorldBackgroundColor", &world_color.x))
                        {
                            graph->set_background_color(world_color);

                            ref<project> proj = m_context->get_object<project>("project");

                            if (proj.is_valid())
                            {
                                proj->save_settings(eng);
                            }
                        }

                        ImGui::EndTable();
                    }
                }
                break;
                case view::EditCamera:
                {
                    imgui_helper::print("Camera");
                    imgui_helper::spacing();

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
                        imgui_helper::print("Location");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::DragFloat3("##DeepEngineCameraLocation", &location.x, 0.1f))
                        {
                            cam->set_location(location);
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        imgui_helper::print("Yaw");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::DragFloat("##DeepEngineCameraYaw", &yaw, 0.1f))
                        {
                            cam->set_yaw(yaw);
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        imgui_helper::print("Pitch");

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
                        imgui_helper::print("Vertical FOV");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::DragFloat("##DeepEngineCameraVerticalFOV", &vertical_fov, 0.1f))
                        {
                            cam->set_lens(vertical_fov, aspect_ratio, z_near, z_far);
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        imgui_helper::print("Horizontal FOV");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);
                        imgui_helper::print("%.3f", horizontal_fov);

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        imgui_helper::print("Aspect ratio");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);
                        imgui_helper::print("%.3f", aspect_ratio);

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        imgui_helper::print("Z near");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::DragFloat("##DeepEngineCameraZNear", &z_near, 0.1f))
                        {
                            cam->set_lens(vertical_fov, aspect_ratio, z_near, z_far);
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        imgui_helper::print("Z far");

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
                    imgui_helper::print("ImGui");
                    imgui_helper::spacing();

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
                        imgui_helper::print("Background color");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::ColorEdit4("##ImGuiGlobalBackgroundColor", &background_color.x))
                        {
                            im->set_global_background_color(background_color);

                            ref<project> proj = m_context->get_object<project>("project");

                            if (proj.is_valid())
                            {
                                proj->save_settings(eng);
                            }
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        imgui_helper::print("Border color");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::ColorEdit4("##ImGuiGlobalBorderColor", &border_color.x))
                        {
                            im->set_global_border_color(border_color);

                            ref<project> proj = m_context->get_object<project>("project");

                            if (proj.is_valid())
                            {
                                proj->save_settings(eng);
                            }
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        imgui_helper::print("Text color");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::ColorEdit4("##ImGuiGlobalTextColor", &text_color.x))
                        {
                            im->set_global_text_color(text_color);

                            ref<project> proj = m_context->get_object<project>("project");

                            if (proj.is_valid())
                            {
                                proj->save_settings(eng);
                            }
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        imgui_helper::print("Border size");

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);

                        if (ImGui::DragFloat("##ImGuiGlobalBorderSize", &border_size, 0.1f))
                        {
                            im->set_global_border_size(border_size);

                            ref<project> proj = m_context->get_object<project>("project");

                            if (proj.is_valid())
                            {
                                proj->save_settings(eng);
                            }
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

                            ref<project> proj = m_context->get_object<project>("project");

                            if (proj.is_valid())
                            {
                                proj->save_settings(eng);
                            }
                        }

                        ImGui::EndTable();
                    }
                }
                break;
                case view::DirectX11Framebuffer:
                {
                    Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer_tex = graph->get_back_buffer_texture();

                    if (back_buffer_tex)
                    {
                        D3D11_TEXTURE2D_DESC desc;
                        back_buffer_tex->GetDesc(&desc);

                        imgui_helper::print("Resolution: %ux%u", desc.Width, desc.Height);
                        imgui_helper::print("MSAA: %u", desc.SampleDesc.Count);

                        imgui_helper::print_separator("Live Preview");
                        imgui_helper::print("Back buffer:");

                        float avaiable_width = ImGui::GetContentRegionAvail().x;
                        float aspect_ratio   = static_cast<float>(desc.Height) / static_cast<float>(desc.Width);
                        float final_height   = avaiable_width * aspect_ratio;

                        ImVec2 final_size = ImVec2(avaiable_width, final_height);

                        ImGui::Image((ImTextureID) graph->get_back_buffer_mirror_view().Get(),
                                     final_size,
                                     ImVec2(0.0f, 0.0f),
                                     ImVec2(1.0f, 1.0f),
                                     ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                                     manager->get_global_border_color());
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
