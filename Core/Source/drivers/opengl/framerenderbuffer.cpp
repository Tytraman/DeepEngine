#include "DE/drivers/opengl/framerenderbuffer.hpp"
#include "DE/drivers/opengl/framebuffer.hpp"
#include "DE/drivers/opengl/renderbuffer.hpp"
#include "DE/drivers/opengl/vbo.hpp"
#include "DE/drivers/opengl/vao.hpp"
#include "DE/drivers/opengl/texture.hpp"

namespace deep
{

    namespace GL3
    {

        /*
        =========================
        framerenderbuffer::create
        =========================
        */
        bool framerenderbuffer::create(const char *name, int width, int height)
        {
            framebuffer_manager *framebufferManager = framebuffer_manager::get_singleton();
            renderbuffer_manager *renderbufferManager = renderbuffer_manager::get_singleton();
            vbo_manager *vboManager = vbo_manager::get_singleton();
            vao_manager *vaoManager = vao_manager::get_singleton();
            texture_manager *textureManager = texture_manager::get_singleton();

            // Crée un framebuffer.
            m_Framebuffer = framebufferManager->create(name);
            framebufferManager->bind(m_Framebuffer);

            string strName = name;
            strName.append("_fb");

            // Alloue de la place pour stocker la texture du framebuffer.
            gl_id texture = textureManager->create_2D(strName.str());
            textureManager->bind(texture, 0);
            textureManager->alloc_space(width, height);
            textureManager->set_texture_filtering(texture_manager::gl_texture_filter::Linear);

            // Indique que le framebuffer doit utiliser la place précédemment allouée.
            framebufferManager->attach_texture(texture);

            // Crée un renderbuffer.
            string renderbufferName = name;
            renderbufferName.append("_rb");
            m_Renderbuffer = renderbufferManager->create(renderbufferName.str());
            renderbufferManager->bind(m_Renderbuffer);
            renderbufferManager->store(width, height);     // Indique la taille du renderbuffer, a la même taille que le framebuffer car ils seront liés.
            renderbufferManager->bind_default();

            // Lie le renderbuffer au framebuffer.
            framebufferManager->attach_renderbuffer(m_Framebuffer, m_Renderbuffer);

            // Vérifie que le framebuffer a bien été créé et fonctionnel.
            bool ret = framebufferManager->check();

            framebufferManager->bind_default();

            if(!ret)
            {
                renderbufferManager->destroy(m_Renderbuffer);
                textureManager->destroy(texture);
                framebufferManager->destroy(m_Framebuffer);

                return false;
            }

            strName = name;
            strName.append("_vbo");
            m_VBO = vboManager->create(strName.str());

            string vaoName = name;
            vaoName.append("_vao");
            m_VAO = vaoManager->create(vaoName.str());

            float vPos[] =
            {
                // Positions  // Coordonnées de texture
                -1.0f,  1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f,
                 1.0f, -1.0f, 1.0f, 0.0f,
            

                -1.0f,  1.0f, 0.0f, 1.0f,
                 1.0f, -1.0f, 1.0f, 0.0f,
                 1.0f,  1.0f, 1.0f, 1.0f
            };

            memory_chunk chunk(vPos, sizeof(vPos));

            vboManager->bind(m_VBO);
            vaoManager->bind(m_VAO);

            vboManager->transmit_data(chunk);
            vboManager->set_vertices_number(6);

            vboManager->add_attribute(0, gl_attrib_components_number::x2, gl_type::Float, 4 * sizeof(float), 0);
            vboManager->add_attribute(1, gl_attrib_components_number::x2, gl_type::Float, 4 * sizeof(float), 2 * sizeof(float));

            vaoManager->bind_default();
            vboManager->bind_default();

            m_Width = width;
            m_Height = height;

            return true;
        }

        /*
        ==========================
        framerenderbuffer::destroy
        ==========================
        */
        void framerenderbuffer::destroy()
        {
            framebuffer_manager *framebufferManager = framebuffer_manager::get_singleton();
            renderbuffer_manager *renderbufferManager = renderbuffer_manager::get_singleton();

            renderbufferManager->destroy(m_Renderbuffer);
            framebufferManager->destroy(m_Framebuffer, true);
        }

        /*
        =====================================
        framerenderbuffer::saveTextureAsImage
        =====================================
        */
        bool framerenderbuffer::saveTextureAsImage(const char *destpath)
        {
            framebuffer_manager *framebufferManager = framebuffer_manager::get_singleton();

            gl_id currentBuffer = framebufferManager->current_id();

            if(!framebufferManager->bind(m_Framebuffer))
                return false;

            bool ret = framebufferManager->save_texture_as_image(m_Width, m_Height, destpath);

            framebufferManager->bind(currentBuffer);

            return ret;
        }

        /*
        =====================================
        framerenderbuffer::resize
        =====================================
        */
        bool framerenderbuffer::resize(const char *newName, int width, int height)
        {
            destroy();

            return create(newName, width, height);
        }

    }

}
