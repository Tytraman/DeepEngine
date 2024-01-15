#ifndef __DEEP_ENGINE_DRAWABLE_HPP__
#define __DEEP_ENGINE_DRAWABLE_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/drawable_types.h>
#include <DE/renderer.hpp>
#include <DE/vec.hpp>

#include <stdint.h>
#include <vector>

namespace deep
{

	class window;

	class drawable
    {

		friend window;

		protected:
			fvec2 m_Pos;
			bool m_Visible;
			bool m_DrawVectors;

		protected:
			drawable(const fvec2 &pos, bool visible = true);

		public:
			//=== Méthodes virtuelles pures ===// 
			DE_API virtual void draw(const fmat2x2 &baseVectors) = 0;

			DE_API virtual void scale(float scale1, float scale2) = 0;
			DE_API virtual void translate(float x, float y) = 0;
			DE_API virtual void rotate(float degrees) = 0;
			
			DE_API virtual uint32_t getType() const = 0;
			DE_API fvec2 getPos() const;
			DE_API bool isVisible() const;

			DE_API virtual void setPos(const fvec2 &pos);
			DE_API void setVisible(bool value);
			DE_API void setVectorsVisible(bool value);

        public:
            drawable() = delete;

	};

	/*
	================
	drawable::getPos
	================
	*/
	inline fvec2 drawable::getPos() const
	{
		return m_Pos;
	}

	/*
	===================
	drawable::isVisible
	===================
	*/
	inline bool drawable::isVisible() const
	{
		return m_Visible;
	}

	/*
	================
	drawable::setPos
	================
	*/
	inline void drawable::setPos(const fvec2 &pos)
	{
		m_Pos = pos;
	}

	/*
	====================
	drawable::setVisible
	====================
	*/
	inline void drawable::setVisible(bool value)
	{
		m_Visible = value;
	}

	/*
	====================
	drawable::setVisible
	====================
	*/
	inline void drawable::setVectorsVisible(bool value)
	{
		m_DrawVectors = value;
	}

	class ColoredDrawable : public drawable
	{

		protected:
			colora m_Color;

		protected:
			ColoredDrawable(const fvec2 &pos, const colora &color, bool visible = true);

		public:
			ColoredDrawable() = delete;

	};

}

#endif