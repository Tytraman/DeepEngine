#ifndef DEEP_ENGINE_IMGUI_HELPER_HPP
#define DEEP_ENGINE_IMGUI_HELPER_HPP

#include "DeepEngine/deep_engine_export.h"
#include "D3D/texture.hpp"

#include <imgui.h>

namespace deep
{
    class DEEP_ENGINE_API imgui_helper
    {
      public:
        imgui_helper()                                = delete;
        imgui_helper(const imgui_helper &)            = delete;
        imgui_helper &operator=(const imgui_helper &) = delete;

        static void print(const char *fmt, ...) noexcept IM_FMTARGS(1);
        static void print_separator(const char *text) noexcept;
        static void print_bullet(const char *fmt, ...) noexcept IM_FMTARGS(1);

        static void image(const ref<D3D::texture> &tex, float width, float height) noexcept;

        static void spacing() noexcept;
        static void same_line() noexcept;
    };
} // namespace deep

#endif
