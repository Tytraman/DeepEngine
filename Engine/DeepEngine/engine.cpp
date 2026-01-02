#include "engine.hpp"

#include <DeepLib/lib.hpp>
#include <DeepLib/context.hpp>

#include <stdio.h>

namespace deep
{
    engine engine::create()
    {
        ref<ctx> context = lib::create_ctx();

        engine eng;

        eng.m_window = window::create(context, DEEP_TEXT_NATIVE("DeepEngineClass"), DEEP_TEXT_NATIVE("Deep Engine"), 0, 0, 1200, 800);

        if (eng.m_window.is_valid())
        {
            eng.m_graphics = D3D::graphics::create(context, eng.m_window->get_handle());

            eng.m_window->show();
        }

        return eng;
    }

    void engine::run()
    {
        keyboard &kbd = m_window->get_keyboard();
        mouse &ms     = m_window->get_mouse();

        while (m_window->process_message())
        {
            m_graphics->clear_buffer(0.0f, 0.0f, 0.0f);

            m_graphics->end_frame();
        }
    }
} // namespace deep
