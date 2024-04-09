#ifndef __DEEP_ENGINE_SCENE_GRAPH_HPP__
#define __DEEP_ENGINE_SCENE_GRAPH_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/ref_counted.hpp"
#include "core/string/string.hpp"
#include "graphics/material.hpp"
#include "maths/vec.hpp"

namespace deep
{

    struct scene_node_transform
    {

    };

    class scene_node_properties
    {

        public:

        private:
            uint64_t m_ID;
            string m_Name;
            ref<material> m_Material;

    };

    class scene_node : public ref_counted
    {



    };

}

#endif