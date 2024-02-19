#ifndef __DEEP_ENGINE_RESOURCES_HPP__
#define __DEEP_ENGINE_RESOURCES_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/string.hpp"
#include "DE/image/bmp.hpp"
#include "DE/drivers/opengl/def.hpp"

namespace deep
{

    class resource_manager
    {

        public:
            DE_API static resource_manager *get_singleton();
            
            DE_API bool init(const char *filepath);
            DE_API void shutdown();

            DE_API GL3::gl_id loadTexture(const char *name, uint8_t unit);
            DE_API bool loadBMP(const char *name, bmp &dest);

            DE_API string getResourcesFolder();
            DE_API string getShadersFolder();
            DE_API string getTexturesFolder();
            DE_API string getSoundsFolder();
            DE_API string getScreenshotsFolder();
            DE_API string get_fonts_folder();

        private:
            resource_manager();

            string m_ResourcesFolder;
            string m_ShadersFolder;
            string m_TexturesFolder;
            string m_SoundsFolder;
            string m_ScreenshotsFolder;
            string m_FontsFolder;

        public:
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

    /*
    ==================================
    resource_manager::get_fonts_folder
    ==================================
    */
    inline string resource_manager::get_fonts_folder()
    {
        return m_FontsFolder;
    }

}


#endif