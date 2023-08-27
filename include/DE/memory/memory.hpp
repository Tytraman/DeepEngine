#ifndef __DEEP_ENGINE_MEMORY_HPP__
#define __DEEP_ENGINE_MEMORY_HPP__

#include <DE/def.hpp>
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

			/// @brief      Alloue un espace mémoire sans le traquer avec le gestionnaire de mémoire.
			/// @param size Nombre d'octets à allouer pour l'espace mémoire. 
			/// @return     L'adresse vers l'espace mémoire ou \c nullptr si une erreur est survenue.
			static mem_ptr allocNoTrack(size_t size);

			/// @brief      Alloue un espace mémoire en le traquant avec le gesionnaire de mémoire. <i>(Recommandé pour éviter les memory leaks)</i>.
			/// @param size Nombre d'octets à allouer pour l'espace mémoire.
			/// @return     L'adresse vers l'espace mémoire ou \c nullptr si une erreur ests survenue.
			static mem_ptr alloc(size_t size);

			/// @brief         Ré-alloue un espace mémoire sans le traquer avec le gestionnaire de mémoire.
			/// @param memory  Adresse de l'espace mémoire à ré-allouer.
			/// @param newSize Nombre d'octets à allouer pour l'espace mémoire. 
			/// @return        L'adresse vers le nouvel espace mémoire ou \c nullptr si une erreur est survenue.
			static mem_ptr reallocNoTrack(mem_ptr memory, size_t newSize);

			/// @brief         Ré-alloue un espace mémoire en le traquant avec le gestionnaire de mémoire.
			/// @param memory  Adresse de l'espace mémoire à ré-allouer.
			/// @param newSize Nombre d'octets à allouer pour l'espace mémoire. 
			/// @return        L'adresse vers le nouvel espace mémoire ou \c nullptr si une erreur est survenue.
			static mem_ptr realloc(mem_ptr memory, size_t newSize);

			/// @brief        Libère un espace mémoire qui a été créé sans être traqué.
			/// @param memory Adresse de l'espace mémoire à libérer.
			static void freeNoTrack(mem_ptr memory);

			/// @brief        Libère un espace mémoire qui a été créé en étant traqué.
			/// @param memory Adresse de l'espace mémoire à libérer.
			static void free(const mem_ptr memory);

	};

	using mem = MemoryManager;

	class DE_API MemoryChunk {

		public:
			MemoryChunk(mem_ptr data = nullptr, size_t size = 0);

			static bool alloc(MemoryChunk &dest, size_t size);
			void free();

			mem_ptr data();
			size_t size() const;

			void setData(mem_ptr data);
			void setSize(size_t size);

		private:
			mem_ptr m_Data;
			size_t  m_Size;

	};

	/*
	=================
	MemoryChunk::free
	=================
	*/
	inline void MemoryChunk::free()
	{
		mem::free(m_Data);
		m_Data = nullptr;
		m_Size = 0;
	}

	/*
	=================
	MemoryChunk::data
	=================
	*/
	inline mem_ptr MemoryChunk::data()
	{
		return m_Data;
	}

	/*
	=================
	MemoryChunk::size
	=================
	*/
	inline size_t MemoryChunk::size() const
	{
		return m_Size;
	}

	/*
	====================
	MemoryChunk::setData
	====================
	*/
	inline void MemoryChunk::setData(mem_ptr data)
	{
		m_Data = data;
	}

	/*
	====================
	MemoryChunk::setSize
	====================
	*/
	inline void MemoryChunk::setSize(size_t size)
	{
		m_Size = size;
	}

}

#endif