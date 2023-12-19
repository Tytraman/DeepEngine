#ifndef __DEEP_ENGINE_SETTINGS_HPP__
#define __DEEP_ENGINE_SETTINGS_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/string.hpp>

namespace deep
{

    class engine_settings;



    class DE_API engine_settings
    {

        public:
            static bool init(const char *filepath);

            static bool getDebugMode();
            static string &getResourcesDirectory();

        private:
            static bool m_DebugMode;
            static string m_ResourcesDirectory;

        public:
            engine_settings() = delete;

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