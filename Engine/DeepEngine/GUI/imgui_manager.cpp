#include "DeepEngine/GUI/imgui_manager.hpp"

#include <DeepLib/memory/memory.hpp>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

namespace deep
{
    imgui_manager::imgui_manager(const ref<ctx> &context, bool enabled) noexcept
            : object::object(context),
              m_enabled(enabled)
    {
    }

    ref<imgui_manager> imgui_manager::create(const ref<ctx> &context) noexcept
    {
        imgui_manager *im = mem::alloc_type<imgui_manager>(context.get(), context, true);

        if (im == nullptr)
        {
            return ref<imgui_manager>();
        }

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

    void imgui_manager::draw() noexcept
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

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
} // namespace deep
