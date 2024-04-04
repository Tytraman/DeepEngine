#include "core/config/settings.hpp"
#include "file/file_object.hpp"
#include "core/common.hpp"
#include "core/core.hpp"
#include "core/resources/resource_manager.hpp"

#include <stdio.h>

namespace deep
{

    engine_settings *engine_settings::g_EngineSettings = nullptr;

    /*
    ================================
    engine_settings::engine_settings
    ================================
    */
    engine_settings::engine_settings()
        :
#if DE_DEBUG
          m_DebugMode(true)
#else
          m_DebugMode(false)
#endif
    { }

    void fobj_load_warning_callback(const string &name, uint64_t line, char charactere)
    {
        fprintf(stderr, "[WARNING] File Object \"%s\" invalid charactere '%c' at line %llu. Ignoring the line.\n", name.str(), charactere, line);
    }

    /*
    =====================
    engine_settings::init
    =====================
    */
    bool engine_settings::init(stream *inputStream)
    {
        ref<stream> is(inputStream);
        resource_manager *resManager = resource_manager::get_singleton();

        if(!is.is_valid())
        {
            return false;
        }

        file_object fobj("engine_settings");

        if(!is->is_opened())
        {
            if(!is->open())
            {
                return false;
            }
        }

        if(!fobj.load(is.get(), fobj_load_warning_callback))
        {
            is->close();

            return false;
        }

        is->close();

        const char *key = "screenshots_folder";
        pair<string, string> *element = fobj.search_element(key);
        if(element == nullptr)
        {
            core::err() << DE_TERM_RESET "[" DE_TERM_FG_RED "ERROR" DE_TERM_RESET "] Engine settings : Unable to find '" DE_TERM_FG_YELLOW << key << DE_TERM_RESET "' parameter.\n";

            return false;
        }
        
        resManager->set_screenshots_folder(element->value2().str());

        key = "export_folder";
        element = fobj.search_element(key);
        if(element == nullptr)
        {
            core::err() << DE_TERM_RESET "[" DE_TERM_FG_RED "ERROR" DE_TERM_RESET "] Engine settings : Unable to find '" DE_TERM_FG_YELLOW << key << DE_TERM_RESET "' parameter.\n";

            return false;
        }

        resManager->set_export_folder(element->value2().str());

        element = fobj.search_element("imgui_font_filename");
        if(element == nullptr)
        {
            fprintf(stderr, "[ERROR] Engine settings: Unable to find 'imgui_font_filename' parameter.\n");

            return false;
        }

        m_ImGuiFontFilename = rvalue_cast(element->value2());

        return true;
    }

}
