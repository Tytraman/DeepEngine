#include "DE/graphics/material.hpp"

namespace deep
{

    /*
    ==============================================
    material_manager::material_item::material_item
    ==============================================
    */
    material_manager::material_item::material_item(const char *_name, const fvec3 &_ambient, const fvec3 &_diffuse, const fvec3 &_specular, float _shininess)
        : name(_name),
          ambient(_ambient),
          diffuse(_diffuse),
          specular(_specular),
          shininess(_shininess)
    { }
    
    /*
    ==================================
    material_manager::material_manager
    ==================================
    */
    material_manager::material_manager()
    { }

    /*
    ===============================
    material_manager::get_singleton
    ===============================
    */
    material_manager *material_manager::get_singleton()
    {
        static material_manager singleton;

        return &singleton;
    }

}
