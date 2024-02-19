#ifndef __DEEP_ENGINE_FILE_OBJECT_HPP__
#define __DEEP_ENGINE_FILE_OBJECT_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/string.hpp"
#include "DE/io/stream.hpp"
#include "DE/memory/hash_table.hpp"
#include "DE/memory/pair.hpp"
#include "DE/io/stream.hpp"

namespace deep
{
    struct file_object_container;
    class  file_object;

    using file_object_load_warning_callback = void (*)(const string &name, uint64_t line, char charactere);

    using file_object_enum_element_callback = bool (*)(hash_entry<pair<string, string>> &entry, string &currentPath, mem_ptr args);
    using file_object_enum_container_callback = bool (*)(file_object_container &container, string &currentPath, mem_ptr args);

    struct file_object_container
    {
        string name;
        hash_table<pair<string, string>> items;
        hash_table<file_object_container> containers;

        DE_API file_object_container();
        DE_API file_object_container(const char *key);

        DE_API void enumerate(file_object_enum_element_callback elementCallback, file_object_enum_container_callback containerCallback, mem_ptr args, size_t maxSubContainer = -1, file_object_container **container = nullptr, pair<string, string> **element = nullptr);
        DE_API file_object_container *search_container(const char *path);
        DE_API pair<string, string> *search_element(const char *path);
    };

    /// @brief File Object est un format spécialement créé pour le DeepEngine. La syntax ressemble à celle du JSON.
    class file_object
    {

        public:
            DE_API file_object(stream *inputStream, const char *name);

            /// @brief Charge le File Object en parsant le contenu du flux de données.
            /// @param warningCallback Fonction appelée lorsqu'un avertissement survint pendant le chargement.
            /// @return \c true en cas de succès, \c false dans le cas inverse.
            DE_API bool load(file_object_load_warning_callback warningCallback = nullptr);

            DE_API void enumerate(file_object_enum_element_callback elementCallback, file_object_enum_container_callback containerCallback, mem_ptr args, size_t maxSubContainer = -1, file_object_container **container = nullptr, pair<string, string> **element = nullptr);
            DE_API file_object_container *search_container(const char *path);
            DE_API pair<string, string> *search_element(const char *path);

        private:
            stream *m_InputStream;
            string m_Name;
            file_object_container m_Container;
            uint64_t m_WarningsNumber;

        public:
            file_object() = delete;

    };

    /*
    ======================
    file_object::enumerate
    ======================
    */
    inline void file_object::enumerate(file_object_enum_element_callback elementCallback, file_object_enum_container_callback containerCallback, mem_ptr args, size_t maxSubContainer, file_object_container **container, pair<string, string> **element)
    {
        m_Container.enumerate(elementCallback, containerCallback, args, maxSubContainer, container, element);
    }
    
    /*
    ============================
    file_object::search_container
    ============================
    */
    inline file_object_container *file_object::search_container(const char *path)
    {
        return m_Container.search_container(path);
    }

    /*
    ==========================
    file_object::search_element
    ==========================
    */
    inline pair<string, string> *file_object::search_element(const char *path)
    {
        return m_Container.search_element(path);
    }

}


#endif