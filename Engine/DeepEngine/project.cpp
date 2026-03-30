#include "DeepEngine/project.hpp"
#include "DeepEngine/engine.hpp"

#include <DeepLib/memory/memory.hpp>

#include <vector>

namespace deep
{
    static const char *WORLD            = "world";
    static const char *UI               = "UI";
    static const char *BACKGROUND_COLOR = "background_color";
    static const char *BORDER_COLOR     = "border_color";
    static const char *TEXT_COLOR       = "text_color";
    static const char *R                = "R";
    static const char *G                = "G";
    static const char *B                = "B";
    static const char *A                = "A";
    static const char *BORDER_SIZE      = "border_size";

    project::project(const ref<ctx> &context) noexcept
            : object(context)
    {
    }

    ref<project> project::create(const ref<ctx> &context, string_native &folder_path, const char *name, ref<project> &current_proj) noexcept
    {
        if (current_proj.is_valid())
        {
            current_proj->m_settings_stream->close();
        }
        else
        {
            current_proj = ref<project>(context, mem::alloc_type<project>(context.get(), context));
        }

        if (!current_proj.is_valid())
        {
            return ref<project>();
        }

        current_proj->m_folder_path = move(folder_path);

        string_native project_settings_path = current_proj->m_folder_path;
        project_settings_path.append(DEEP_NATIVE_SEPARATOR DEEP_TEXT_NATIVE("DeepEngineProject.json"));

        current_proj->m_settings_stream = ref<file_stream>(context, mem::alloc_type<file_stream>(context.get(),
                                                                                                 context,
                                                                                                 project_settings_path,
                                                                                                 core_fs::file_mode::CreateNew,
                                                                                                 core_fs::file_access::ReadWrite,
                                                                                                 core_fs::file_share::Read));

        if (!current_proj->m_settings_stream.is_valid())
        {
            return ref<project>();
        }

        if (!current_proj->m_settings_stream->open())
        {
            return ref<project>();
        }

        if (name != nullptr)
        {
            current_proj->m_settings["name"] = name;
        }
        else
        {
            current_proj->m_settings["name"] = "New Project";
        }
        current_proj->m_settings["version"] = "0.0.1";

        current_proj->m_settings[WORLD][BACKGROUND_COLOR] = {
            { R, 0.0f },
            { G, 0.0f },
            { B, 0.0f }
        };

        current_proj->m_settings[UI][BORDER_COLOR] = {
            { R, imgui_manager::DefaultGlobalBorderColor.x * 255.0f },
            { G, imgui_manager::DefaultGlobalBorderColor.y * 255.0f },
            { B, imgui_manager::DefaultGlobalBorderColor.z * 255.0f },
            { A, imgui_manager::DefaultGlobalBorderColor.w * 255.0f }
        };

        current_proj->m_settings[UI][BACKGROUND_COLOR] = {
            { R, imgui_manager::DefaultGlobalBackgroundColor.x * 255.0f },
            { G, imgui_manager::DefaultGlobalBackgroundColor.y * 255.0f },
            { B, imgui_manager::DefaultGlobalBackgroundColor.z * 255.0f },
            { A, imgui_manager::DefaultGlobalBackgroundColor.w * 255.0f }
        };

        current_proj->m_settings[UI][TEXT_COLOR] = {
            { R, imgui_manager::DefaultGlobalTextColor.x * 255.0f },
            { G, imgui_manager::DefaultGlobalTextColor.y * 255.0f },
            { B, imgui_manager::DefaultGlobalTextColor.z * 255.0f },
            { A, imgui_manager::DefaultGlobalTextColor.w * 255.0f }
        };

        usize bytes_written;
        std::string settings_dump = current_proj->m_settings.dump(4);

        current_proj->m_settings_stream->write(settings_dump.c_str(), settings_dump.size(), &bytes_written);

        return current_proj;
    }

