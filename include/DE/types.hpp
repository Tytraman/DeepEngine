#ifndef __DEEP_ENGINE_TYPES_HPP__
#define __DEEP_ENGINE_TYPES_HPP__

#include <DE/def.h>
#include <DE/int.hpp>
#include <DE/color.hpp>

#include <SDL.h>

namespace de {


#if DE_WINDOWS
	typedef WCHAR os_char;

	typedef HANDLE os_fd;

#define NULL_FD NULL

#else	// DE_WINDOWS
	typedef char os_char;

	typedef int os_fd;

#define NULL_FD (-1)

#endif	// else DE_WINDOWS

	
	typedef SDL_Keycode keycode;
	typedef SDL_Vertex	vertex;

	namespace direction {
		enum Direction : uint8 {
			Left,
			Right,
			Up,
			Down
		};
	}

	DE_UNION(dual8,
		struct {
			uint8 l;
			uint8 h;
		};
		uint16 value;
	);

	DE_UNION(dual16,
		struct {
			uint16 l;
			uint16 h;
		};
		uint32 value;
	);

	DE_UNION(dual32,
		struct {
			uint32 l;
			uint32 h;
		};
		uint64 value;
	);

	struct DE_API size {
		uint32 width;
		uint32 height;

		size();
		size(uint32 width, uint32 height);
	};

	/*
	==========
	size::size
	==========
	*/
	inline size::size() 
	{
		width = 0;
		height = 0;
	}

	/*
	==========
	size::size
	==========
	*/
	inline size::size(uint32 width, uint32 height)
	{
		this->width = width;
		this->height = height;
	}

	/// @brief Position X et Y sont formes d'entiers. @ref fposition pour la forme flottante.
	struct DE_API position {
		int32 x;
		int32 y;

		position();
		position(int32 x, int32 y);
	};

	/*
	==================
	position::position
	==================
	*/
	inline position::position()
	{
		this->x = 0;
		this->y = 0;
	}

	/*
	==================
	position::position
	==================
	*/
	inline position::position(int32 x, int32 y)
	{
		this->x = x;
		this->y = y;
	}

	/// @brief Position X et Y sont formes de flottants. @ref position pour la forme entière.
	struct DE_API fposition {
		float x;
		float y;

		fposition();
		fposition(float x, float y);
	};

	/*
	====================
	fposition::fposition
	====================
	*/
	inline fposition::fposition()
	{
		this->x = 0.0f;
		this->y = 0.0f;
	}

	/*
	====================
	fposition::fposition
	====================
	*/
	inline fposition::fposition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	struct DE_API rect {
		position pos;
		size dim;

		rect();
		rect(const position &&pos, const size &&dim);
	};

	/*
	==========
	rect::rect
	==========
	*/
	inline rect::rect()
		: pos(), dim()
	{ }

	/*
	==========
	rect::rect
	==========
	*/
	inline rect::rect(const position &&pos, const size &&dim)
	{
		this->pos = pos;
		this->dim = dim;
	}

}

#endif