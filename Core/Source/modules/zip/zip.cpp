#include "modules/zip/zip.hpp"

#include <zip.h>

namespace deep
{

    zip_int64_t __deep_zip_seek(stream *s, void *data)
    {
        zip_source_args_seek *argsSeek = static_cast<zip_source_args_seek *>(data);
        ref<stream> strm = s;

        stream::seek_origin or;

        if(!strm->can_seek())
        {
            return -1;
        }

        switch(argsSeek->whence)
        {
            default: return -1;
            case SEEK_SET:
            {
                or = stream::seek_origin::Begin;
            } break;
            case SEEK_CUR:
            {
                or = stream::seek_origin::Current;
            } break;
            case SEEK_END:
            {
                or = stream::seek_origin::End;
            } break;
        }

        strm->seek(static_cast<ssize_t>(argsSeek->offset), or);

        return 0;
    }

    zip_int64_t __deep_zip_source_callback(void *userdata, void *data, zip_uint64_t len, zip_source_cmd_t cmd)
    {
        // 'userdata' est un pointeur spécifié par l'utilisateur au moment de la création du zip_source.
        zip::stream_context *ctx = static_cast<zip::stream_context *>(userdata);

        // cmd correspond à la commande envoyée par libzip.
        // Il faut retourner -1 en cas d'erreur, 0 en cas de succès ou autre si la doc le spécifie.
        switch(cmd)
        {
            default: return -1;
            case ZIP_SOURCE_SUPPORTS: return zip_source_make_command_bitmap(ZIP_SOURCE_OPEN, ZIP_SOURCE_CLOSE, ZIP_SOURCE_FREE, ZIP_SOURCE_READ, ZIP_SOURCE_BEGIN_WRITE, ZIP_SOURCE_WRITE, ZIP_SOURCE_COMMIT_WRITE, ZIP_SOURCE_ROLLBACK_WRITE, ZIP_SOURCE_STAT, ZIP_SOURCE_ERROR, ZIP_SOURCE_SEEK_WRITE, ZIP_SOURCE_SEEK, ZIP_SOURCE_TELL_WRITE, ZIP_SOURCE_TELL, ZIP_SOURCE_REMOVE);
            case ZIP_SOURCE_OPEN:
            {
                if(!ctx->original->is_opened())
                {
                    if(!ctx->original->open())
                    {
                        return -1;
                    }
                }
            } return 0;
            case ZIP_SOURCE_CLOSE: return 0;    // Ce n'est pas libzip qui doit s'occuper de la fermeture des streams.
            case ZIP_SOURCE_FREE:  return 0;
            case ZIP_SOURCE_READ:
            {
                // Lit le stream de 'len' octets et les stocks dans 'data'.
                // Doit retourner le nombre d'octets lus ou 0 en cas d'EOF.
                size_t bytesRead = 0;

                if(ctx->original->read(data, 0, static_cast<size_t>(len), &bytesRead))
                {
                    return static_cast<zip_int64_t>(bytesRead);
                }
            } return -1;
            case ZIP_SOURCE_BEGIN_WRITE: return 0;
            case ZIP_SOURCE_WRITE:
            {
                // Écrit dans le stream 'len' octets depuis le buffer 'data'.
                // Retourne le nombre d'octets écris.
                size_t bytesWrite = 0;

                if(ctx->temp->write(data, 0, static_cast<size_t>(len), &bytesWrite))
                {
                    return static_cast<zip_int64_t>(bytesWrite);
                }
            } return -1;
            case ZIP_SOURCE_COMMIT_WRITE:
            {
                size_t bytesWrite = 0;

                ctx->temp->seek(0, stream::seek_origin::Begin);
                ctx->original->seek(0, stream::seek_origin::Begin);

                ctx->original->set_length(0);

                if(ctx->temp->copy_to(*ctx->original.get()))
                {
                    return static_cast<zip_int64_t>(ctx->original->get_length());
                }
            } return -1;
            case ZIP_SOURCE_ROLLBACK_WRITE: return 0;
            case ZIP_SOURCE_STAT:
            {
                zip_stat_t *zipStat = static_cast<zip_stat_t *>(data);

                zip_stat_init(zipStat);

                zipStat->size = static_cast<zip_uint64_t>(ctx->original->get_length());
                zipStat->valid |= ZIP_STAT_SIZE;
            } return sizeof(zip_stat_t);
            case ZIP_SOURCE_ERROR:
            {
                int *errs = static_cast<int *>(data);
                errs[0] = ZIP_ER_OPNOTSUPP;
                errs[1] = EINVAL;
            } return 2 * sizeof(int);
            case ZIP_SOURCE_SEEK_WRITE: return __deep_zip_seek(ctx->temp.get(), data);
            case ZIP_SOURCE_SEEK:       return __deep_zip_seek(ctx->original.get(), data);
            case ZIP_SOURCE_TELL_WRITE: return static_cast<zip_int64_t>(ctx->temp->get_position());
            case ZIP_SOURCE_TELL:       return static_cast<zip_int64_t>(ctx->original->get_position());
            case ZIP_SOURCE_REMOVE:     return 0;
        }
    }

