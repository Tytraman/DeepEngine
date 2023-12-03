#ifndef __DEEP_ENGINE_RESOURCES_HPP__
#define __DEEP_ENGINE_RESOURCES_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/rendering/opengl_utils.hpp>
#include <DE/string.hpp>

namespace de
{

    class DE_API resource_manager
    {

        public:
            static bool init(const char *filepath);
            static void shutdown();

            static texture_id loadTexture(const char *name, uint8_t unit);

            static string getResourcesFolder();
            static string getShadersFolder();
            static string getTexturesFolder();
            static string getSoundsFolder();
            static string getScreenshotsFolder();

        private:
            static string m_ResourcesFolder;
            static string m_ShadersFolder;
            static string m_TexturesFolder;
            static string m_SoundsFolder;
            static string m_ScreenshotsFolder;

        public:
            resource_manager() = delete;
            resource_manager(const resource_manager &) = delete;
            resource_manager(resource_manager &&) = delete;

    };

    /*
    ====================================
    resource_manager::getResourcesFolder
    ====================================
    */
    inline string resource_manager::getResourcesFolder()
    {
        return m_ResourcesFolder;
    }

    /*
    ==================================
    resource_manager::getShadersFolder
    ==================================
    */
    inline string resource_manager::getShadersFolder()
    {
        return m_ShadersFolder;
    }

    /*
    ===================================
    resource_manager::getTexturesFolder
    ===================================
    */
    inline string resource_manager::getTexturesFolder()
    {
        return m_TexturesFolder;
    }

    /*
    =================================
    resource_manager::getSoundsFolder
    =================================
    */
    inline string resource_manager::getSoundsFolder()
    {
        return m_SoundsFolder;
    }

    /*
    ======================================
    resource_manager::getScreenshotsFolder
    ======================================
    */
    inline string resource_manager::getScreenshotsFolder()
    {
        return m_ScreenshotsFolder;
    }

}


#endif