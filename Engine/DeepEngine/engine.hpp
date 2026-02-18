#ifndef DEEP_ENGINE_ENGINE_HPP
#define DEEP_ENGINE_ENGINE_HPP

#include "DeepEngine/deep_engine_export.h"

#include <DeepLib/object.hpp>
#include <DeepLib/window/window.hpp>
#include <DeepLib/maths/vec.hpp>
#include <DeepLib/maths/mat.hpp>

#include "DeepEngine/camera.hpp"
#include "DeepEngine/GUI/gui.hpp"
#include "DeepEngine/GUI/imgui_manager.hpp"
#include "D3D/graphics.hpp"

namespace deep
{
    class DEEP_ENGINE_API engine : public object
    {
      public:
        static ref<engine> create() noexcept;

        void run() noexcept;

        uint64 get_time_millis() const noexcept;
        float get_time_seconds() const noexcept;

        ref<window> get_window();
        uint32 get_FPS() const noexcept;
        gui_mode get_gui_mode() const noexcept;

      private:
        bool process_inputs() noexcept;

      private:
        ref<window> m_window;
        ref<D3D::graphics> m_graphics;
        ref<imgui_manager> m_imgui_manager;
        uint64 m_startup_tick_count;
        uint64 m_startup_time_millis;
        uint32 m_FPS;
        ref<camera> m_camera;
        gui_mode m_gui_mode;

      protected:
        engine(const ref<ctx> &context) noexcept;

      public:
        friend memory_manager;
    };

    inline ref<window> engine::get_window()
    {
        return m_window;
    }

    inline uint32 engine::get_FPS() const noexcept
    {
        return m_FPS;
    }

    inline gui_mode engine::get_gui_mode() const noexcept
    {
        return m_gui_mode;
    }
} // namespace deep

#endif
