#ifndef DEEP_ENGINE_ENGINE_HPP
#define DEEP_ENGINE_ENGINE_HPP

#include "DeepEngine/deep_engine_export.h"

#include <DeepLib/object.hpp>
#include <DeepLib/window/window.hpp>
#include <DeepLib/maths/vec.hpp>
#include <DeepLib/maths/mat.hpp>

#include "D3D/graphics.hpp"

namespace deep
{
    class DEEP_ENGINE_API engine : public object
    {
      public:
        static engine create();

        void run();

        uint64 get_time_millis() const noexcept;
        float get_time_seconds() const noexcept;

        ref<window> get_window();

        bool is_valid() const;

      private:
        bool process_inputs() noexcept;

      private:
        ref<window> m_window;
        ref<D3D::graphics> m_graphics;
        uint64 m_startup_tick_count;
        uint64 m_startup_time_millis;
        uint32 m_FPS;
        fvec3 m_player_location;
        fmat4 m_projection;

      protected:
        engine(const ref<ctx> &context, const fvec3 &player_location) noexcept;
    };

    inline ref<window> engine::get_window()
    {
        return m_window;
    }

    inline bool engine::is_valid() const
    {
        return m_window.is_valid();
    }
} // namespace deep

#endif
