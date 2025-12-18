#include "engine.hpp"

#include <DeepLib/lib.hpp>
#include <DeepLib/context.hpp>

namespace deep
{
    engine engine::create()
    {
        ref<ctx> context = lib::create_ctx();

        engine eng;

        eng.m_window = window::create(context, DEEP_TEXT_NATIVE("DeepEngineClass"), DEEP_TEXT_NATIVE("Deep Engine"), 0, 0, 800, 400);

        if (eng.m_window.is_valid())
        {
            eng.m_window->show();
        }

        return eng;
    }

    void engine::run()
    {
        while (m_window->process_message())
        {
        }
    }
} // namespace deep
