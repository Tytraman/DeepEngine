#ifndef __DEEP_ENGINE_WINDOW_HPP__
#define __DEEP_ENGINE_WINDOW_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/error.hpp"
#include "DE/events.hpp"
#include "DE/renderer.hpp"
#include "DE/memory/list.hpp"
#include "DE/imgui/deimgui.hpp"
#include "DE/drivers/opengl/framerenderbuffer.hpp"
#include "DE/drivers/opengl/renderer.hpp"

#include <stdint.h>

#include <SDL.h>

namespace deep
{

#if DE_WINDOWS

    using window_handle = HWND;

    static constexpr window_handle invalid_window_handle = nullptr;

#else
#error Need implementation
#endif


    /// @class window
    /// @brief Permet la gestion d'une fenêtre.
    class window
    {

        public:
            using pre_event_callback = void (*)(window &window);

            typedef void (*event_callback)(window &window, devent e);
            typedef void (*update_callback)(window &window);

        public:
            DE_API window(uint16_t targetMS, uint16_t targetFPS);
            
            /// @brief			Crée une fenêtre avec un titre et une taille.
            /// @param win		La fenêtre à créer.
            /// @param title	Titre de la fenêtre.
            /// @param size		Taille de la fenêtre.
            /// @return			Le code d'erreur.
            DE_API static error_status create(window &win, const char *title, const size &size);

            DE_API static window_handle find(const char *className);
            DE_API static void show(window_handle win);
            DE_API static void focus(window_handle win);
            DE_API static void set_foreground(window_handle win);
            DE_API static void set_active(window_handle win);
            
            /// @brief Détruit la fenêtre ainsi que tous ses composants internes.
            DE_API void destroy();
            
            /// @brief Game Loop
            DE_API void run();

            DE_API bool add_pre_event_callback(pre_event_callback callback);
            
            /// @brief			Récupère et retire l'évènement le plus vieux de la queue.
            /// @return			Un \ref de::devent lorsqu'un évènement s'est produit ou \c nullptr s'il n'y a aucun évènement.
            /// @remark			La valeur retournée par cette méthode doit être \c delete.
            DE_API devent poll_event() const;

            DE_API void set_cursor_position(int x, int y);

            DE_API static void default_input_callback(window &window, devent e);


            //===== GETTERS =====//

            DE_API SDL_Window *get_window();
            DE_API size get_size() const;
            DE_API uint32_t get_width() const;
            DE_API uint32_t get_height() const;
            DE_API const char *get_title() const;
            DE_API GL3::gl_renderer &get_renderer();
            DE_API bool is_showing_debug_panel() const;

            //===== SETTERS =====//

            DE_API void set_event_callback(event_callback callabck);
            DE_API void set_update_callback(update_callback callback);
            DE_API void set_title(const char *title) const;
            DE_API void set_showing_debug_panel(bool value);

        private:
            SDL_Window              *m_Window;
            list<pre_event_callback> m_PreEventCallbacks;
            event_callback           m_EventCallback;
            update_callback          m_UpdateCallback;
            GL3::gl_renderer         m_Renderer;
            uint16_t                 m_TargetMSPerUpdate;
            uint16_t                 m_TargetFPS;
            bool                     m_Running;
            bool                     m_ShowDebugPanel;
            GL3::framerenderbuffer   m_FRB;

            void internalEventCallback(devent e);

    };

    /*
    ============
    window::find
    ============
    */
    inline window_handle window::find(const char *className)
    {
#if DE_WINDOWS
        return FindWindowA(className, nullptr);
#else
#error Need implementation
#endif
    }

    /*
    ============
    window::show
    ============
    */
    inline void window::show(window_handle win)
    {
#if DE_WINDOWS
        ShowWindow(win, SW_SHOWNORMAL);
#else
#error Need implementation
#endif
    }

    /*
    =============
    window::focus
    =============
    */
    inline void window::focus(window_handle win)
    {
#if DE_WINDOWS
        SetFocus(win);
#else
#error Need implementation
#endif
    }

    /*
    =====================
    window::set_foreground
    =====================
    */
    inline void window::set_foreground(window_handle win)
    {
#if DE_WINDOWS
        SetForegroundWindow(win);
#else
#error Need implementation
#endif
    }

    inline void window::set_active(window_handle win)
    {
#if DE_WINDOWS
        SetActiveWindow(win);
#else
#error Need implementation
#endif
    }

    /*
    ===========================
    window::add_pre_event_callback
    ===========================
    */
    inline bool window::add_pre_event_callback(pre_event_callback callback)
    {
        return m_PreEventCallbacks.add(callback);
    }

    /*
    =================
    window::get_window
    =================
    */
    inline SDL_Window *window::get_window()
    {
        return m_Window;
    }

    /*
    ===================
    window::get_renderer
    ===================
    */
    inline GL3::gl_renderer &window::get_renderer()
    {
        return m_Renderer;
    }

    /*
    ===========================
    window::is_showing_debug_panel
    ===========================
    */
    inline bool window::is_showing_debug_panel() const
    {
        return m_ShowDebugPanel;
    }

    /*
    ========================
    window::set_event_callback
    ========================
    */
    inline void window::set_event_callback(event_callback callback)
    {
        m_EventCallback = callback;
    }

    /*
    =========================
    window::set_update_callback
    =========================
    */
    inline void window::set_update_callback(update_callback callback)
    {
        m_UpdateCallback = callback;
    }

    /*
    ============================
    window::set_showing_debug_panel
    ============================
    */
    inline void window::set_showing_debug_panel(bool value)
    {
        if(value)
            im_gui_debug_menu::addWindow(this);
        else
            im_gui_debug_menu::removeWindow(this);
    }

}

#endif