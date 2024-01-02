#include "DE/window.hpp"
#include "DE/def.hpp"
#include "DE/core.hpp"
#include "DE/key.hpp"
#include "DE/ecs/scene.hpp"
#include "DE/ecs/system.hpp"
#include "DE/resources.hpp"
#include "DE/drivers/opengl/core.hpp"

#include <glad/glad.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include <string>

namespace deep
{

    /*
    =============================
    window::internalEventCallback
    =============================
    */
    void window::internalEventCallback(devent e)
    {
        static bool insertPressed = false;
        static bool f2Pressed     = false;
        static bool f11Pressed    = false;
        static int lastWindowWidth  = get_width();
        static int lastWindowHeight = get_height();

        // Vérifie le type d'évènement qui vient de se produire.
        switch(e->getType())
        {
            default:
                break;
            case event_type::KeyDown:
            {
                switch(e->getKeysym())
                {
                    default:
                        break;
                    // Debug menu toggle.
                    case dkey::Insert:
                    {
                        if(!insertPressed)
                        {
                            m_ShowDebugPanel = !m_ShowDebugPanel;
                            SDL_SetRelativeMouseMode((SDL_bool) !m_ShowDebugPanel);
                            set_cursor_position(get_width() / 2, get_height() / 2);
                            insertPressed = true;
                        }
                    } break;
                    // Screenshot.
                    case dkey::F2:
                    {
                        if(!f2Pressed)
                        {
                            resource_manager *resourcesManager = resource_manager::get_singleton();

                            f2Pressed = true;

                            uint32_t year, month, day, hour, minute, second;

                            core::getLocalTime(&year, &month, &day, &hour, &minute, &second, nullptr);
                            string destpath = resourcesManager->getScreenshotsFolder();
                            destpath.append("screenshot_");
                            destpath.append(std::to_string(year).c_str());
                            destpath.append("_");
                            destpath.append(std::to_string(month).c_str());
                            destpath.append("_");
                            destpath.append(std::to_string(day).c_str());
                            destpath.append("_");
                            destpath.append(std::to_string(hour).c_str());
                            destpath.append("_");
                            destpath.append(std::to_string(minute).c_str());
                            destpath.append("_");
                            destpath.append(std::to_string(second).c_str());
                            destpath.append(".bmp");

                            if(m_FRB.saveTextureAsImage(destpath.str()))
                            {
                                printf(DE_TERM_FG_GREEN "Screenshot saved" DE_TERM_RESET ": %s\n", destpath.str());
                            }
                        }
                    } break;
                    // Fullscreen toggle.
                    case dkey::F11:
                    {
                        if(!f11Pressed)
                        {
                            if(SDL_GetWindowFlags(m_Window) & SDL_WINDOW_FULLSCREEN)
                            {
                                SDL_SetWindowFullscreen(m_Window, 0);
                                SDL_SetWindowSize(m_Window, lastWindowWidth, lastWindowHeight);
                                GL3::core::update_viewport(lastWindowWidth, lastWindowHeight);
                                set_cursor_position(get_width() / 2, get_height() / 2);

                                m_FRB.resize("primary_frb", lastWindowWidth, lastWindowHeight);
                            }
                            else
                            {
                                SDL_DisplayMode DM;
                                SDL_GetCurrentDisplayMode(0, &DM);

                                lastWindowWidth  = get_width();
                                lastWindowHeight = get_height();
                                
                                SDL_SetWindowSize(m_Window, DM.w, DM.h);
                                GL3::core::update_viewport(DM.w, DM.h);
                                SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN);
                                set_cursor_position(get_width() / 2, get_height() / 2);

                                m_FRB.resize("primary_frb", DM.w, DM.h);
                            }
                            f11Pressed = true;
                        }
                    } break;
                }
            } break;
            case event_type::KeyUp:
            {
                switch(e->getKeysym())
                {
                    default:
                        break;
                    case dkey::Insert:
                    {
                        insertPressed = false;
                    } break;
                    case dkey::F2:
                    {
                        f2Pressed = false;
                    } break;
                    case dkey::F11:
                    {
                        
                        f11Pressed = false;
                    } break;
                }
            } break;
            case event_type::Window:
            {
                // Vérifie le type d'évènement de fenêtre qui vient de se produire
                switch(e->getWindowEventType())
                {
                    default:
                        break;
                    case events::WindowResized:
                    {	// Redimension de la fenêtre
                        size newSize = e->getWindowSize();
                        GL3::core::update_viewport(newSize.width, newSize.height);
                        //printf("window resized: %dx%d\n", newSize.width, newSize.height);
                    } break;
                }
            } break;
        }
    }

    /*
    ==============
    window::window
    ==============
    */
    window::window(uint16_t targetMS, uint16_t targetFPS)
        : m_Window(NULL),
          m_EventCallback(default_input_callback),
          m_UpdateCallback(nullptr),
          m_TargetMSPerUpdate(targetMS),
          m_TargetFPS(targetFPS),
          m_Running(false),
          m_ShowDebugPanel(false)
    { }

    /*
    ==============
    window::create
    ==============
    */
    error_status window::create(window &win, const char *title, const size &size)
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 32);

        win.m_Window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.width, size.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if(win.m_Window == NULL)
            return error_status::CreateWindowSDL;

        SDL_SetRelativeMouseMode(SDL_TRUE);
        win.set_cursor_position(win.get_width() / 2, win.get_height() / 2);
        
        if(!GL3::gl_renderer::create(win.m_Renderer, &win)) {
            win.destroy();
            return error_status::GLCreateContext;
        }

        gladLoadGL();
        
        glViewport(0, 0, size.width, size.height);

        // Permet de tester la profondeur lors du rendu afin de ne pas superposer les triangles.
        DE_GL_CALL(glEnable(GL_DEPTH_TEST));
        DE_GL_CALL(glEnable(GL_MULTISAMPLE));

        // Active la fonctionnalité d'OpenGL qui permet de gérer la transparence des couleurs lorsque nécessaire.
        DE_GL_CALL(glEnable(GL_BLEND));
        DE_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        DE_GL_CALL(glEnable(GL_CULL_FACE));
        GL3::core::set_cull_face(GL3::core::gl_cull_face::Back);
        DE_GL_CALL(glFrontFace(GL_CCW));

        GL3::texture_manager *textureManager = GL3::texture_manager::get_singleton();

        // Une fois qu'OpenGL a bien été initialisé,
        // on crée une texture blanche de 1x1 par défaut.

        uint8_t colors[] =
        {
            255, 255, 255, 255
        };

        GL3::gl_id whiteTex = textureManager->create_2D("default_white");
        textureManager->bind(whiteTex, 0);

        textureManager->set_texture_wrapping_s(GL3::texture_manager::gl_texture_wrap::Repeat);
        textureManager->set_texture_wrapping_t(GL3::texture_manager::gl_texture_wrap::Repeat);
        textureManager->set_texture_filtering(GL3::texture_manager::gl_texture_filter::Nearest);

        textureManager->transmit_texture(colors, 1, 1, image_color_space::RGBA);

        textureManager->set_white_texture(whiteTex);

        if(!win.m_FRB.create("primary_frb", win.get_width(), win.get_height()))
        {
            fprintf(stderr, "Unable to create the Frame Render Buffer.\n");
        }

        return error_status::NoError;
    }

    /*
    ===============
    window::destroy
    ===============
    */
    void window::destroy()
    {
        m_PreEventCallbacks.free();

        // Détruit la fenêtre
        SDL_DestroyWindow(m_Window);
        m_Window = NULL;
    }

    void __executePreEventCallbacks(list<window::pre_event_callback> &preEventCallbacks, window &window)
    {
        size_t length = preEventCallbacks.count();
        size_t i;
        window::pre_event_callback callback;

        for(i = 0; i < length; ++i)
        {
            callback = preEventCallbacks[i];
            
            if(callback != nullptr)
                callback(window);
        }
    }

    /*
    ===========
    window::run
    ===========
    */
    void window::run()
    {
        devent e;
        uint64_t lag = 0;
        uint64_t previous = core::getTick();
        uint64_t end;
        uint64_t current;
        uint64_t elapsed;
        uint32_t cn = 0;
        uint32_t updates = 0;
        std::string title(get_title());

        system_manager *systemManager = system_manager::get_singleton();

        m_Running = true;

        // Met à jour l'état du clavier.
        key::update();

        // Boucle infinie du jeu.
        uint64_t startTime = core::getCurrentTimeMillis(), endTime;
        while(m_Running)
        {
            scene_id sceneID = scene::getActiveSceneID();

            // Calcule le temps passé à faire la boucle
            current = core::getTick();		// Récupère le tick actuel.
            elapsed = current - previous;	// Compte combien de temps s'est écoulé entre la dernière itération et maintenant.
            previous = current;				// Sauvegarde le temps actuel pour qu'il devienne le temps précédent à la prochaine itération.
            lag += elapsed;					// Plus le système est lent, et plus le lag sera élevé.

            __executePreEventCallbacks(m_PreEventCallbacks, *this);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            if(m_ShowDebugPanel)
                im_gui_debug_menu::render(this);

            // Récupère les évènements système, les entrées utilisateurs etc...
            // et exécute un callback s'il y en a un.
            while((e = poll_event()) != nullptr)
            {
                // Gère les évènements internes.
                internalEventCallback(e);

                // Appelle le callback utilisateur.
                if(m_EventCallback != nullptr)
                    m_EventCallback(*this, e);

                mem::free(e);
            }

            // Ne met à jour l'état du jeu que si le lag est supérieur au ms visé.
            // m_TargetMSPerUpdate est une constante qui ne doit jamais changer car elle impacte directement
            // la vitesse du jeu et l'entièreté de la physique.
            while(lag >= m_TargetMSPerUpdate)
            {
                // Exécute la fonction update callback de l'utilisateur.
                if(m_UpdateCallback != nullptr)
                    m_UpdateCallback(*this);

                // Exécute les systèmes créés par l'utilisateur.
                systemManager->executeSystems();

                // Applique l'accélération à toutes les entités en possédant.
                systemManager->accelerationSystem();

                // Applique la vélocité à toutes les entités en possédant.
                systemManager->velocitySystem();

                // Les collisions sont vérifiées après tous les déplacements possibles afin d'éviter les problèmes.
                systemManager->colliderSystem();

                lag -= m_TargetMSPerUpdate;
                updates++;

                // Note: plus on reste longtemps dans cette boucle, et plus le lag sera élevé
            }

            // Fait le rendu final de la frame !
            systemManager->renderSystem(m_Renderer, m_FRB, sceneID);

            end = core::getTick();

            cn++;
            endTime = core::getCurrentTimeMillis();
            elapsed = endTime - startTime;

            if(elapsed >= 1000)
            {
                // TODO: Retirer au moment de la release.
                set_title((title + " | UPS: " + std::to_string(updates) + " | FPS: " + std::to_string(cn)).c_str());

                cn = 0;
                updates = 0;
                startTime = endTime;
            }
        }
    }

    /*
    =================
    window::poll_event
    =================
    */
    devent window::poll_event() const
    {
        devent e = devent_s::create();
        if(!e->pollEvent())
        {
            mem::free(e);
            return nullptr;
        }

        return e;
    }

    /*
    ============================
    window::default_input_callback
    ============================
    */
    void window::default_input_callback(window &window, devent e)
    {
        switch(e->getType())
        {
            default: break;
            case event_type::Window:
            {
                switch(e->getWindowEventType())
                {
                    default:
                        break;
                    case events::WindowClosed:
                    {
                        window.m_Running = false;
                    } break;
                }
            } break;
            case event_type::KeyDown:
            {
                switch(e->getKeysym())
                {
                    default:
                        break;
                    case dkey::Esc:
                    {
                        window.m_Running = false;
                    } break;
                }
            } break;
        }
    }

    /*
    ===============
    window::get_size
    ===============
    */
    size window::get_size() const
    {
        size windowSize;
        SDL_GetWindowSize(m_Window, (int *) &windowSize.width, (int *) &windowSize.height);
        return windowSize;
    }

    /*
    ================
    window::get_width
    ================
    */
    uint32_t window::get_width() const
    {
        int w, h;
        SDL_GetWindowSize(m_Window, &w, &h);
        return w;
    }

    /*
    =================
    window::get_height
    =================
    */
    uint32_t window::get_height() const
    {
        int w, h;
        SDL_GetWindowSize(m_Window, &w, &h);
        return h;
    }

    const char *window::get_title() const
    {
        return SDL_GetWindowTitle(m_Window);
    }

    void window::set_title(const char *title) const
    {
        SDL_SetWindowTitle(m_Window, title);
    }

    void window::set_cursor_position(int x, int y)
    {
        SDL_WarpMouseInWindow(m_Window, x, y);
    }

}

