#ifndef __DEEP_ENGINE_TEXT_LOADER_HPP__
#define __DEEP_ENGINE_TEXT_LOADER_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/resources/resource_loader.hpp"

namespace deep
{

    class text_loader : public resource_loader
    {

        public:
            DE_API text_loader(zip_writer *zipResource);

            DE_API size_t get_resource_size(const char *name) const override;
            DE_API ref<resource> load_resource(const char *name) const override;

    };

}

#endif