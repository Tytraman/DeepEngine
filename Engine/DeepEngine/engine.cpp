#include "engine.hpp"

#include <DeepLib/lib.hpp>
#include <DeepLib/context.hpp>
#include <DeepLib/time/time.hpp>
#include <DeepLib/string/string_native.hpp>

#include <stdio.h>

namespace deep
{
    engine engine::create()
    {
        ref<ctx> context = lib::create_ctx();

        engine eng = engine(context);

        eng.m_startup_tick_count  = time::get_tick_count();
        eng.m_startup_time_millis = time::get_current_time_millis();
        eng.m_FPS                 = 0;

        eng.m_window = window::create(context, DEEP_TEXT_NATIVE("DeepEngineClass"), DEEP_TEXT_NATIVE("Deep Engine"), 0, 0, 1200, 800);

        if (eng.m_window.is_valid())
        {
            eng.m_graphics = D3D::graphics::create(context, *eng.m_window);

            eng.m_window->show();
        }

        return eng;
    }

    void engine::run()
    {
        keyboard &kbd = m_window->get_keyboard();
        mouse &ms     = m_window->get_mouse();
        uint64 tick_count;
        uint64 elapsed;
        uint64 previous_tick_count = time::get_tick_count();
        uint64 lag                 = 0;
        uint32 cn                  = 0;
        uint64 start_time          = time::get_current_time_millis();
        uint64 end_time;

        // Boucle infinie du jeu. S'arrête quand l'utilisateur ferme la fenêtre.
        while (m_window->process_message())
        {
            // Calcule le temps passé à faire la boucle.
            tick_count          = time::get_tick_count();
            elapsed             = tick_count - previous_tick_count;
            previous_tick_count = tick_count;
            lag += elapsed; // Plus le système est lent, et plus le lag sera élevé.

            m_graphics->clear_buffer(0.0f, 0.0f, 0.0f);

            m_graphics->draw_test_triangle(get_time_seconds() * 10.0f);

            m_graphics->end_frame();

            // Met à jour le nombre de FPS.
            cn++;
            end_time = time::get_current_time_millis();
            elapsed  = end_time - start_time;

            if (elapsed >= 1000)
            {
                m_FPS = cn;

                cn         = 0;
                start_time = end_time;

                // get_context()->out() << "FPS: " << m_FPS << "\r\n";
            }
        }
    }

    uint64 engine::get_time_millis() const noexcept
    {
        return time::get_current_time_millis() - m_startup_time_millis;
    }

    float engine::get_time_seconds() const noexcept
    {
        uint64 time_millis = time::get_current_time_millis() - m_startup_time_millis;

        return time_millis / 1000.0f;
    }
} // namespace deep
