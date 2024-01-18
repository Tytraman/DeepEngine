#ifndef __DEEP_ENGINE_MATERIAL_HPP__
#define __DEEP_ENGINE_MATERIAL_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/vec.hpp"
#include "DE/memory/hash_table.hpp"
#include "DE/memory/ref_counter.hpp"
#include "DE/string.hpp"

#include "DE/drivers/opengl/shader.hpp"

namespace deep
{

    enum class material_type : uint8_t
    {
        None,
        Custom,
        Colored,
        Textured
    };

    class imaterial : public reference_counter
    {

        public:
            DE_API material_type get_type() const;
            DE_API GL3::gl_id get_program() const;

            DE_API void set_program(GL3::gl_id program);

            DE_API virtual bool send_data() = 0;

        protected:
            material_type m_Type;
            GL3::gl_id m_Program;

    };

    inline material_type imaterial::get_type() const
    {
        return m_Type;
    }

    inline GL3::gl_id imaterial::get_program() const
    {
        return m_Program;
    }

    inline void imaterial::set_program(GL3::gl_id program)
    {
        m_Program = program;
    }

    class color_material : public imaterial
    {

        public:
            DE_API color_material(GL3::gl_id program, const vec3<float> &ambient, const vec3<float> &diffuse, const vec3<float> &specular, float shininess);

            DE_API bool send_data() override;

            DE_API vec3<float> get_ambient() const;
            DE_API vec3<float> get_diffuse() const;
            DE_API vec3<float> get_specular() const;
            DE_API float get_shininess() const;

            DE_API void set_ambient(const vec3<float> &ambient);
            DE_API void set_diffuse(const vec3<float> &diffuse);
            DE_API void set_specular(const vec3<float> &specular);
            DE_API void set_shininess(float shininess);

        private:
            vec3<float> m_Ambient;
            vec3<float> m_Diffuse;
            vec3<float> m_Specular;
            float       m_Shininess;

    };

    /*
    ===========================
    color_material::get_ambient
    ===========================
    */
    inline vec3<float> color_material::get_ambient() const
    {
        return m_Ambient;
    }

    /*
    ===========================
    color_material::get_diffuse
    ===========================
    */
    inline vec3<float> color_material::get_diffuse() const
    {
        return m_Diffuse;
    }

    /*
    ============================
    color_material::get_specular
    ============================
    */
    inline vec3<float> color_material::get_specular() const
    {
        return m_Specular;
    }

    /*
    =============================
    color_material::get_shininess
    =============================
    */
    inline float color_material::get_shininess() const
    {
        return m_Shininess;
    }

    /*
    ===========================
    color_material::set_ambient
    ===========================
    */
    inline void color_material::set_ambient(const vec3<float> &ambient)
    {
        m_Ambient = ambient;
    }

    /*
    ===========================
    color_material::set_diffuse
    ===========================
    */
    inline void color_material::set_diffuse(const vec3<float> &diffuse)
    {
        m_Diffuse = diffuse;
    }

    /*
    ============================
    color_material::set_specular
    ============================
    */
    inline void color_material::set_specular(const vec3<float> &specular)
    {
        m_Specular = specular;
    }

    /*
    =============================
    color_material::set_shininess
    =============================
    */
    inline void color_material::set_shininess(float shininess)
    {
        m_Shininess = shininess;
    }

    class texture_material : public imaterial
    {

        public:
            DE_API texture_material(GL3::gl_id program, GL3::gl_id diffuseTexture, const vec3<float> &specular, float shininess);

            DE_API bool send_data() override;

        private:
            GL3::gl_id  m_DiffuseTexture;
            vec3<float> m_Specular;
            float       m_Shininess;

    };

}

#endif