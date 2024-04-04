#ifndef __DEEP_ENGINE_ZIP_HPP__
#define __DEEP_ENGINE_ZIP_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/string/string.hpp"
#include "io/stream.hpp"
#include "io/memory_stream.hpp"
#include "io/text_writer.hpp"
#include "modules/zip/zip_types.hpp"

namespace deep
{

    class zip
    {

        public:
            using enum_entries_callback = bool (*)(zip *archive, int64_t index, const char *filename, uint64_t uncompressedSize, uint64_t compressedSize, zip_compression_method compressionMethod, void *args);

            struct stream_context
            {
                ref<stream> original;
                ref<memory_stream> temp;
            };

        public:
            DE_API zip();
            DE_API zip(stream *strm);
            DE_API virtual ~zip();

            virtual bool init() = 0;

            DE_API bool init_base(ref<stream> strm);

            DE_API bool enumerate(enum_entries_callback callback, void *args);

            DE_API bool read_file(int64_t index, mem_ptr buffer, size_t size);
            DE_API bool read_file(int64_t index, stream *os);
            DE_API bool read_file(int64_t index, ref<text_writer> ow);

            DE_API int64_t get_number_of_entries() const;
            DE_API const char *get_archive_comment() const;

            DE_API int64_t get_file_index(const char *filepath) const;
            DE_API const char *get_filename(int64_t index) const;
            DE_API uint64_t get_file_uncompressed_size(int64_t fileIndex) const;
            DE_API uint64_t get_file_compressed_size(int64_t fileIndex) const;
            DE_API zip_compression_method get_file_compression_method(int64_t fileIndex) const;

            DE_API bool is_directory(const char *path) const;
            DE_API bool is_directory(int64_t index) const;

            DE_API static const char *get_compression_method_str(zip_compression_method method);

        protected:
            stream_context m_Context;
            void *m_Zip;
            void *m_ZipSource;

        protected:
            DE_API static void *create_source_function(stream_context *context);

    };

    /*
    =================
    zip::is_directory
    =================
    */
    inline bool zip::is_directory(const char *path) const
    {
        // Dans libzip, chaque dossier fini par un slash, et uniquement les dossiers.
        return string_utils::ends_with(path, "/");
    }

    /*
    =================
    zip::is_directory
    =================
    */
    inline bool zip::is_directory(int64_t index) const
    {
        return is_directory(get_filename(index));
    }

}

#endif