#ifndef __DEEP_ENGINE_TEXT_HPP__
#define __DEEP_ENGINE_TEXT_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/resources/resource.hpp"
#include "core/string/string.hpp"

namespace deep
{

    class text_loader;

    class text : public resource
    {

        public:
            DE_API text();
            DE_API text(const char *value);

            DE_API mem_ptr get_resource_data() const override;
            DE_API size_t get_resource_size() const override;

        protected:
            string m_Content;

        public:
            friend text_loader;

    };

    /*
    =======================
    text::get_resource_data
    =======================
    */
    inline mem_ptr text::get_resource_data() const
    {
        return static_cast<mem_ptr>(rm_const<char *>(m_Content.str()));
    }

    /*
    =======================
    text::get_resource_size
    =======================
    */
    inline size_t text::get_resource_size() const
    {
        return m_Content.get_length();
    }

}

#endif