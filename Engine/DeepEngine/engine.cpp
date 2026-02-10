#include "engine.hpp"
#include "D3D/resource_factory.hpp"
#include "D3D/shader/shader_factory.hpp"
#include "D3D/drawable/drawable_factory.hpp"
#include "D3D/device_context.hpp"
#include "D3D/buffer/per_frame_buffer.hpp"

#include <DeepLib/lib.hpp>
#include <DeepLib/context.hpp>
#include <DeepLib/time/time.hpp>
#include <DeepLib/string/string_native.hpp>
#include <DeepLib/stream/file_stream.hpp>
#include <DeepCore/display.hpp>
#include <DeepLib/filesystem/filesystem.hpp>
#include <DeepLib/image/png.hpp>

namespace deep
{
    ref<engine> engine::create()
    {
        ref<ctx> context = lib::create_ctx();

        if (!context.is_valid())
        {
            return ref<engine>();
        }

        string_native cwd = fs::get_cwd(context);

        context->out() << DEEP_TEXT_UTF8("Current working directory: ") << *cwd << DEEP_TEXT_UTF8("\r\n");

        ref<engine> eng = ref<engine>(context, mem::alloc_type<engine>(context.get(), context));

        if (!eng.is_valid())
        {
            context->err() << DEEP_TEXT_UTF8("[ERROR] Engine creation failed.\r\n");

            return ref<engine>();
        }

        eng->m_startup_tick_count  = time::get_tick_count();
        eng->m_startup_time_millis = time::get_current_time_millis();

        uint32 primary_monitor_index = 0;
        uint32 width;
        uint32 height;
        uint32 frequency;

        core_display::get_primary_monitor_index(&primary_monitor_index);
        core_display::get_monitor_infos(ctx::get_internal_ctx(context.get()), primary_monitor_index, &width, &height, &frequency);

        /*width /= 3;
        height /= 3;*/

        context->out() << DEEP_TEXT_UTF8("Creating window in ") << width << DEEP_TEXT_UTF8("x") << height << DEEP_TEXT_UTF8(":") << frequency << DEEP_TEXT_UTF8("...");

        eng->m_window = window::create(context, DEEP_TEXT_NATIVE("DeepEngineClass"), DEEP_TEXT_NATIVE("Deep Engine"), core_window::style::Borderless, false, 0, 0, width, height);
        if (!eng->m_window.is_valid())
        {
            context->out() << DEEP_TEXT_UTF8(" failed\r\n");
            context->err() << DEEP_TEXT_UTF8("[ERROR] Window creation failed.\r\n");

            return ref<engine>();
        }

        eng->m_window->get_keyboard().set_auto_repeat(true);

        context->out() << DEEP_TEXT_UTF8(" OK\r\nCreating camera...");

        eng->m_camera = ref<camera>(context, mem::alloc_type<camera>(context.get(), context, fvec3(0.0f, 0.0f, 0.0f)));
        if (!eng->m_camera.is_valid())
        {
            context->out() << DEEP_TEXT_UTF8(" failed\r\n");
            context->err() << DEEP_TEXT_UTF8("[ERROR] Camera creation failed.\r\n");

            return ref<engine>();
        }
        eng->m_camera->set_lens(90.0f, static_cast<float>(eng->m_window->get_width()) / static_cast<float>(eng->m_window->get_height()), 1.0f, 1000.0f);

        context->out() << DEEP_TEXT_UTF8(" OK\r\n");

        eng->m_graphics = D3D::graphics::create(context, *eng->m_window, eng->m_camera->get_location());

        eng->m_window->show();

        return eng;
    }