    /*
    ========
    zip::zip
    ========
    */
    zip::zip()
        : m_Zip(nullptr),
          m_ZipSource(nullptr)
    {
        m_Context.original = nullptr;
        m_Context.temp = nullptr;
    }

    /*
    ========
    zip::zip
    ========
    */
    zip::zip(stream *strm)
        : m_Zip(nullptr),
          m_ZipSource(nullptr)
    {
        m_Context.original = strm;
        m_Context.temp = mem::alloc_type<memory_stream>();
    }

    /*
    =========
    zip::~zip
    =========
    */
    zip::~zip()
    {
        zip_source_t *source = static_cast<zip_source_t *>(m_ZipSource);
        zip_t *zip = static_cast<zip_t *>(m_Zip);

        if(zip != nullptr)
        {
            zip_close(zip);
        }
        // zip_close est censé free 'source'.
        else
        {
            if(source != nullptr)
            {
                zip_source_free(source);
            }
        }
    }

    /*
    ==============
    zip::init_base
    ==============
    */
    bool zip::init_base(ref<stream> strm)
    {
        memory_stream *mStrm = mem::alloc_type<memory_stream>();

        if(mStrm == nullptr)
        {
            return false;
        }

        m_Context.original = strm;
        m_Context.temp = mStrm;

        return true;
    }

    /*
    ==============
    zip::enumerate
    ==============
    */
    bool zip::enumerate(enum_entries_callback callback, void *args)
    {
        int64_t entries;
        int64_t index;

        if(callback == nullptr)
        {
            return false;
        }

        entries = get_number_of_entries();

        for(index = 0; index < entries; ++index)
        {
            if(!callback(this, index, get_filename(index), get_file_uncompressed_size(index), get_file_compressed_size(index), get_file_compression_method(index), args))
            {
                break;
            }
        }

        return true;
    }

    /*
    =====================
    zip_reader::read_file
    =====================
    */
    bool zip::read_file(int64_t index, mem_ptr buffer, size_t size)
    {
        zip_t *zip = static_cast<zip_t *>(m_Zip);
        zip_file_t *file;
        zip_int64_t bytesRead;

        file = zip_fopen_index(zip, static_cast<zip_uint64_t>(index), 0);
        if(file == nullptr)
        {
            return false;
        }

        bytesRead = zip_fread(file, buffer, size);

        zip_fclose(file);

        return bytesRead > 0;
    }

    /*
    =====================
    zip_reader::read_file
    =====================
    */
    bool zip::read_file(int64_t index, stream *os)
    {
        ref<stream> strm = os;
        zip_t *zip = static_cast<zip_t *>(m_Zip);
        zip_file_t *file;
        bool ret = true;

        if(!strm->can_write())
        {
            return false;
        }

        if(!strm->is_opened())
        {
            if(!strm->open())
            {
                return false;
            }
        }

        file = zip_fopen_index(zip, static_cast<zip_uint64_t>(index), 0);
        if(file == nullptr)
        {
            return false;
        }

        uint8_t buffer[4096];
        zip_int64_t bytesRead;

        while(true)
        {
            bytesRead = zip_fread(file, buffer, sizeof(buffer));

            if(bytesRead == 0)
            {
                break;
            }

            if(bytesRead == -1)
            {
                ret = false;
                break;
            }

            if(!strm->write(buffer, 0, static_cast<size_t>(bytesRead), nullptr))
            {
                ret = false;
                break;
            }
        }

        zip_fclose(file);

        return ret;
    }