    ref<project> project::open(const ref<ctx> &context, string_native &folder_path, ref<project> &current_proj) noexcept
    {
        if (current_proj.is_valid())
        {
            current_proj->m_settings_stream->close();
        }
        else
        {
            current_proj = ref<project>(context, mem::alloc_type<project>(context.get(), context));
        }

        if (!current_proj.is_valid())
        {
            return ref<project>();
        }

        current_proj->m_folder_path = move(folder_path);

        string_native project_settings_path = current_proj->m_folder_path;
        project_settings_path.append(DEEP_NATIVE_SEPARATOR DEEP_TEXT_NATIVE("DeepEngineProject.json"));

        current_proj->m_settings_stream = ref<file_stream>(context, mem::alloc_type<file_stream>(context.get(),
                                                                                                 context,
                                                                                                 project_settings_path,
                                                                                                 core_fs::file_mode::Open,
                                                                                                 core_fs::file_access::ReadWrite,
                                                                                                 core_fs::file_share::Read));

        if (!current_proj->m_settings_stream.is_valid())
        {
            return ref<project>();
        }

        if (!current_proj->m_settings_stream->open())
        {
            return ref<project>();
        }

        usize size = current_proj->m_settings_stream->get_length();
        usize bytes_read;

        std::vector<char> settings_dump;
        settings_dump.resize(size);

        current_proj->m_settings_stream->read(&settings_dump[0], size, &bytes_read);

        current_proj->m_settings = json::parse(settings_dump);

        return current_proj;
    }

    void project::load_settings(ref<engine> &eng) noexcept
    {
        if (!eng.is_valid())
        {
            return;
        }

        ref<D3D::graphics> graph = eng->get_graphics();
        if (!graph.is_valid())
        {
            return;
        }

        ref<imgui_manager> im_manager = eng->get_imgui_manager();
        if (!im_manager.is_valid())
        {
            return;
        }

        float world_r = 0.0f;
        float world_g = 0.0f;
        float world_b = 0.0f;

        if (m_settings.contains(WORLD))
        {
            json &world = m_settings[WORLD];

            if (world.contains(BACKGROUND_COLOR))
            {
                json &world_background_color = world[BACKGROUND_COLOR];

                if (world_background_color.contains(R))
                {
                    world_r = world_background_color[R];
                }

                if (world_background_color.contains(G))
                {
                    world_g = world_background_color[G];
                }

                if (world_background_color.contains(B))
                {
                    world_b = world_background_color[B];
                }
            }
        }

        float ui_border_color_R = imgui_manager::DefaultGlobalBorderColor.x;
        float ui_border_color_G = imgui_manager::DefaultGlobalBorderColor.y;
        float ui_border_color_B = imgui_manager::DefaultGlobalBorderColor.z;
        float ui_border_color_A = imgui_manager::DefaultGlobalBorderColor.w;

        float ui_background_color_R = imgui_manager::DefaultGlobalBackgroundColor.x;
        float ui_background_color_G = imgui_manager::DefaultGlobalBackgroundColor.y;
        float ui_background_color_B = imgui_manager::DefaultGlobalBackgroundColor.z;
        float ui_background_color_A = imgui_manager::DefaultGlobalBackgroundColor.w;

        float ui_text_color_R = imgui_manager::DefaultGlobalTextColor.x;
        float ui_text_color_G = imgui_manager::DefaultGlobalTextColor.y;
        float ui_text_color_B = imgui_manager::DefaultGlobalTextColor.z;
        float ui_text_color_A = imgui_manager::DefaultGlobalTextColor.w;

        float ui_border_size = imgui_manager::DefaultBorderSize;

        if (m_settings.contains(UI))
        {
            json &ui = m_settings[UI];

            if (ui.contains(BORDER_COLOR))
            {
                json &ui_border_color = ui[BORDER_COLOR];

                if (ui_border_color.contains(R))
                {
                    ui_border_color_R = ui_border_color[R];
                }

                if (ui_border_color.contains(G))
                {
                    ui_border_color_G = ui_border_color[G];
                }

                if (ui_border_color.contains(B))
                {
                    ui_border_color_B = ui_border_color[B];
                }

                if (ui_border_color.contains(A))
                {
                    ui_border_color_A = ui_border_color[A];
                }
            }

            if (ui.contains(BACKGROUND_COLOR))
            {
                json &ui_background_color = ui[BACKGROUND_COLOR];

                if (ui_background_color.contains(R))
                {
                    ui_background_color_R = ui_background_color[R];
                }

                if (ui_background_color.contains(G))
                {
                    ui_background_color_G = ui_background_color[G];
                }

                if (ui_background_color.contains(B))
                {
                    ui_background_color_B = ui_background_color[B];
                }

                if (ui_background_color.contains(A))
                {
                    ui_background_color_A = ui_background_color[A];
                }
            }

            if (ui.contains(TEXT_COLOR))
            {
                json &ui_text_color = ui[TEXT_COLOR];

                if (ui_text_color.contains(R))
                {
                    ui_text_color_R = ui_text_color[R];
                }

                if (ui_text_color.contains(G))
                {
                    ui_text_color_G = ui_text_color[G];
                }

                if (ui_text_color.contains(B))
                {
                    ui_text_color_B = ui_text_color[B];
                }

                if (ui_text_color.contains(A))
                {
                    ui_text_color_A = ui_text_color[A];
                }
            }

            if (ui.contains(BORDER_SIZE))
            {
                ui_border_size = ui[BORDER_SIZE];
            }
        }

        graph->set_background_color(fvec4(
                world_r / 255.0f,
                world_g / 255.0f,
                world_b / 255.0f,
                1.0f));

        im_manager->set_global_border_color(ImVec4(
                ui_border_color_R / 255.0f,
                ui_border_color_G / 255.0f,
                ui_border_color_B / 255.0f,
                ui_border_color_A / 255.0f));

        im_manager->set_global_background_color(ImVec4(
                ui_background_color_R / 255.0f,
                ui_background_color_G / 255.0f,
                ui_background_color_B / 255.0f,
                ui_background_color_A / 255.0f));

        im_manager->set_global_text_color(ImVec4(
                ui_text_color_R / 255.0f,
                ui_text_color_G / 255.0f,
                ui_text_color_B / 255.0f,
                ui_text_color_A / 255.0f));

        im_manager->set_global_border_size(ui_border_size);
    }

