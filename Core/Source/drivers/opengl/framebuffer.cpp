#include "DE/drivers/opengl/framebuffer.hpp"
#include "DE/drivers/opengl/texture.hpp"
#include "DE/drivers/opengl/renderbuffer.hpp"
#include "DE/image/bmp.hpp"

namespace deep
{

    namespace GL3
    {

        /*
        ==================================
        framebuffer_item::framebuffer_item
        ==================================
        */
        framebuffer_manager::framebuffer_item::framebuffer_item(GLuint _fbo)
            : fbo(_fbo),
              texture(0),
              rbo(0)
        { }

        /*
        ========================================
        framebuffer_manager::framebuffer_manager
        ========================================
        */
        framebuffer_manager::framebuffer_manager()
            : m_CurrentlyBound(0),
              m_CurrentID(0)
        { }

        /*
        ==================================
        framebuffer_manager::get_singleton
        ==================================
        */
        framebuffer_manager *framebuffer_manager::get_singleton()
        {
            static framebuffer_manager singleton;

            return &singleton;
        }

        /*
        ===========================
        framebuffer_manager::create
        ===========================
        */
        gl_id framebuffer_manager::create(const char *name)
        {
            GLuint fbo;
            DE_GL_CALL(glGenFramebuffers(1, &fbo));

            auto &el = m_Framebuffers.insert(name, framebuffer_item(fbo));

            return el.key;
        }

        /*
        ============================
        framebuffer_manager::rawBind
        ============================
        */
        void framebuffer_manager::rawBind(GLuint fbo)
        {
            DE_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

            m_CurrentlyBound = fbo;
        }

        /*
        =========================
        framebuffer_manager::bind
        =========================
        */
        bool framebuffer_manager::bind(gl_id fbo)
        {
            hash_entry<framebuffer_item> *el = m_Framebuffers[fbo];
            if(el == nullptr)
                return false;

            renderbuffer_manager *renderbufferManager = renderbuffer_manager::get_singleton();

            rawBind(el->value.fbo);
            m_CurrentID = fbo;

            hash_entry<GLuint> *al = renderbufferManager->get(el->value.rbo);
            if(al != nullptr)
            {
                renderbufferManager->set_currently_bound(al->value);
                renderbufferManager->set_current_id(el->value.rbo);
            }

            return true;
        }

        /*
        =========================
        framebuffer_manager::bind
        =========================
        */
        bool framebuffer_manager::bind(const char *name)
        {
            auto el = m_Framebuffers[name];
            if(el == nullptr)
                return false;

            rawBind(el->value.fbo);
            m_CurrentID = el->key;

            return true;
        }

        /*
        ==========================
        framebuffer_manager::check
        ==========================
        */
        bool framebuffer_manager::check()
        {
            return DE_GL_CALLV(glCheckFramebufferStatus(GL_FRAMEBUFFER)) == GL_FRAMEBUFFER_COMPLETE;
        }

        /*
        ================================
        framebuffer_manager::bindDefault
        ================================
        */
        void framebuffer_manager::bindDefault()
        {
            DE_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
            m_CurrentlyBound = 0;
            m_CurrentID = 0;
        }

        /*
        ===============================
        framebuffer_manager::rawDestroy
        ===============================
        */
        void framebuffer_manager::rawDestroy(GLuint fbo)
        {
            DE_GL_CALL(glDeleteFramebuffers(1, &fbo));
        }

        /*
        ============================
        framebuffer_manager::destroy
        ============================
        */
        bool framebuffer_manager::destroy(gl_id fbo, bool destroyTexture)
        {
            auto el = m_Framebuffers[fbo];
            if(el == nullptr)
                return false;

            if(destroyTexture)
            {
                GL3::texture_manager::get_singleton()->destroy(el->value.texture);
            }
            

            rawDestroy(el->value.fbo);
            m_Framebuffers.remove(el->key);

            return true;
        }

        /*
        ============================
        framebuffer_manager::destroy
        ============================
        */
        bool framebuffer_manager::destroy(const char *name, bool destroyTexture)
        {
            auto el = m_Framebuffers[name];
            if(el == nullptr)
                return false;

            if(destroyTexture)
                texture_manager::get_singleton()->destroy(el->value.texture);

            rawDestroy(el->value.fbo);
            m_Framebuffers.remove(el->key);

            return true;
        }

        /*
        ==================================
        framebuffer_manager::attachTexture
        ==================================
        */
        bool framebuffer_manager::attachTexture(GL3::gl_id texture)
        {
            auto el = GL3::texture_manager::get_singleton()->get(texture);
            if(el == nullptr)
                return false;

            auto al = m_Framebuffers[m_CurrentID];
            if(al == nullptr)
                return false;

            // Attache la texture au framebuffer.
            DE_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, el->value, 0));
            al->value.texture = texture;

            return true;
        }

        /*
        ==================================
        framebuffer_manager::attachTexture
        ==================================
        */
        bool framebuffer_manager::attachTexture(const char *name)
        {
            auto el = GL3::texture_manager::get_singleton()->get(name);
            if(el == nullptr)
                return false;

            // Attache la texture au framebuffer.
            DE_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, el->value, 0));
            m_Framebuffers[m_CurrentID]->value.texture = el->value;

            return true;
        }

        /*
        =======================================
        framebuffer_manager::attachRenderbuffer
        =======================================
        */
        bool framebuffer_manager::attachRenderbuffer(gl_id fbo, gl_id rbo)
        {
            hash_entry<framebuffer_item> *el = m_Framebuffers[fbo];
            if(el == nullptr)
                return false;

            renderbuffer_manager *renderbufferManager = renderbuffer_manager::get_singleton();

            hash_entry<GLuint> *al = renderbufferManager->get(rbo);
            if(al == nullptr)
                return false;

            DE_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, el->value.fbo));
            el->value.rbo = rbo;

            return true;
        }

        /*
        =======================================
        framebuffer_manager::attachRenderbuffer
        =======================================
        */
        bool framebuffer_manager::attachRenderbuffer(const char *fboName, const char *rboName)
        {
            hash_entry<framebuffer_item> *el = m_Framebuffers[fboName];
            if(el == nullptr)
                return false;

            renderbuffer_manager *renderbufferManager = renderbuffer_manager::get_singleton();

            hash_entry<GLuint> *al = renderbufferManager->get(rboName);
            if(al == nullptr)
                return false;

            DE_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, el->value.fbo));
            el->value.rbo = al->key;

            return true;
        }

        /*
        =======================================
        framebuffer_manager::saveTextureAsImage
        =======================================
        */
        bool framebuffer_manager::saveTextureAsImage(int width, int height, const char *filedest)
        {
            uint8_t *buffer = static_cast<uint8_t *>(mem::alloc(static_cast<uint64_t>(width) * height * sizeof(uint8_t) * 4));
            if(buffer == nullptr)
                return false;

            // Cette fonction permet de récupérer l'ensemble des pixels du framebuffer actif.
            // Après plusieurs tests, il semblerait que si le format est GL_RGB,
            // l'octet correspondant à la transparence est mis à 0 au lieu d'être ignoré.
            // Donc autant utiliser GL_RGBA ou GL_BGRA.
            DE_GL_CALL(glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, buffer));   // GL_BGRA est utilisé car le format BMP stock les pixels dans ce format.

            bmp image;
            if(!image.create(width, height, 32, image_color_space::BGRA))
            {
                mem::free(buffer);
                return false;
            }

            image.convert_raw(buffer);
            image.save(filedest);
            image.destroy();

            mem::free(buffer);

            return true;
        }

    }

}
