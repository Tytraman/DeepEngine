#ifndef DEEP_ENGINE_IMGUI_MANAGER_HPP
#define DEEP_ENGINE_IMGUI_MANAGER_HPP

#include "DeepEngine/deep_engine_export.h"

#include <DeepLib/object.hpp>
#include <DeepLib/memory/ref_counted.hpp>

namespace deep
{
    class DEEP_ENGINE_API imgui_manager : public object
    {
      public:
        imgui_manager()                                 = delete;
        imgui_manager(const imgui_manager &)            = delete;
        imgui_manager &operator=(const imgui_manager &) = delete;

        static ref<imgui_manager> create(const ref<ctx> &context) noexcept;

        void init(window_handle win) noexcept;
        void shutdown() noexcept;

        void draw() noexcept;

        bool is_enabled() const noexcept;
        void set_enabled(bool value) noexcept;

      protected:
        imgui_manager(const ref<ctx> &context, bool enabled) noexcept;

      private:
        bool m_enabled;

      public:
        friend memory_manager;
    };
} // namespace deep

#endif
