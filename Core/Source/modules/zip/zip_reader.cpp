#include "DE/modules/zip/zip_reader.hpp"
#include "DE/core/core.hpp"

#include <minizip-ng/mz.h>
#include <minizip-ng/mz_os.h>
#include <minizip-ng/mz_strm.h>
#include <minizip-ng/mz_zip.h>
#include <minizip-ng/mz_zip_rw.h>

namespace deep
{

    /*
    ======================
    zip_reader::zip_reader
    ======================
    */
    zip_reader::zip_reader()
        : m_RawData(nullptr),
          m_ZipReader(nullptr)
    { }

    /*
    =======================
    zip_reader::~zip_reader
    =======================
    */
    zip_reader::~zip_reader()
    {
        if(m_ZipReader != nullptr)
        {
            mz_zip_reader_delete(&m_ZipReader);
        }

        if(m_RawData != nullptr)
        {
            mem::free(m_RawData);
        }
    }

    /*
    ================
    zip_reader::load
    ================
    */
    bool zip_reader::load(stream *inputStream)
    {
        ref<stream> s(inputStream);

        if(!s->can_read())
        {
            return false;
        }

        if(!s->is_opened())
        {
            if(!s->open())
            {
                return false;
            }
        }

        size_t len = s->get_length() - s->get_position();
        if(len == 0)
        {
            return false;
        }

        mem_ptr rawData = static_cast<uint8_t *>(mem::alloc(len));
        size_t bytesRead = 0;

        if(!s->read(rawData, 0, len, &bytesRead))
        {
            mem::free(rawData);

            return false;
        }

        if(!load(rawData, static_cast<int32_t>(bytesRead)))
        {
            mem::free(rawData);

            return false;
        }

        return true;
    }

    /*
    ================
    zip_reader::load
    ================
    */
    bool zip_reader::load(mem_ptr rawData, int32_t size)
    {
        int32_t err;

        m_ZipReader = mz_zip_reader_create();
        if(m_ZipReader == nullptr)
        {
            return false;
        }

        // Le 0 à la fin indique qu'on ne copie pas le buffer en interne mais qu'on utilise celui passé en paramètre.
        err = mz_zip_reader_open_buffer(m_ZipReader, static_cast<uint8_t *>(rawData), size, 0);
        if(err != MZ_OK)
        {
            mz_zip_reader_delete(&m_ZipReader);

            return false;
        }

        m_RawData = rawData;

        return true;
    }

    /*
    =====================
    zip_reader::enumerate
    =====================
    */
    bool zip_reader::enumerate(next_entry_callback callback, void *args)
    {
        if(callback == nullptr)
        {
            return true;
        }

        int32_t err = mz_zip_reader_goto_first_entry(m_ZipReader);
        mz_zip_file *fileInfo = nullptr;
        compression_method method;

        if(err != MZ_OK && err != MZ_END_OF_LIST)
        {
            return false;
        }

        while(true)
        {
            err = mz_zip_reader_entry_get_info(m_ZipReader, &fileInfo);

            if(err != MZ_OK)
            {
                return false;
            }

            switch(fileInfo->compression_method)
            {
                default:
                {
                    method = compression_method::Unknown;
                } break;
                case MZ_COMPRESS_METHOD_STORE:
                {
                    method = compression_method::Store;
                } break;
                case MZ_COMPRESS_METHOD_DEFLATE:
                {
                    method = compression_method::Deflate;
                } break;
                case MZ_COMPRESS_METHOD_BZIP2:
                {
                    method = compression_method::Bzip2;
                } break;
                case MZ_COMPRESS_METHOD_LZMA:
                {
                    method = compression_method::Lzma;
                } break;
                case MZ_COMPRESS_METHOD_XZ:
                {
                    method = compression_method::XZ;
                } break;
                case MZ_COMPRESS_METHOD_ZSTD:
                {
                    method = compression_method::Zstd;
                } break;
            }

            underlying_type<file_attribute> attributes = 0;

            if(fileInfo->external_fa & 0x1) // Read-only
            {
                attributes |= to_utype(file_attribute::ReadOnly);
            }

            if(fileInfo->external_fa & 0x2) // Hidden
            {
                attributes |= to_utype(file_attribute::Hidden);
            }

            if(fileInfo->external_fa & 0x4) // System
            {
                attributes |= to_utype(file_attribute::System);
            }

            if(fileInfo->external_fa & 0x10) // Directory
            {
                attributes |= to_utype(file_attribute::Directory);
            }

            if(fileInfo->external_fa & 0x20) // Archive
            {
                attributes |= to_utype(file_attribute::Archive);
            }

            if(fileInfo->external_fa & 0x100) // Temporary
            {
                attributes |= to_utype(file_attribute::Temporary);
            }

            if(fileInfo->external_fa & 0x200) // Sparse file
            {
                attributes |= to_utype(file_attribute::SparseFile);
            }

            if(fileInfo->external_fa & 0x800) // Compressed
            {
                attributes |= to_utype(file_attribute::Compressed);
            }

            if(fileInfo->external_fa & 0x1000) // Offline
            {
                attributes |= to_utype(file_attribute::Offline);
            }

            if(fileInfo->external_fa & 0x2000) // Not content indexed
            {
                attributes |= to_utype(file_attribute::NotContentIndexed);
            }

            if(fileInfo->external_fa & 0x4000) // Encrypted
            {
                attributes |= to_utype(file_attribute::Encrypted);
            }

            if(!callback(fileInfo->filename, fileInfo->compressed_size, fileInfo->uncompressed_size, method, static_cast<file_attribute>(attributes), args))
            {
                break;
            }

            err = mz_zip_reader_goto_next_entry(m_ZipReader);

            if(err == MZ_END_OF_LIST)
            {
                break;
            }

            if(err != MZ_OK)
            {
                return false;
            }
        }

        return true;
    }

