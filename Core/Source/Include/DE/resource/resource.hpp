#ifndef __DEEP_ENGINE_CORE_HPP__
#define __DEEP_ENGINE_CORE_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/list.hpp>

namespace deep
{

    class iresource;
    class resource_cache;

    using resource_loader_function = iresource (&)();

    class DE_API iresource
    {

        public:
            

    };

    class DE_API resource_cache
    {



    };

}

#endif