    /*
    =====================
    zip_reader::read_file
    =====================
    */
    bool zip::read_file(int64_t index, ref<text_writer> ow)
    {
        zip_t *zip = static_cast<zip_t *>(m_Zip);
        zip_file_t *file;
        bool ret = true;

        if(!ow->is_opened())
        {
            if(!ow->open())
            {
                return false;
            }
        }

        file = zip_fopen_index(zip, static_cast<zip_uint64_t>(index), 0);
        if(file == nullptr)
        {
            return false;
        }

        uint8_t buffer[4096];
        zip_int64_t bytesRead;

        while(true)
        {
            bytesRead = zip_fread(file, buffer, sizeof(buffer));

            if(bytesRead == 0)
            {
                break;
            }

            if(bytesRead == -1)
            {
                ret = false;
                break;
            }

            if(!ow->write(add_const<char *>(buffer), 0, static_cast<size_t>(bytesRead)))
            {
                ret = false;
                break;
            }
        }

        zip_fclose(file);

        return ret;
    }

    /*
    ==========================
    zip::get_number_of_entries
    ==========================
    */
    int64_t zip::get_number_of_entries() const
    {
        zip_t *zip = static_cast<zip_t *>(m_Zip);

        return static_cast<int64_t>(zip_get_num_entries(zip, 0));
    }

    /*
    ========================
    zip::get_archive_comment
    ========================
    */
    const char *zip::get_archive_comment() const
    {
        zip_t *zip = static_cast<zip_t *>(m_Zip);

        return zip_get_archive_comment(zip, nullptr, 0);
    }

    /*
    ===================
    zip::get_file_index
    ===================
    */
    int64_t zip::get_file_index(const char *filepath) const
    {
        zip_t *zip = static_cast<zip_t *>(m_Zip);

        // Retourne -1 si aucune entrée n'a été trouvée.
        return static_cast<int64_t>(zip_name_locate(zip, filepath, 0));
    }

    /*
    =================
    zip::get_filename
    =================
    */
    const char *zip::get_filename(int64_t index) const
    {
        zip_t *zip = static_cast<zip_t *>(m_Zip);

        return zip_get_name(zip, static_cast<zip_uint64_t>(index), 0);
    }

    /*
    ===============================
    zip::get_file_uncompressed_size
    ===============================
    */
    uint64_t zip::get_file_uncompressed_size(int64_t fileIndex) const
    {
        zip_stat_t st;
        zip_t *source = static_cast<zip_t *>(m_Zip);

        if(zip_stat_index(source, fileIndex, 0, &st) == -1)
        {
            return 0;
        }

        return static_cast<uint64_t>(st.size);
    }

    /*
    =============================
    zip::get_file_compressed_size
    =============================
    */
    uint64_t zip::get_file_compressed_size(int64_t fileIndex) const
    {
        zip_stat_t st;
        zip_t *source = static_cast<zip_t *>(m_Zip);

        if(zip_stat_index(source, fileIndex, 0, &st) == -1)
        {
            return 0;
        }

        return static_cast<uint64_t>(st.comp_size);
    }

    /*
    ================================
    zip::get_file_compression_method
    ================================
    */
    zip_compression_method zip::get_file_compression_method(int64_t fileIndex) const
    {
        zip_stat_t st;
        zip_t *source = static_cast<zip_t *>(m_Zip);

        if(zip_stat_index(source, fileIndex, 0, &st) == -1)
        {
            return zip_compression_method::Unknown;
        }

        switch(st.comp_method)
        {
            default:             return zip_compression_method::Unknown;
            case ZIP_CM_STORE:   return zip_compression_method::Store;
            case ZIP_CM_DEFLATE: return zip_compression_method::Deflate;
            case ZIP_CM_BZIP2:   return zip_compression_method::Bzip2;
            case ZIP_CM_LZMA:    return zip_compression_method::Lzma;
            case ZIP_CM_XZ:      return zip_compression_method::XZ;
            case ZIP_CM_ZSTD:    return zip_compression_method::Zstd;
        }
    }

    /*
    ===============================
    zip::get_compression_method_str
    ===============================
    */
    const char *zip::get_compression_method_str(zip_compression_method method)
    {
        switch(method)
        {
            default: return "unknown";
            case zip_compression_method::Store:   return "store";
            case zip_compression_method::Deflate: return "deflate";
            case zip_compression_method::Bzip2:   return "bzip2";
            case zip_compression_method::Lzma:    return "lzma";
            case zip_compression_method::XZ:      return "XZ";
            case zip_compression_method::Zstd:    return "zstd";
        }
    }

    /*
    ===========================
    zip::create_source_function
    ===========================
    */
    void *zip::create_source_function(stream_context *context)
    {
        zip_error_t err;
        return zip_source_function_create(__deep_zip_source_callback, context, &err);
    }

}
