#include "DE/core/settings.hpp"
#include "DE/file/file_object.hpp"
#include "DE/core/common.hpp"

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

        pair<string, string> *element = fobj.search_element("resources_directory");
        if(element == nullptr)
        {
            fprintf(stderr, "[ERROR] Engine settings: Unable to find 'resources_directory' parameter.\n");

            return false;
        }

        m_ResourcesDirectory = rvalue_cast(element->value2());

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