    bool project::save_settings(const ref<engine> &eng) noexcept
    {
        if (!eng.is_valid())
        {
            return false;
        }

        ref<D3D::graphics> graph = eng->get_graphics();
        if (!graph.is_valid())
        {
            return false;
        }

        ref<imgui_manager> im_manager = eng->get_imgui_manager();
        if (!im_manager.is_valid())
        {
            return false;
        }

        if (!m_settings_stream.is_valid())
        {
            return false;
        }

        fvec4 world_background_color = graph->get_background_color();

        ImVec4 ui_border_color     = im_manager->get_global_border_color();
        ImVec4 ui_background_color = im_manager->get_global_background_color();
        ImVec4 ui_text_color       = im_manager->get_global_text_color();
        float ui_border_size       = im_manager->get_global_border_size();

        m_settings[WORLD][BACKGROUND_COLOR] = {
            { R, world_background_color.x * 255.0f },
            { G, world_background_color.y * 255.0f },
            { B, world_background_color.z * 255.0f }
        };

        m_settings[UI][BORDER_COLOR] = {
            { R, ui_border_color.x * 255.0f },
            { G, ui_border_color.y * 255.0f },
            { B, ui_border_color.z * 255.0f },
            { A, ui_border_color.w * 255.0f }
        };

        m_settings[UI][BACKGROUND_COLOR] = {
            { R, ui_background_color.x * 255.0f },
            { G, ui_background_color.y * 255.0f },
            { B, ui_background_color.z * 255.0f },
            { A, ui_background_color.w * 255.0f }
        };

        m_settings[UI][TEXT_COLOR] = {
            { R, ui_text_color.x * 255.0f },
            { G, ui_text_color.y * 255.0f },
            { B, ui_text_color.z * 255.0f },
            { A, ui_text_color.w * 255.0f }
        };

        m_settings[UI][BORDER_SIZE] = ui_border_size;

        m_settings_stream->set_length(0);
        m_settings_stream->seek(0, stream::seek_origin::Begin);

        usize bytes_written;
        std::string settings_dump = m_settings.dump(4);

        return m_settings_stream->write(settings_dump.c_str(), settings_dump.size(), &bytes_written);
    }

} // namespace deep
