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

	class DE_API drawable
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
			virtual void draw(const fmat2x2 &baseVectors) = 0;

			virtual void scale(float scale1, float scale2) = 0;
			virtual void translate(float x, float y) = 0;
			virtual void rotate(float degrees) = 0;
			
			virtual uint32_t getType() const = 0;
			fvec2 getPos() const;
			bool isVisible() const;

			virtual void setPos(const fvec2 &pos);
			void setVisible(bool value);
			void setVectorsVisible(bool value);

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

	class DE_API ColoredDrawable : public drawable {

		protected:
			colora m_Color;

		protected:
			ColoredDrawable(const fvec2 &pos, const colora &color, bool visible = true);

		public:
			ColoredDrawable() = delete;

	};

}

#endif