    void engine::run()
    {
        uint64 tick_count;
        uint64 elapsed;
        uint64 previous_tick_count = time::get_tick_count();
        uint64 lag                 = 0;
        uint32 cn                  = 0;
        uint64 start_time          = time::get_current_time_millis();
        uint64 end_time;

        //////////////////
        // ZONE DE TEST //
        //////////////////

        const D3D11_INPUT_ELEMENT_DESC cube_ied[] = {
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        const D3D11_INPUT_ELEMENT_DESC textured_cube_ied[] = {
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        file_stream fs = file_stream(get_context(), DEEP_TEXT_NATIVE("test_cube_vs.cso"), core_fs::file_mode::Open, core_fs::file_access::Read, core_fs::file_share::Read);
        fs.open();

        ref<D3D::vertex_shader> cube_vs = D3D::shader_factory::create_vertex_shader(get_context(), &fs, cube_ied, sizeof(cube_ied) / sizeof(D3D11_INPUT_ELEMENT_DESC), m_graphics->get_device());
        fs.close();

        fs = file_stream(get_context(), DEEP_TEXT_NATIVE("test_cube_ps.cso"), core_fs::file_mode::Open, core_fs::file_access::Read, core_fs::file_share::Read);
        fs.open();

        ref<D3D::pixel_shader> cube_ps = D3D::shader_factory::create_pixel_shader(get_context(), &fs, m_graphics->get_device());
        fs.close();

        fs = file_stream(get_context(), DEEP_TEXT_NATIVE("textured_cube_vs.cso"), core_fs::file_mode::Open, core_fs::file_access::Read, core_fs::file_share::Read);
        fs.open();

        ref<D3D::vertex_shader> textured_cube_vs = D3D::shader_factory::create_vertex_shader(get_context(), &fs, textured_cube_ied, sizeof(textured_cube_ied) / sizeof(D3D11_INPUT_ELEMENT_DESC), m_graphics->get_device());
        fs.close();

        fs = file_stream(get_context(), DEEP_TEXT_NATIVE("textured_cube_ps.cso"), core_fs::file_mode::Open, core_fs::file_access::Read, core_fs::file_share::Read);
        fs.open();

        ref<D3D::pixel_shader> textured_cube_ps = D3D::shader_factory::create_pixel_shader(get_context(), &fs, m_graphics->get_device());
        fs.close();

        ref<D3D::cube> c1 = D3D::drawable_factory::create_cube(get_context(), cube_vs, cube_ps, fvec3(0.0f, 0.0f, 4.0f), fvec3(), fvec3(1.0f, 1.0f, 1.0f), m_graphics->get_device());

        m_graphics->add_drawable(ref_cast<D3D::drawable>(c1));

        fs = file_stream(get_context(),
                         DEEP_TEXT_NATIVE("Resources") DEEP_NATIVE_SEPARATOR DEEP_TEXT_NATIVE("Textures") DEEP_NATIVE_SEPARATOR DEEP_TEXT_NATIVE("texture.png"),
                         core_fs::file_mode::Open,
                         core_fs::file_access::Read,
                         core_fs::file_share::Read);
        fs.open();

        png png_source = png::load(get_context(), &fs);

        if (png_source.is_valid() && png_source.check() && png_source.read_info())
        {
            image img_tex1 = png_source.read_image();

            if (img_tex1.is_valid())
            {
                ref<D3D::texture> tex1  = D3D::resource_factory::create_texture(get_context(), img_tex1, m_graphics->get_device());
                ref<D3D::sampler> samp1 = D3D::resource_factory::create_sampler(get_context(), m_graphics->get_device());

                ref<D3D::textured_cube> tc1 = D3D::drawable_factory::create_textured_cube(
                        get_context(),
                        textured_cube_vs,
                        textured_cube_ps,
                        fvec3(4.0f, 0.0f, 4.0f),
                        fvec3(),
                        fvec3(1.0f, 1.0f, 1.0f),
                        tex1,
                        samp1,
                        m_graphics->get_device());

                m_graphics->add_drawable(ref_cast<D3D::drawable>(tc1));
            }
            else
            {
                get_context()->err() << DEEP_TEXT_UTF8("[ERROR] Cannot read image data from 'texture.png'.\r\n");
            }
        }
        else
        {
            get_context()->err() << DEEP_TEXT_UTF8("[ERROR] Cannot load 'texture.png'.\r\n");
        }

        //////////////////////
        // FIN ZONE DE TEST //
        //////////////////////

        // Boucle infinie du jeu. S'arrête quand l'utilisateur ferme la fenêtre.
        while (m_window->process_message())
        {
            // Calcule le temps passé à faire la boucle.
            tick_count          = time::get_tick_count();
            elapsed             = tick_count - previous_tick_count;
            previous_tick_count = tick_count;
            lag += elapsed; // Plus le système est lent, et plus le lag sera élevé.

            if (!process_inputs())
            {
                break;
            }

            m_graphics->clear_buffer(0.0f, 0.0f, 0.0f);

            m_graphics->draw_all(m_camera->get_projection(), m_camera->get_view());

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
            }
        }

        uint64 running_time_millis = get_time_millis();

        get_context()->out() << "DeepEngine ran for " << running_time_millis << "ms.\r\n";
    }

    bool engine::process_inputs() noexcept
    {
        keyboard &kbd = m_window->get_keyboard();
        mouse &ms     = m_window->get_mouse();

        while (!kbd.key_is_empty())
        {
            keyboard::event e = kbd.read_key();

            switch (e.get_key())
            {
                default:
                    break;
                case vkeys::Escape:
                    return false;
                case vkeys::F3:
                {
                    get_context()->out() << "FPS: " << m_FPS << "\r\n";
                }
                break;
                case vkeys::F9:
                {
                    if (kbd.key_is_pressed(vkeys::Control))
                    {
                        m_graphics->get_device_context().set_rasterizer_state(D3D::rasterizer_state::CullBackSolid);
                    }
                }
                break;
                case vkeys::F10:
                {
                    if (kbd.key_is_pressed(vkeys::Control))
                    {
                        m_graphics->get_device_context().set_rasterizer_state(D3D::rasterizer_state::CullBackWireframe);
                    }
                }
                break;
                case vkeys::F11:
                {
                    if (kbd.key_is_pressed(vkeys::Control))
                    {
                        m_graphics->get_device_context().set_rasterizer_state(D3D::rasterizer_state::CullFrontSolid);
                    }
                }
                break;
                case vkeys::F12:
                {
                    if (kbd.key_is_pressed(vkeys::Control))
                    {
                        m_graphics->get_device_context().set_rasterizer_state(D3D::rasterizer_state::CullFrontWireframe);
                    }
                }
                break;
                case vkeys::Z:
                {
                    m_camera->walk(0.5f);
                }
                break;
                case vkeys::Q:
                {
                    m_camera->strafe(-0.5f);
                }
                break;
                case vkeys::S:
                {
                    m_camera->walk(-0.5f);
                }
                break;
                case vkeys::D:
                {
                    m_camera->strafe(0.5f);
                }
                break;
                case vkeys::Up:
                {
                    m_camera->rotate_vertically(5.0f);
                }
                break;
                case vkeys::Left:
                {
                    m_camera->rotate_horizontally(-5.0f);
                }
                break;
                case vkeys::Down:
                {
                    m_camera->rotate_vertically(-5.0f);
                }
                break;
                case vkeys::Right:
                {
                    m_camera->rotate_horizontally(5.0f);
                }
                break;
                case vkeys::Spacebar:
                {
                    m_camera->move_vertically(0.5f);
                }
                break;
                case vkeys::Control:
                {
                    m_camera->move_vertically(-0.5f);
                }
                break;
            }
        }

        return true;
    }

    engine::engine(const ref<ctx> &context) noexcept
            : object(context),
              m_startup_tick_count(0),
              m_startup_time_millis(0),
              m_FPS(0)
    {
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
