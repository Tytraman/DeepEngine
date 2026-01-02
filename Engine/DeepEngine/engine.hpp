#ifndef DEEP_ENGINE_ENGINE_HPP
#define DEEP_ENGINE_ENGINE_HPP

#include "DeepEngine/deep_engine_export.h"

#include <DeepLib/object.hpp>
#include <DeepLib/window/window.hpp>

#include "D3D/graphics.hpp"

namespace deep
{
    class DEEP_ENGINE_API engine : public object
    {
      public:
        static engine create();

        void run();

        ref<window> get_window();

        bool is_valid() const;

      private:
        ref<window> m_window;
        ref<D3D::graphics> m_graphics;
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
