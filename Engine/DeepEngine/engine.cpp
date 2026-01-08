#include "engine.hpp"
#include "D3D/shader/shader_factory.hpp"
#include "D3D/drawable/drawable_factory.hpp"

#include <DeepLib/lib.hpp>
#include <DeepLib/context.hpp>
#include <DeepLib/time/time.hpp>
#include <DeepLib/string/string_native.hpp>
#include <DeepLib/stream/file_stream.hpp>

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

        const D3D11_INPUT_ELEMENT_DESC ied[] = {
            { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, sizeof(float) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        file_stream fs = file_stream(get_context(), DEEP_TEXT_NATIVE("test_triangle_vs.cso"), core_fs::file_mode::Open, core_fs::file_access::Read, core_fs::file_share::Read);
        fs.open();

        ref<D3D::vertex_shader> vs = D3D::shader_factory::create_vertex_shader(get_context(), &fs, ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC), m_graphics->get_device());
        fs.close();

        fs = file_stream(get_context(), DEEP_TEXT_NATIVE("test_triangle_ps.cso"), core_fs::file_mode::Open, core_fs::file_access::Read, core_fs::file_share::Read);
        fs.open();

        ref<D3D::pixel_shader> ps = D3D::shader_factory::create_pixel_shader(get_context(), &fs, m_graphics->get_device());
        fs.close();

        ref<D3D::rectangle> rect = D3D::drawable_factory::create_rectangle(get_context(), vs, ps, m_graphics->get_device(), m_graphics->get_device_context());

        m_graphics->add_drawable(ref_cast<D3D::drawable>(rect));

        // Boucle infinie du jeu. S'arrête quand l'utilisateur ferme la fenêtre.
        while (m_window->process_message())
        {
            // Calcule le temps passé à faire la boucle.
            tick_count          = time::get_tick_count();
            elapsed             = tick_count - previous_tick_count;
            previous_tick_count = tick_count;
            lag += elapsed; // Plus le système est lent, et plus le lag sera élevé.

            m_graphics->clear_buffer(0.0f, 0.0f, 0.0f);

            m_graphics->draw_all();

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
