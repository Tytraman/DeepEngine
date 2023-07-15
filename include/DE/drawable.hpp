#ifndef __DEEP_ENGINE_DRAWABLE_HPP__
#define __DEEP_ENGINE_DRAWABLE_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/drawable_types.h>
#include <DE/panel.hpp>
#include <DE/renderer.hpp>
#include <DE/vec.hpp>

#include <stdint.h>
#include <vector>

namespace de {

	class Window;

	class DE_API Drawable {

		friend Window;
		friend DrawablePanel;

		protected:
			fvec2 _pos;
			bool _visible;
			bool _drawVectors;
			DrawablePanel *_panel;

		protected:
			Drawable(DrawablePanel *panel, const fvec2 &pos, bool visible = true);

		public:
			Drawable() = delete;

			//=== Méthodes virtuelles pures ===//
			virtual void draw(const fmat2x2 &baseVectors) = 0;
			virtual void rotate(float degrees) = 0;
			virtual uint32_t getType() const = 0;

			//=== Mééthodes virtuelles ===//
			virtual void move(direction::Direction direction, int value);
			virtual void setPos(const fvec2 &pos);

			
			fvec2 getPos() const;
			bool isVisible() const;

			
			void setVisible(bool value);
			void setVectorsVisible(bool value);
	};

	/*
	================
	Drawable::getPos
	================
	*/
	inline fvec2 Drawable::getPos() const
	{
		return _pos;
	}

	/*
	===================
	Drawable::isVisible
	===================
	*/
	inline bool Drawable::isVisible() const
	{
		return _visible;
	}

	/*
	================
	Drawable::setPos
	================
	*/
	inline void Drawable::setPos(const fvec2 &pos)
	{
		_pos = pos;
	}

	/*
	====================
	Drawable::setVisible
	====================
	*/
	inline void Drawable::setVisible(bool value)
	{
		_visible = value;
	}

	/*
	====================
	Drawable::setVisible
	====================
	*/
	inline void Drawable::setVectorsVisible(bool value)
	{
		_drawVectors = value;
	}

	class DE_API ColoredDrawable : public Drawable {

		protected:
			colora _color;

		protected:
			ColoredDrawable(DrawablePanel *panel, const fvec2 &pos, const colora &color, bool visible = true);

		public:
			ColoredDrawable() = delete;

	};

}

#endif