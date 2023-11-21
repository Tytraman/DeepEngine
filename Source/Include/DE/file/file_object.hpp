#ifndef __DEEP_ENGINE_FILE_OBJECT_HPP__
#define __DEEP_ENGINE_FILE_OBJECT_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/string.hpp>
#include <DE/stream.hpp>
#include <DE/memory/hash_table.hpp>
#include <DE/memory/pair.hpp>

namespace de
{
    struct file_object_container;
    class  file_object;

    using file_object_load_warning_callback = void (*)(const char *filename, uint64_t line, char charactere);

    using file_object_enum_element_callback = bool (*)(hash_entry<pair<string, string>> &entry, string &currentPath, mem_ptr args);
    using file_object_enum_container_callback = bool (*)(file_object_container &container, string &currentPath, mem_ptr args);

    struct DE_API file_object_container
    {
        string name;
        hash_table<pair<string, string>> items;
        hash_table<file_object_container> containers;

        file_object_container();
        file_object_container(const char *key);

        void enumerate(file_object_enum_element_callback elementCallback, file_object_enum_container_callback containerCallback, mem_ptr args, file_object_container **container = nullptr, pair<string, string> **element = nullptr);
        file_object_container *searchContainer(const char *path);
        pair<string, string> *searchElement(const char *path);
    };

    class DE_API file_object : public input_file_stream
    {

        public:
            file_object(const char *filename);
            bool load(file_object_load_warning_callback warningCallback = nullptr);

            void enumerate(file_object_enum_element_callback elementCallback, file_object_enum_container_callback containerCallback, mem_ptr args, file_object_container **container = nullptr, pair<string, string> **element = nullptr);
            file_object_container *searchContainer(const char *path);
            pair<string, string> *searchElement(const char *path);

        private:
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
    inline void file_object::enumerate(file_object_enum_element_callback elementCallback, file_object_enum_container_callback containerCallback, mem_ptr args, file_object_container **container, pair<string, string> **element)
    {
        m_Container.enumerate(elementCallback, containerCallback, args, container, element);
    }
    
    /*
    ============================
    file_object::searchContainer
    ============================
    */
    inline file_object_container *file_object::searchContainer(const char *path)
    {
        return m_Container.searchContainer(path);
    }

    /*
    ==========================
    file_object::searchElement
    ==========================
    */
    inline pair<string, string> *file_object::searchElement(const char *path)
    {
        return m_Container.searchElement(path);
    }

}


#endif