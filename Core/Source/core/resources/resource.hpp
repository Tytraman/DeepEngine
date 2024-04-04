#ifndef __DEEP_ENGINE_RESOURCE_HPP__
#define __DEEP_ENGINE_RESOURCE_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/ref_counted.hpp"

namespace deep
{

    class resource : public ref_counted
    {

        public:
            virtual mem_ptr get_resource_data() const = 0;
            virtual size_t get_resource_size() const = 0;

    };

}

#endif