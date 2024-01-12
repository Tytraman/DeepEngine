#ifndef __DEEP_ENGINE_GRAPHIC_HPP__
#define __DEEP_ENGINE_GRAPHIC_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/vec.hpp"
#include "DE/memory/hash_table.hpp"
#include "DE/string.hpp"

namespace deep
{

    class material_manager
    {

        public:
            struct material_item
            {
                string name;
                fvec3 ambient;
                fvec3 diffuse;
                fvec3 specular;
                float shininess;

                DE_API material_item(const char *name, const fvec3 &ambient, const fvec3 &diffuse, const fvec3 &specular, float shininess);
            };

        public:
            DE_API static material_manager *get_singleton();

            DE_API de_id create(const char *name, const fvec3 &ambient, const fvec3 &diffuse, const fvec3 &specular, float shininess);

            DE_API material_item *get(de_id materialID);

        private:
            material_manager();

            hash_table<material_item> m_Materials;

        public:
            material_manager(const material_manager &) = delete;
            material_manager(material_manager &&) = delete;

    };

    /*
    ========================
    material_manager::create
    ========================
    */
    inline de_id material_manager::create(const char *name, const fvec3 &ambient, const fvec3 &diffuse, const fvec3 &specular, float shininess)
    {
        hash_entry<material_item> &h = m_Materials.insert(name, material_item(name, ambient, diffuse, specular, shininess));
        return h.key;
    }

    /*
    =====================
    material_manager::get
    =====================
    */
    inline material_manager::material_item *material_manager::get(de_id materialID)
    {
        hash_entry<material_item> *hs = m_Materials[materialID];
        if(hs == nullptr)
            return nullptr;

        return &hs->value;
    }

}

#endif