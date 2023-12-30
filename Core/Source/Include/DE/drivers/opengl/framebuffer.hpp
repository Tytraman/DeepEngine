#ifndef __DEEP_ENGINE_OPENGL_FRAMEBUFFER_HPP__
#define __DEEP_ENGINE_OPENGL_FRAMEBUFFER_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/memory/hash_table.hpp"
#include "DE/drivers/opengl/def.hpp"

namespace deep
{

    namespace GL3
    {

        class framebuffer_manager
        {

            public:
                struct framebuffer_item
                {
                    GLuint fbo;
                    gl_id texture;
                    gl_id rbo;

                    DE_API framebuffer_item(GLuint fbo);
                };

            public:
                DE_API static framebuffer_manager *get_singleton();

                DE_API gl_id create(const char *name);

                DE_API void rawBind(GLuint fbo);
                DE_API bool bind(gl_id fbo);
                DE_API bool bind(const char *name);

                DE_API void bindDefault();

                DE_API bool check();

                DE_API void rawDestroy(GLuint fbo);
                DE_API bool destroy(gl_id fbo, bool destroyTexture = false);
                DE_API bool destroy(const char *name, bool destroyTexture = false);

                DE_API bool attachTexture(GL3::gl_id texture);
                DE_API bool attachTexture(const char *name);

                DE_API bool attachRenderbuffer(gl_id fbo, gl_id rbo);
                DE_API bool attachRenderbuffer(const char *fboName, const char *rboName);

                DE_API bool saveTextureAsImage(int width, int height, const char *filedest);

                DE_API hash_entry<framebuffer_item> *get(gl_id fbo);
                DE_API hash_entry<framebuffer_item> *get(const char *name);

                DE_API GLuint currentlyBound();
                DE_API gl_id currentID();
                DE_API gl_id currentTextureID();

            private:
                framebuffer_manager();

                GLuint m_CurrentlyBound;
                gl_id m_CurrentID;

                hash_table<framebuffer_item> m_Framebuffers;

            public:
                framebuffer_manager(const framebuffer_manager &) = delete;
                framebuffer_manager(framebuffer_manager &&) = delete;

        };

        /*
        ========================
        framebuffer_manager::get
        ========================
        */
        inline hash_entry<framebuffer_manager::framebuffer_item> *framebuffer_manager::get(gl_id fbo)
        {
            return m_Framebuffers[fbo];
        }

        /*
        ========================
        framebuffer_manager::get
        ========================
        */
        inline hash_entry<framebuffer_manager::framebuffer_item> *framebuffer_manager::get(const char *name)
        {
            return m_Framebuffers[name];
        }

        /*
        ===================================
        framebuffer_manager::currentlyBound
        ===================================
        */
        inline GLuint framebuffer_manager::currentlyBound()
        {
            return m_CurrentlyBound;
        }

        /*
        ==============================
        framebuffer_manager::currentID
        ==============================
        */
        inline gl_id framebuffer_manager::currentID()
        {
            return m_CurrentID;
        }

        /*
        =====================================
        framebuffer_manager::currentTextureID
        =====================================
        */
        inline gl_id framebuffer_manager::currentTextureID()
        {
            hash_entry<framebuffer_item> *el = m_Framebuffers[m_CurrentID];
            if(el == nullptr)
                return 0;

            return el->value.texture;
        }

    }

}

#endif