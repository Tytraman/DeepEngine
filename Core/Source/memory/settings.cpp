#include <DE/memory/settings.hpp>
#include <DE/file/file_object.hpp>
#include <DE/common.hpp>

#include <stdio.h>

namespace deep
{

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

    /*
    ==============================
    engine_settings::get_singleton
    ==============================
    */
    engine_settings *engine_settings::get_singleton()
    {
        static engine_settings singleton;

        return &singleton;
    }

    void fobj_load_warning_callback(const char *filename, uint64_t line, char charactere)
    {
        fprintf(stderr, "[WARNING] File Object \"%s\" invalid charactere '%c' at line %llu. Ignoring the line.\n", filename, charactere, line);
    }

    /*
    =====================
    engine_settings::init
    =====================
    */
    bool engine_settings::init(const char *filepath)
    {
        file_object fobj(filepath);

        if(!fobj.open())
            return false;

        if(!fobj.load(fobj_load_warning_callback))
        {
            fobj.close();
            return false;
        }

        fobj.close();

        pair<string, string> *element = fobj.searchElement("resources_directory");
        if(element == nullptr)
        {
            fprintf(stderr, "[ERROR] Engine settings: Unable to find 'resources_directory' parameter.\n");
            return false;
        }

        m_ResourcesDirectory = rvalue_cast(element->value2());

        element = fobj.searchElement("imgui_font_filename");
        if(element == nullptr)
        {
            fprintf(stderr, "[ERROR] Engine settings: Unable to find 'imgui_font_filename' parameter.\n");
            return false;
        }

        m_ImGuiFontFilename = rvalue_cast(element->value2());

        return true;
    }

}
