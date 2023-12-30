#ifndef __DEEP_ENGINE_SHAPE_HPP__
#define __DEEP_ENGINE_SHAPE_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/vec.hpp"
#include "DE/drivers/opengl/def.hpp"

namespace deep
{

    class DE_API polygon
    {

        public:
            polygon(GL3::gl_id vbo, GL3::gl_id vao);

            GL3::gl_id vbo() const;
            GL3::gl_id vao() const;

        private:
            GL3::gl_id m_VBO;
            GL3::gl_id m_VAO;

    };

    /*
    ============
    polygon::vbo
    ============
    */
    inline GL3::gl_id polygon::vbo() const
    {
        return m_VBO;
    }

    /*
    ============
    polygon::vao
    ============
    */
    inline GL3::gl_id polygon::vao() const
    {
        return m_VAO;
    }

    /// @struct rect
    /// @brief  Structure représentant un rectangle.
    struct DE_API rect
    {
        fvec2 pos; ///< Position du rectangle.
        float w;   ///< Largeur du rectangle.
        float h;   ///< Hauteur du rectangle.

        /// @brief          Constructeur de rectangle.
        /// @param position Position du rectangle.
        /// @param width    Largeur du rectangle.
        /// @param height   Hauteur du rectangle.
        rect(const fvec2 &position = fvec2(0.0f, 0.0f), float width = 0.0f, float height = 0.0f);

        /// @brief   Vérifie si un point se situe à l'intérieur du rectangle.
        /// @param v Le vecteur décrivant le point.
        /// @return  \c true si le point se situe à l'intérieur du rectangle.
        bool isInside(const fvec2 &v) const;

        bool isInside(const rect &r) const;
    };

    /*
    ==============
    rect::isInside
    ==============
    */
    inline bool rect::isInside(const fvec2 &v) const
    {
        return (v.x >= pos.x && v.y >= pos.y && v.x < pos.x + w && v.y < pos.y + h);
    }

    /*
    ==============
    rect::isInside
    ==============
    */
    inline bool rect::isInside(const rect &r) const
    {
        return (r.pos.y + r.h > pos.y && r.pos.y < pos.y + h &&
                r.pos.x + r.w > pos.x && r.pos.x < pos.x + w);
    }

}

#endif