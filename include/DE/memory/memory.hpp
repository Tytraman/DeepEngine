#ifndef __DEEP_ENGINE_MEMORY_HPP__
#define __DEEP_ENGINE_MEMORY_HPP__

#include <DE/def.h>
#include <DE/types.hpp>

namespace de {

#if DE_WINDOWS
	using mem_ptr = LPVOID;
#else

#endif

	namespace MemoryStatus {

		enum Code : uint8_t {
			OK,
			ProcessHeapError,
			AllocError,
			ReallocError
		};

	}

	/// @class MemoryManager
	/// @brief Permet une gestion plus simple de la mémoire.
	class DE_API MemoryManager {

		public:
			static mem_ptr allocNoTrack(size_t size);
			static mem_ptr alloc(size_t size);

			static mem_ptr reallocNoTrack(mem_ptr memory, size_t newSize);
			static mem_ptr realloc(mem_ptr memory, size_t newSize);

			static void freeNoTrack(mem_ptr memory);
			static void free(mem_ptr memory);

	};

	using mem = MemoryManager;

}

#endif