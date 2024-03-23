#ifndef __DEEP_ENGINE_FILE_HPP__
#define __DEEP_ENGINE_FILE_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/list.hpp"

namespace deep
{

    using file_enum_callback_ascii = bool (*)(const char *filename, bool isDirectory, void *args);
    using file_enum_callback_wide = bool (*)(wchar_t *filename, bool isDirectory, void *args);

    class DE_API file
    {

        public:
            static bool enumFiles(const char *path, file_enum_callback_ascii callback, void *args);
            static bool enumFiles(wchar_t *path, file_enum_callback_wide callback, void *args);
            static void printAllFiles(wchar_t *path);
            static void getAllFiles(wchar_t *path, list<wchar_t *> &dest);
            static void getAllFilesEndingWith(wchar_t *path, const wchar_t *end, list<wchar_t *> &dest);

        public:
            file() = delete;

    };

}

#endif