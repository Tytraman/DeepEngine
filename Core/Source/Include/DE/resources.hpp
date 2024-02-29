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
            DE_API static void set_singleton(resource_manager *ptr);
            
            DE_API bool init(const char *filepath);
            DE_API void shutdown();

            DE_API GL3::gl_id load_texture(const char *name, uint8_t unit);
            DE_API bool load_bmp(const char *name, bmp &dest);

            DE_API string get_resources_folder();
            DE_API string get_shaders_folder();
            DE_API string get_textures_folder();
            DE_API string get_sounds_folder();
            DE_API string get_screenshots_folder();
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

            friend memory_manager;

        private:
            static resource_manager *g_ResourceManager;

    };

    /*
    ===============================
    resource_manager::get_singleton
    ===============================
    */
    inline resource_manager *resource_manager::get_singleton()
    {
        return g_ResourceManager;
    }

    /*
    ===============================
    resource_manager::set_singleton
    ===============================
    */
    inline void resource_manager::set_singleton(resource_manager *ptr)
    {
        g_ResourceManager = ptr;
    }

    /*
    ====================================
    resource_manager::get_resources_folder
    ====================================
    */
    inline string resource_manager::get_resources_folder()
    {
        return m_ResourcesFolder;
    }

    /*
    ==================================
    resource_manager::get_shaders_folder
    ==================================
    */
    inline string resource_manager::get_shaders_folder()
    {
        return m_ShadersFolder;
    }

    /*
    ===================================
    resource_manager::get_textures_folder
    ===================================
    */
    inline string resource_manager::get_textures_folder()
    {
        return m_TexturesFolder;
    }

    /*
    =================================
    resource_manager::get_sounds_folder
    =================================
    */
    inline string resource_manager::get_sounds_folder()
    {
        return m_SoundsFolder;
    }

    /*
    ======================================
    resource_manager::get_screenshots_folder
    ======================================
    */
    inline string resource_manager::get_screenshots_folder()
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