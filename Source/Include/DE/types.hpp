#ifndef __DEEP_ENGINE_TYPES_HPP__
#define __DEEP_ENGINE_TYPES_HPP__

#include <DE/def.hpp>
#include <DE/color.hpp>

#include <stdint.h>

#include <SDL.h>

/// @namespace de
/// @brief Espace de nom regroupant tout ce qui est relatif au DeepEngine.
namespace deep {


#if DE_WINDOWS
	typedef WCHAR os_char;

	typedef HANDLE os_fd;

#define NULL_FD NULL

#else	// DE_WINDOWS
	typedef char os_char;

	typedef int os_fd;

#define NULL_FD (-1)

#endif	// else DE_WINDOWS

	using de_id = uint32_t;

	constexpr size_t nothing = -1;	///< Quand rien n'a été trouvé.
	constexpr de_id badID = -1;		///< Quand un ID n'est pas valide.
	
	typedef SDL_Keycode keycode;

	namespace direction {
		enum Direction : uint8_t {
			Left,
			Right,
			Up,
			Down
		};
	}

	DE_UNION(dual8,
		struct {
			uint8_t l;
			uint8_t h;
		};
		uint16_t value;
	);

	DE_UNION(dual16,
		struct {
			uint16_t l;
			uint16_t h;
		};
		uint32_t value;
	);

	DE_UNION(dual32,
		struct {
			uint32_t l;
			uint32_t h;
		};
		uint64_t value;
	);

	struct DE_API size {
		uint32_t width;
		uint32_t height;

		size();
		size(uint32_t width, uint32_t height);
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
	inline size::size(uint32_t width, uint32_t height)
	{
		this->width = width;
		this->height = height;
	}

	/// @brief Position X et Y sont formes d'entiers. @ref fposition pour la forme flottante.
	struct DE_API position {
		int32_t x;
		int32_t y;

		position();
		position(int32_t x, int32_t y);
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
	inline position::position(int32_t x, int32_t y)
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

}

#endif