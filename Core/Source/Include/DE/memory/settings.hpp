#ifndef __DEEP_ENGINE_SETTINGS_HPP__
#define __DEEP_ENGINE_SETTINGS_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/string.hpp"
#include "DE/io/stream.hpp"

namespace deep
{

    class engine_settings
    {

        public:
            DE_API static engine_settings *get_singleton();
            DE_API static void set_singleton(engine_settings *ptr);

            DE_API bool init(stream *inputStream);

            DE_API bool get_debug_mode();
            DE_API string &get_resources_directory();
            DE_API string &get_imgui_font_filename();

        private:
            engine_settings();

            bool m_DebugMode;
            string m_ResourcesDirectory;
            string m_ImGuiFontFilename;

        public:
            engine_settings(const engine_settings &) = delete;

            friend memory_manager;

        private:
            static engine_settings *g_EngineSettings;

    };

    /*
    ==============================
    engine_settings::get_singleton
    ==============================
    */
    inline engine_settings *engine_settings::get_singleton()
    {
        return g_EngineSettings;
    }

    /*
    ==============================
    engine_settings::set_singleton
    ==============================
    */
    inline void engine_settings::set_singleton(engine_settings *ptr)
    {
        g_EngineSettings = ptr;
    }

    /*
    ===============================
    engine_settings::get_debug_mode
    ===============================
    */
    inline bool engine_settings::get_debug_mode()
    {
        return m_DebugMode;
    }

    /*
    ========================================
    engine_settings::get_resources_directory
    ========================================
    */
    inline string &engine_settings::get_resources_directory()
    {
        return m_ResourcesDirectory;
    }

    /*
    ========================================
    engine_settings::get_resources_directory
    ========================================
    */
    inline string &engine_settings::get_imgui_font_filename()
    {
        return m_ImGuiFontFilename;
    }

}

#endif