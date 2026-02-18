#include "DeepEngine/GUI/imgui_drawable.hpp"

namespace deep
{
    imgui_drawable::imgui_drawable(const ref<ctx> &context, bool enabled) noexcept
            : object(context), m_enabled(enabled)
    {
    }

    bool imgui_drawable::is_enabled() const noexcept
    {
        return m_enabled;
    }

    void imgui_drawable::set_enabled(bool value) noexcept
    {
        m_enabled = value;
    }
} // namespace deep
