#ifndef __DEEP_ENGINE_RESOURCE_MANAGER_HPP__
#define __DEEP_ENGINE_RESOURCE_MANAGER_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/resources/resource.hpp"
#include "core/resources/resource_loader.hpp"
#include "core/string/string.hpp"
#include "io/stream.hpp"
#include "modules/zip/zip_writer.hpp"
#include "file/file_object.hpp"
#include "core/templates/hash_table.hpp"

namespace deep
{

    class resource_manager
    {

        public:
            DE_API static resource_manager *get_singleton();
            
            DE_API bool init(ref<stream> resourcesStream);
            DE_API void shutdown();

            DE_API ref<resource> get_resource(const char *name);

            DE_API string get_screenshots_folder();
            DE_API string get_export_folder();

            DE_API void set_screenshots_folder(const char *value);
            DE_API void set_export_folder(const char *value);

            DE_API void add_loader(const char *type, ref<resource_loader> loader);

        private:
            resource_manager();

            zip_writer m_ZipResource;
            file_object m_FileListing;

            string m_ScreenshotsFolder;
            string m_ExportFolder;

            hash_table<ref<resource_loader>> m_Loaders;
            hash_table<ref<resource>> m_Resources;

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
        if(g_ResourceManager == nullptr)
        {
            g_ResourceManager = mem::alloc_type<resource_manager>();
        }

        return g_ResourceManager;
    }

    /*
    ========================================
    resource_manager::get_screenshots_folder
    ========================================
    */
    inline string resource_manager::get_screenshots_folder()
    {
        return m_ScreenshotsFolder;
    }

    /*
    ===================================
    resource_manager::get_export_folder
    ===================================
    */
    inline string resource_manager::get_export_folder()
    {
        return m_ExportFolder;
    }

    /*
    ========================================
    resource_manager::set_screenshots_folder
    ========================================
    */
    inline void resource_manager::set_screenshots_folder(const char *value)
    {
        m_ScreenshotsFolder = value;
    }

    /*
    ===================================
    resource_manager::set_export_folder
    ===================================
    */
    inline void resource_manager::set_export_folder(const char *value)
    {
        m_ExportFolder = value;
    }

}

#endif