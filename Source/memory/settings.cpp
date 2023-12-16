#include <DE/memory/settings.hpp>
#include <DE/file/file_object.hpp>
#include <DE/common.hpp>

#include <stdio.h>

namespace deep
{

#if DE_DEBUG
    bool engine_settings::m_DebugMode = true;
#else
    bool engine_settings::m_DebugMode = false;
#endif

    string engine_settings::m_ResourcesDirectory;

    void fobj_load_warning_callback(const char *filename, uint64_t line, char charactere)
    {
        fprintf(stderr, "[WARNING] File Object \"%s\" invalid charactere '%c' at line %llu. Ignoring the line.\n", filename, charactere, line);
    }

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

        auto element = fobj.searchElement("resources_directory");
        if(element == nullptr)
        {
            fprintf(stderr, "[ERROR] Engine settings: Unable to find 'resources_directory' parameter.\n");
            return false;
        }

        m_ResourcesDirectory = rvalue_cast(element->value2());

        return true;
    }

}
