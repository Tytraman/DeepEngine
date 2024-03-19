#include "DE/modules/zip/zip_reader.hpp"
#include "DE/core/core.hpp"
#include "DE/core/hash_table.hpp"

#include <zip.h>

namespace deep
{

    zip_int64_t __deep_zip_source_callback(void *userdata, void *data, zip_uint64_t len, zip_source_cmd_t cmd)
    {
        // 'userdata' est un pointeur spécifié par l'utilisateur au moment de la création du zip_source.
        ref<stream> strm = static_cast<stream *>(userdata);

        // cmd correspond à la commande envoyée par libzip.
        // Il faut retourner -1 en cas d'erreur, 0 en cas de succès ou autre si la doc le spécifie.
        switch(cmd)
        {
            default: return -1;
            case ZIP_SOURCE_SUPPORTS: return zip_source_make_command_bitmap(ZIP_SOURCE_OPEN, ZIP_SOURCE_CLOSE, ZIP_SOURCE_FREE, ZIP_SOURCE_READ, ZIP_SOURCE_STAT, ZIP_SOURCE_ERROR, ZIP_SOURCE_SEEK, ZIP_SOURCE_TELL);
            case ZIP_SOURCE_OPEN:
            {
                if(!strm->is_opened())
                {
                    if(!strm->open())
                    {
                        return -1;
                    }
                }
            } return 0;
            case ZIP_SOURCE_CLOSE: return 0;    // Ce n'est pas libzip qui doit s'occuper de la fermeture des streams.
            case ZIP_SOURCE_FREE: return 0;
            case ZIP_SOURCE_READ:
            {
                // Lit le stream de 'len' octets et les stocks dans 'data'.
                // Doit retourner le nombre d'octets lus ou 0 en cas d'EOF.
                size_t bytesRead = 0;

                if(strm->read(data, 0, static_cast<size_t>(len), &bytesRead))
                {
                    return static_cast<zip_int64_t>(bytesRead);
                }
            } return -1;
            case ZIP_SOURCE_STAT:
            {
                zip_stat_t *zipStat = static_cast<zip_stat_t *>(data);

                zip_stat_init(zipStat);

                zipStat->size = static_cast<zip_uint64_t>(strm->get_length());
                zipStat->valid |= ZIP_STAT_SIZE;
            } return sizeof(zip_stat_t);
            case ZIP_SOURCE_ERROR:
            {
                int *errs = static_cast<int *>(data);
                errs[0] = ZIP_ER_OPNOTSUPP;
                errs[1] = EINVAL;
            } return 2 * sizeof(int);
            case ZIP_SOURCE_SEEK:
            {
                zip_source_args_seek *argsSeek = static_cast<zip_source_args_seek *>(data);

                stream::seek_origin or;

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

                if(!strm->seek(static_cast<ssize_t>(argsSeek->offset), or))
                {
                    return -1;
                }
            } return 0;
            case ZIP_SOURCE_TELL: return static_cast<zip_int64_t>(strm->get_position());
        }
    }

    /*
    ======================
    zip_reader::zip_reader
    ======================
    */
    zip_reader::zip_reader(stream *inputStream)
        : m_InputStream(inputStream),
          m_Zip(nullptr),
          m_ZipSource(nullptr)
    {  }

    /*
    ================
    zip_reader::init
    ================
    */
    bool zip_reader::init()
    {
        zip_error_t err;
        zip_t *zip;
        zip_source_t *source = zip_source_function_create(__deep_zip_source_callback, m_InputStream.get(), &err);

        if(source == nullptr)
        {
            return false;
        }

        zip = zip_open_from_source(source, ZIP_RDONLY, &err);

        if(zip == nullptr)
        {
            zip_source_close(source);
            zip_source_free(source);

            return false;
        }

        m_Zip = zip;
        m_ZipSource = source;

        return true;
    }

    /*
    =================================
    zip_reader::get_number_of_entries
    =================================
    */
    int64_t zip_reader::get_number_of_entries() const
    {
        zip_t *zip = static_cast<zip_t *>(m_Zip);

        return static_cast<int64_t>(zip_get_num_entries(zip, 0));
    }

    /*
    ===============================
    zip_reader::get_archive_comment
    ===============================
    */
    const char *zip_reader::get_archive_comment() const
    {
        zip_t *zip = static_cast<zip_t *>(m_Zip);

        return zip_get_archive_comment(zip, nullptr, 0);
    }

}
