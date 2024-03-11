#ifndef __DEEP_ENGINE_MATERIAL_HPP__
#define __DEEP_ENGINE_MATERIAL_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/maths/vec.hpp"
#include "DE/core/hash_table.hpp"
#include "DE/core/ref_counted.hpp"
#include "DE/core/string.hpp"

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

    class material : public ref_counted
    {

        public:
            DE_API material_type get_type() const;
            DE_API GL3::gl_id get_program() const;

            DE_API void set_program(GL3::gl_id program);

            virtual bool send_data() = 0;

        protected:
            material_type m_Type;
            GL3::gl_id m_Program;

    };

    /*
    ==================
    material::get_type
    ==================
    */
    inline material_type material::get_type() const
    {
        return m_Type;
    }

    /*
    =====================
    material::get_program
    =====================
    */
    inline GL3::gl_id material::get_program() const
    {
        return m_Program;
    }

    /*
    =====================
    material::set_program
    =====================
    */
    inline void material::set_program(GL3::gl_id program)
    {
        m_Program = program;
    }

    class color_material : public material
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

    class texture_material : public material
    {

        public:
            DE_API texture_material(GL3::gl_id program, GL3::gl_id diffuseTexture, const vec3<float> &specular, float shininess);

            DE_API bool send_data() override;

            DE_API GL3::gl_id get_diffuse_texture() const;
            DE_API vec3<float> get_specular() const;
            DE_API float get_shininess() const;

            DE_API void set_diffuse_texture(GL3::gl_id texture);
            DE_API void set_specular(const vec3<float> &specular);
            DE_API void set_shininess(float shininess);

        private:
            GL3::gl_id  m_DiffuseTexture;
            vec3<float> m_Specular;
            float       m_Shininess;

    };

    /*
    ===================================
    color_material::get_diffuse_texture
    ===================================
    */
    inline GL3::gl_id texture_material::get_diffuse_texture() const
    {
        return m_DiffuseTexture;
    }

    /*
    ============================
    color_material::get_specular
    ============================
    */
    inline vec3<float> texture_material::get_specular() const
    {
        return m_Specular;
    }

    /*
    =============================
    color_material::get_shininess
    =============================
    */
    inline float texture_material::get_shininess() const
    {
        return m_Shininess;
    }

    /*
    ===================================
    color_material::set_diffuse_texture
    ===================================
    */
    inline void texture_material::set_diffuse_texture(GL3::gl_id texture)
    {
        m_DiffuseTexture = texture;
    }

    /*
    ============================
    color_material::set_specular
    ============================
    */
    inline void texture_material::set_specular(const vec3<float> &specular)
    {
        m_Specular = specular;
    }

    /*
    =============================
    color_material::set_shininess
    =============================
    */
    inline void texture_material::set_shininess(float shininess)
    {
        m_Shininess = shininess;
    }

}

#endif