#ifndef __DEEP_ENGINE_FOBJ_HPP__
#define __DEEP_ENGINE_FOBJ_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/resources/resource.hpp"
#include "file/file_object.hpp"

namespace deep
{

    class fobj : public resource
    {

        public:
            DE_API fobj(const char *name);

            DE_API mem_ptr get_resource_data() const override;
            DE_API size_t get_resource_size() const override;

        protected:
            file_object m_Fobj;

    };

}

#endif