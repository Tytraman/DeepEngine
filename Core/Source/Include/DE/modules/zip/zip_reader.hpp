#ifndef __DEEP_ENGINE_ZIP_READER_HPP__
#define __DEEP_ENGINE_ZIP_READER_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/io/stream.hpp"
#include "DE/core/string.hpp"
#include "DE/modules/zip/zip_types.hpp"

namespace deep
{

    class zip_reader
    {

        public:
            DE_API zip_reader(stream *inputStream);

            DE_API bool init();

            DE_API int64_t get_number_of_entries() const;
            DE_API const char *get_archive_comment() const;

        private:
            ref<stream> m_InputStream;
            void *m_Zip;
            void *m_ZipSource;

    };

}

#endif