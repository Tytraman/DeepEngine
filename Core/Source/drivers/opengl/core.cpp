#include "DE/drivers/opengl/core.hpp"

namespace deep
{

    namespace GL3
    {
        
        /*
        ===================
        core::query_version
        ===================
        */
        const char *core::query_version()
        {
            return (const char *) glGetString(GL_VERSION);
        }

        /*
        =====================
        core::update_viewport
        =====================
        */
        void core::update_viewport(int width, int height)
        {
            glViewport(0, 0, width, height);
        }

        /*
        ==============================
        core::query_max_vertex_attribs
        ==============================
        */
        int core::query_max_vertex_attribs()
        {
            int val = 0;
            DE_GL_CALL(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &val));
            return val;
        }

        /*
        ===================================
        core::query_max_texture_image_units
        ===================================
        */
        int core::query_max_texture_image_units()
        {
            int val = 0;
            DE_GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &val));
            return val;
        }

        /*
        ============================
        core::query_max_texture_size
        ============================
        */
        int core::query_max_texture_size()
        {
            int val = 0;
            DE_GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val));
            return val;
        }

        /*
        ===============================
        core::query_max_3D_texture_size
        ===============================
        */
        int core::query_max_3D_texture_size()
        {
            int val = 0;
            DE_GL_CALL(glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &val));
            return val;
        }

        /*
        ====================================
        core::query_max_array_texture_layers
        ====================================
        */
        int core::query_max_array_texture_layers()
        {
            int val = 0;
            DE_GL_CALL(glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &val));
            return val;
        }

        /*
        =======================
        core::enable_depth_mask
        =======================
        */
        void core::enable_depth_mask()
        {
            DE_GL_CALL(glDepthMask(GL_TRUE));
        }

        /*
        ========================
        core::disable_depth_mask
        ========================
        */
        void core::disable_depth_mask()
        {
            DE_GL_CALL(glDepthMask(GL_FALSE));
        }

        /*
        =========================
        core::enable_depth_test
        =========================
        */
        void core::enable_depth_test()
        {
            DE_GL_CALL(glEnable(GL_DEPTH_TEST));
        }

        /*
        ==========================
        core::disable_depth_test
        ==========================
        */
        void core::disable_depth_test()
        {
            DE_GL_CALL(glDisable(GL_DEPTH_TEST));
        }

        /*
        ==========================
        core::set_depth_function
        ==========================
        */
        void core::set_depth_function(gl_depth_function func)
        {
            DE_GL_CALL(glDepthFunc(static_cast<GLenum>(func)));
        }

        /*
        =====================
        core::set_cull_face
        =====================
        */
        void core::set_cull_face(gl_cull_face cullFace)
        {
            DE_GL_CALL(glCullFace(static_cast<GLenum>(cullFace)));
        }

    }

}
