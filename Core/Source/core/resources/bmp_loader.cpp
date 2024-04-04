#include "core/resources/bmp_loader.hpp"
#include "io/memory_stream.hpp"
#include "image/bmp.hpp"

namespace deep
{
    
    /*
    ======================
    bmp_loader::bmp_loader
    =======================
    */
    bmp_loader::bmp_loader(zip_writer *zipResource)
        : resource_loader(zipResource)
    { }

    /*
    =============================
    bmp_loader::get_resource_size
    =============================
    */
    size_t bmp_loader::get_resource_size(const char *name) const
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
    =========================
    bmp_loader::load_resource
    =========================
    */
    ref<resource> bmp_loader::load_resource(const char *name) const
    {
        int64_t index = m_ZipResource->get_file_index(name);

        if(index == -1)
        {
            return ref<resource>();
        }

        ref<memory_stream> os = mem::alloc_type<memory_stream>();

        if(!m_ZipResource->read_file(index, os.get()))
        {
            return ref<resource>();
        }

        os->seek(0, stream::seek_origin::Begin);

        image *img = mem::alloc_type<image>();

        if(!bmp::create_from_stream(os.get(), *img))
        {
            mem::free_type(img);

            return ref<resource>();
        }

        return img;
    }

}
