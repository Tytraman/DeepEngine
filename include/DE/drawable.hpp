#ifndef __DEEP_ENGINE_DRAWABLE_HPP__
#define __DEEP_ENGINE_DRAWABLE_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/drawable_types.h>
#include <DE/renderer.hpp>
#include <DE/vec.hpp>

#include <stdint.h>

namespace de {

	class Window;

	/// @brief Classe 
	class DE_API Drawable {

		friend Window;

		protected:
			Renderer *_renderer;
			vec2 _pos;
			bool _visible;
			bool _drawVectors;

		protected:
			Drawable(Renderer *renderer, const vec2 &pos, bool visible = true);
			Drawable(Renderer *renderer, const vec2 &&pos, bool visible = true);

		public:
			Drawable() = delete;

			virtual void rotate(float degrees) = 0;
			virtual void draw() = 0;

			virtual void move(direction::Direction direction, int value);

			//===== Getters =====//

			virtual uint32_t getType() = 0;
			Renderer *getRenderer();
			vec2 getPos() const;
			bool isVisible() const;

			//===== Setters =====//

			virtual void setPos(const vec2 &pos);

			void setRenderer(Renderer *renderer);
			void setVisible(bool value);
			void setVectorsVisible(bool value);

	};

	/*
	=====================
	Drawable::getRenderer
	=====================
	*/
	inline Renderer *Drawable::getRenderer()
	{
		return _renderer;
	}

	/*
	================
	Drawable::getPos
	================
	*/
	inline vec2 Drawable::getPos() const
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
	=====================
	Drawable::setRenderer
	=====================
	*/
	inline void Drawable::setRenderer(Renderer *renderer)
	{
		_renderer = renderer;
	}

	/*
	================
	Drawable::setPos
	================
	*/
	inline void Drawable::setPos(const vec2 &pos)
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

	class DE_API BasicDrawable : public Drawable {

		private:
			

	};

}

#endif