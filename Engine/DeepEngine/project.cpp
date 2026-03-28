#include "DeepEngine/project.hpp"
#include "DeepEngine/engine.hpp"

#include <DeepLib/memory/memory.hpp>

#include <vector>

namespace deep
{
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

        current_proj->m_settings["world"]["background_color"] = {
            { "R", 0.0f },
            { "G", 0.0f },
            { "B", 0.0f }
        };

        constexpr const char *UI = "UI";

        current_proj->m_settings[UI]["border_color"] = {
            { "R", imgui_manager::DefaultGlobalBorderColor.x * 255.0f },
            { "G", imgui_manager::DefaultGlobalBorderColor.y * 255.0f },
            { "B", imgui_manager::DefaultGlobalBorderColor.z * 255.0f },
            { "A", imgui_manager::DefaultGlobalBorderColor.w * 255.0f }
        };

        current_proj->m_settings[UI]["background_color"] = {
            { "R", imgui_manager::DefaultGlobalBackgroundColor.x * 255.0f },
            { "G", imgui_manager::DefaultGlobalBackgroundColor.y * 255.0f },
            { "B", imgui_manager::DefaultGlobalBackgroundColor.z * 255.0f },
            { "A", imgui_manager::DefaultGlobalBackgroundColor.w * 255.0f }
        };

        current_proj->m_settings[UI]["text_color"] = {
            { "R", imgui_manager::DefaultGlobalTextColor.x * 255.0f },
            { "G", imgui_manager::DefaultGlobalTextColor.y * 255.0f },
            { "B", imgui_manager::DefaultGlobalTextColor.z * 255.0f },
            { "A", imgui_manager::DefaultGlobalTextColor.w * 255.0f }
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

        constexpr const char *WORLD            = "world";
        constexpr const char *UI               = "UI";
        constexpr const char *BACKGROUND_COLOR = "background_color";
        constexpr const char *BORDER_COLOR     = "border_color";
        constexpr const char *TEXT_COLOR       = "text_color";

        float world_r = m_settings[WORLD][BACKGROUND_COLOR]["R"];
        float world_g = m_settings[WORLD][BACKGROUND_COLOR]["G"];
        float world_b = m_settings[WORLD][BACKGROUND_COLOR]["B"];

        float ui_border_color_R = m_settings[UI][BORDER_COLOR]["R"];
        float ui_border_color_G = m_settings[UI][BORDER_COLOR]["G"];
        float ui_border_color_B = m_settings[UI][BORDER_COLOR]["B"];
        float ui_border_color_A = m_settings[UI][BORDER_COLOR]["A"];

        float ui_background_color_R = m_settings[UI][BACKGROUND_COLOR]["R"];
        float ui_background_color_G = m_settings[UI][BACKGROUND_COLOR]["G"];
        float ui_background_color_B = m_settings[UI][BACKGROUND_COLOR]["B"];
        float ui_background_color_A = m_settings[UI][BACKGROUND_COLOR]["A"];

        float ui_text_color_R = m_settings[UI][TEXT_COLOR]["R"];
        float ui_text_color_G = m_settings[UI][TEXT_COLOR]["G"];
        float ui_text_color_B = m_settings[UI][TEXT_COLOR]["B"];
        float ui_text_color_A = m_settings[UI][TEXT_COLOR]["A"];

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

        constexpr const char *WORLD            = "world";
        constexpr const char *UI               = "UI";
        constexpr const char *BACKGROUND_COLOR = "background_color";
        constexpr const char *BORDER_COLOR     = "border_color";
        constexpr const char *TEXT_COLOR       = "text_color";

        fvec4 world_background_color = graph->get_background_color();

        ImVec4 ui_border_color     = im_manager->get_global_border_color();
        ImVec4 ui_background_color = im_manager->get_global_background_color();
        ImVec4 ui_text_color       = im_manager->get_global_text_color();

        m_settings[WORLD][BACKGROUND_COLOR] = {
            { "R", world_background_color.x * 255.0f },
            { "G", world_background_color.y * 255.0f },
            { "B", world_background_color.z * 255.0f }
        };

        m_settings[UI][BORDER_COLOR] = {
            { "R", ui_border_color.x * 255.0f },
            { "G", ui_border_color.y * 255.0f },
            { "B", ui_border_color.z * 255.0f },
            { "A", ui_border_color.w * 255.0f }
        };

        m_settings[UI][BACKGROUND_COLOR] = {
            { "R", ui_background_color.x * 255.0f },
            { "G", ui_background_color.y * 255.0f },
            { "B", ui_background_color.z * 255.0f },
            { "A", ui_background_color.w * 255.0f }
        };

        m_settings[UI][TEXT_COLOR] = {
            { "R", ui_text_color.x * 255.0f },
            { "G", ui_text_color.y * 255.0f },
            { "B", ui_text_color.z * 255.0f },
            { "A", ui_text_color.w * 255.0f }
        };

        m_settings_stream->set_length(0);
        m_settings_stream->seek(0, stream::seek_origin::Begin);

        usize bytes_written;
        std::string settings_dump = m_settings.dump(4);

        return m_settings_stream->write(settings_dump.c_str(), settings_dump.size(), &bytes_written);
    }

} // namespace deep
