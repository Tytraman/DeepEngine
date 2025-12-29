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

        eng.m_window = window::create(context, DEEP_TEXT_NATIVE("DeepEngineClass"), DEEP_TEXT_NATIVE("Deep Engine"), 0, 0, 800, 400);

        if (eng.m_window.is_valid())
        {
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
            while (!ms.is_empty())
            {
                mouse::event me = ms.read();

                switch (me.get_action())
                {
                    default:
                        break;
                    case mouse::event::action::LeftPress:
                    {
                        printf("LeftPress\n");
                    }
                    break;
                    case mouse::event::action::RightPress:
                    {
                        printf("RightPress\n");
                    }
                    break;
                    case mouse::event::action::MiddlePress:
                    {
                        printf("MiddlePress\n");
                    }
                    break;
                    case mouse::event::action::Enter:
                    {
                        printf("Enter\n");
                    }
                    break;
                    case mouse::event::action::Leave:
                    {
                        printf("Leave\n");
                    }
                    break;
                }
            }
        }
    }
} // namespace deep
