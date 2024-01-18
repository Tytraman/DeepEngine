#include "DE/graphics/material.hpp"
#include "DE/drivers/opengl/uniform.hpp"

namespace deep
{

    /*
    ==============================
    color_material::color_material
    ==============================
    */
    color_material::color_material(GL3::gl_id program, const vec3<float> &ambient, const vec3<float> &diffuse, const vec3<float> &specular, float shininess)
        : m_Ambient(ambient),
          m_Diffuse(diffuse),
          m_Specular(specular),
          m_Shininess(shininess)
    {
        m_Type = material_type::Colored;
        m_Program = program;
    }

    /*
    =========================
    color_material::send_data
    =========================
    */
    bool color_material::send_data()
    {
        GL3::program_manager *programManager = GL3::program_manager::get_singleton();

        if(programManager->current_id() != m_Program)
            programManager->use(m_Program);

        programManager->set_uniform("deMaterial.ambient", m_Ambient);
        programManager->set_uniform("deMaterial.diffuse", m_Diffuse);
        programManager->set_uniform("deMaterial.specular", m_Specular);
        programManager->set_uniform("deMaterial.shininess", m_Shininess);

        return programManager->send_uniforms();
    }

}