    struct __deep_search_zip_file
    {
        const char *filename;
        bool founded;
    };

    bool __deep_search_file_callback(const char *filename, int64_t /*compressedSize*/, int64_t /*uncompressedSize*/, zip_reader::compression_method /*method*/, zip_reader::file_attribute /*attributes*/, void *args)
    {
        __deep_search_zip_file *search = static_cast<__deep_search_zip_file *>(args);

        if(string_utils::equals(filename, search->filename))
        {
            search->founded = true;

            return false;
        }

        return true;
    }


    /*
    ========================
    zip_reader::extract_file
    ========================
    */
    bool zip_reader::extract_file(const char *filename, stream *outputStream)
    {
        if(!outputStream->can_write())
        {
            return false;
        }

        __deep_search_zip_file search;
        search.filename = filename;
        search.founded = false;

        if(!enumerate(__deep_search_file_callback, &search) || !search.founded)
        {
            return false;
        }

        int32_t len = mz_zip_reader_entry_save_buffer_length(m_ZipReader);
        mem_ptr buffer = mem::alloc(len);

        int32_t err = mz_zip_reader_entry_save_buffer(m_ZipReader, buffer, len);
        if(err != MZ_OK)
        {
            mem::free(buffer);

            return false;
        }

        if(!outputStream->is_opened())
        {
            if(!outputStream->open())
            {
                mem::free(buffer);

                return false;
            }
        }

        bool ok = outputStream->write(buffer, 0, len, nullptr);

        mem::free(buffer);

        return ok;
    }

    /*
    ==========================
    zip_reader::attributes_str
    ==========================
    */
    string zip_reader::attributes_str(file_attribute attributes)
    {
        string str;

        underlying_type<file_attribute> attr = to_utype(attributes);

        if((attr & to_utype(file_attribute::ReadOnly)) > 0)
        {
            str.append("R");
        }

        if((attr & to_utype(file_attribute::Hidden)) > 0)
        {
            str.append("H");
        }

        if((attr & to_utype(file_attribute::System)) > 0)
        {
            str.append("S");
        }

        if((attr & to_utype(file_attribute::Directory)) > 0)
        {
            str.append("D");
        }

        if((attr & to_utype(file_attribute::Archive)) > 0)
        {
            str.append("A");
        }

        if((attr & to_utype(file_attribute::Compressed)) > 0)
        {
            str.append("C");
        }

        if((attr & to_utype(file_attribute::Encrypted)) > 0)
        {
            str.append("E");
        }

        if((attr & to_utype(file_attribute::Offline)) > 0)
        {
            str.append("O");
        }

        if((attr & to_utype(file_attribute::SparseFile)) > 0)
        {
            str.append("P");
        }

        if((attr & to_utype(file_attribute::NotContentIndexed)) > 0)
        {
            str.append("I");
        }

        if((attr & to_utype(file_attribute::Temporary)) > 0)
        {
            str.append("T");
        }

        return str;
    }

    string zip_reader::method_str(compression_method method)
    {
        switch(method)
        {
            default: return string();
            case compression_method::Unknown: return "unknown";
            case compression_method::Store:   return "store";
            case compression_method::Deflate: return "deflate";
            case compression_method::Bzip2:   return "bzip2";
            case compression_method::Lzma:    return "lzma";
            case compression_method::XZ:      return "xz";
            case compression_method::Zstd:    return "zstd";
        }
    }

}
