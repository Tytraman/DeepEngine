#include "core/resources/fobj.hpp"

namespace deep
{

    /*
    ==========
    fobj::fobj
    ==========
    */
    fobj::fobj(const char *name)
        : resource(),
          m_Fobj(name)
    { }

    /*
    =======================
    fobj::get_resource_data
    =======================
    */
    mem_ptr fobj::get_resource_data() const
    {
        return nullptr;
    }

    /*
    =======================
    fobj::get_resource_size
    =======================
    */
    size_t fobj::get_resource_size() const
    {
        return 0;
    }

}
