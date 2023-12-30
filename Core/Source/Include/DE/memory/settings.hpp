#ifndef __DEEP_ENGINE_SETTINGS_HPP__
#define __DEEP_ENGINE_SETTINGS_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/string.hpp>

namespace deep
{

    class engine_settings
    {

        public:
            DE_API static engine_settings *get_singleton();

            DE_API bool init(const char *filepath);

            DE_API bool getDebugMode();
            DE_API string &getResourcesDirectory();

        private:
            engine_settings();

            bool m_DebugMode;
            string m_ResourcesDirectory;

        public:
            engine_settings(const engine_settings &) = delete;

    };

    /*
    =============================
    engine_settings::getDebugMode
    =============================
    */
    inline bool engine_settings::getDebugMode()
    {
        return m_DebugMode;
    }

    /*
    ======================================
    engine_settings::getResourcesDirectory
    ======================================
    */
    inline string &engine_settings::getResourcesDirectory()
    {
        return m_ResourcesDirectory;
    }

}

#endif