#include "core/resources/text_loader.hpp"
#include "core/resources/text.hpp"
#include "io/string_writer.hpp"

namespace deep
{

    /*
    ========================
    text_loader::text_loader
    ========================
    */
    text_loader::text_loader(zip_writer *zipResource)
        : resource_loader(zipResource)
    { }

    /*
    ==============================
    text_loader::get_resource_size
    ==============================
    */
    size_t text_loader::get_resource_size(const char *name) const
    {
        if(m_ZipResource == nullptr)
        {
            return 0;
        }

        int64_t index = m_ZipResource->get_file_index(name);

        if(index == -1)
        {
            return 0;
        }

        return static_cast<size_t>(m_ZipResource->get_file_uncompressed_size(index));
    }

    /*
    ==========================
    text_loader::load_resource
    ==========================
    */
    ref<resource> text_loader::load_resource(const char *name) const
    {
        int64_t index = m_ZipResource->get_file_index(name);

        if(index == -1)
        {
            return ref<resource>();
        }

        ref<string_writer> os = mem::alloc_type<string_writer>();

        if(!m_ZipResource->read_file(index, os.get()))
        {
            return ref<resource>();
        }

        text *txt = mem::alloc_type<text>();
        txt->m_Content = rvalue_cast(os->get_content());

        return txt;
    }

}
