#ifndef __DEEP_ENGINE_RESOURCE_LOADER_HPP__
#define __DEEP_ENGINE_RESOURCE_LOADER_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/ref_counted.hpp"
#include "core/resources/resource.hpp"
#include "modules/zip/zip_writer.hpp"

namespace deep
{

    class resource_loader : public ref_counted
    {

        public:
            DE_API resource_loader(zip_writer *zipResource);

            virtual size_t get_resource_size(const char *name) const = 0;
            virtual ref<resource> load_resource(const char *name) const = 0;

        protected:
            zip_writer *m_ZipResource;

    };

}

#endif