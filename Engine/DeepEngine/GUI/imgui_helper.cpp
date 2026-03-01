#include "DeepEngine/GUI/imgui_helper.hpp"

#include <cstdarg>

namespace deep
{
    void imgui_helper::print(const char *fmt, ...) noexcept
    {
        va_list args;
        va_start(args, fmt);

        ImGui::TextV(fmt, args);

        va_end(args);
    }

    void imgui_helper::print_bullet(const char *fmt, ...) noexcept
    {
        va_list args;
        va_start(args, fmt);

        ImGui::BulletTextV(fmt, args);

        va_end(args);
    }

    void imgui_helper::print_separator(const char *text) noexcept
    {
        ImGui::SeparatorText(text);
    }

    void imgui_helper::image(const ref<D3D::texture> &tex, float width, float height) noexcept
    {
        if (!tex.is_valid())
        {
            return;
        }

        ImGui::Image((ImTextureID) tex->get(), ImVec2(width, height));
    }

    void imgui_helper::spacing() noexcept
    {
        ImGui::Spacing();
    }

    void imgui_helper::same_line() noexcept
    {
        ImGui::SameLine();
    }
} // namespace deep
