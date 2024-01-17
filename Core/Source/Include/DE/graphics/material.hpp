#ifndef __DEEP_ENGINE_MATERIAL_HPP__
#define __DEEP_ENGINE_MATERIAL_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/vec.hpp"
#include "DE/memory/hash_table.hpp"
#include "DE/string.hpp"

#include "DE/drivers/opengl/shader.hpp"

namespace deep
{

    class imaterial
    {

        public:
            DE_API virtual bool send_data() = 0;

    };

    class color_material : public imaterial
    {

        public:
            DE_API color_material(GL3::gl_id program, const vec3<float> &ambient, const vec3<float> &diffuse, const vec3<float> &specular, float shininess);

            DE_API bool send_data() override;

        private:
            vec3<float> m_Ambient;
            vec3<float> m_Diffuse;
            vec3<float> m_Specular;
            float       m_Shininess;

            GL3::gl_id m_Program;

    };

}

#endif