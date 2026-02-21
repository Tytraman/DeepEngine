#include "DeepEngine/GUI/imgui_manager.hpp"
#include "DeepEngine/GUI/imgui_debug_panel.hpp"

#include <DeepLib/memory/memory.hpp>

#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

namespace deep
{
    imgui_manager::imgui_manager(const ref<ctx> &context, bool enabled) noexcept
            : object::object(context),
              m_enabled(enabled),
              m_global_background_color(DefaultGlobalBackgroundColor),
              m_global_border_color(DefaultGlobalBorderColor),
              m_global_text_color(DefaultGlobalTextColor),
              m_global_border_size(DefaultBorderSize),
              m_drawables(context)
    {
    }

    ref<imgui_manager> imgui_manager::create(const ref<ctx> &context) noexcept
    {
        imgui_manager *im = mem::alloc_type<imgui_manager>(context.get(), context, true);

        if (im == nullptr)
        {
            return ref<imgui_manager>();
        }

        imgui_debug_panel *im_debug = mem::alloc_type<imgui_debug_panel>(context.get(), context, true);
        imgui_chat *im_chat         = mem::alloc_type<imgui_chat>(context.get(), context, true);

        im->m_debug_panel = ref<imgui_debug_panel>(context, im_debug);
        im->m_chat        = ref<imgui_chat>(context, im_chat);

        return ref<imgui_manager>(context, im);
    }

    void imgui_manager::init(window_handle win) noexcept
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplWin32_Init(win);
    }

    void imgui_manager::shutdown() noexcept
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();

        ImGui::DestroyContext();
    }

    void imgui_manager::draw_all() noexcept
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::PushStyleColor(ImGuiCol_WindowBg, m_global_background_color);
        ImGui::PushStyleColor(ImGuiCol_Border, m_global_border_color);
        ImGui::PushStyleColor(ImGuiCol_Text, m_global_text_color);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, m_global_border_size);

        if (m_debug_panel.is_valid())
        {
            m_debug_panel->draw();
        }

        if (m_chat.is_valid())
        {
            m_chat->draw();
        }

        usize index;
        for (index = 0; index < m_drawables.count(); ++index)
        {
            ref<imgui_drawable> im = m_drawables[index];

            if (im.is_valid() && im->is_enabled())
            {
                im->draw();
            }
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    bool imgui_manager::is_enabled() const noexcept
    {
        return m_enabled;
    }

    void imgui_manager::set_enabled(bool value) noexcept
    {
        m_enabled = value;
    }

    void imgui_manager::lose_focus() noexcept
    {
        ImGui::SetWindowFocus(nullptr);
    }

    ImVec4 imgui_manager::get_global_background_color() const noexcept
    {
        return m_global_background_color;
    }

    ImVec4 imgui_manager::get_global_border_color() const noexcept
    {
        return m_global_border_color;
    }

    ImVec4 imgui_manager::get_global_text_color() const noexcept
    {
        return m_global_text_color;
    }

    float imgui_manager::get_global_border_size() const noexcept
    {
        return m_global_border_size;
    }

    void imgui_manager::set_global_background_color(const ImVec4 &value) noexcept
    {
        m_global_background_color = value;
    }

    void imgui_manager::set_global_border_color(const ImVec4 &value) noexcept
    {
        m_global_border_color = value;
    }

    void imgui_manager::set_global_text_color(const ImVec4 &value) noexcept
    {
        m_global_text_color = value;
    }

    void imgui_manager::set_global_border_size(float border_size) noexcept
    {
        m_global_border_size = border_size;
    }

    ref<imgui_debug_panel> imgui_manager::get_debug_panel() const noexcept
    {
        return m_debug_panel;
    }

    ref<imgui_chat> imgui_manager::get_chat() const noexcept
    {
        return m_chat;
    }
} // namespace deep
