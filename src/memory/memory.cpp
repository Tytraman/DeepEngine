#include <DE/memory/memory.hpp>
#include <DE/memory/list.hpp>

namespace de {

	List g_MemoryTrack(sizeof(mem_ptr));

	/*
	===========================
	MemoryManager::allocNoTrack
	===========================
	*/
	mem_ptr MemoryManager::allocNoTrack(size_t size)
	{
#if DE_WINDOWS
		HANDLE processHeap = GetProcessHeap();

		if(processHeap == NULL)
			return nullptr;

		return HeapAlloc(processHeap, 0, size);
#else
#error Implementation needed
#endif
	}

	/*
	====================
	MemoryManager::alloc
	====================
	*/
	mem_ptr MemoryManager::alloc(size_t size)
	{
		mem_ptr memory = allocNoTrack(size);

		if(memory == nullptr)
			return nullptr;

		g_MemoryTrack.addCopy(&memory);
		return memory;
	}

	/*
	=============================
	MemoryManager::reallocNoTrack
	=============================
	*/
	mem_ptr MemoryManager::reallocNoTrack(mem_ptr memory, size_t newSize)
	{
#if DE_WINDOWS
		if(memory == nullptr)
			return allocNoTrack(newSize);

		HANDLE processHeap = GetProcessHeap();

		if(processHeap == NULL)
			return nullptr;
		
		return HeapReAlloc(processHeap, 0, memory, newSize);
#else

#endif
	}

	/*
	======================
	MemoryManager::realloc
	======================
	*/
	mem_ptr MemoryManager::realloc(mem_ptr memory, size_t newSize)
	{
		mem_ptr mem = reallocNoTrack(memory, newSize);

		if(memory == nullptr)
			return nullptr;

		size_t index = g_MemoryTrack.find(memory);

		if(index != nothing)
			g_MemoryTrack.setCopy(index, &mem);

		return mem;
	}

	/*
	==========================
	MemoryManager::freeNoTrack
	==========================
	*/
	void MemoryManager::freeNoTrack(mem_ptr memory)
	{
#if DE_WINDOWS
		HANDLE processHeap = GetProcessHeap();

		if(processHeap == NULL)
			return;

		HeapFree(processHeap, 0, memory);
#else

#endif
	}

	/*
	===================
	MemoryManager::free
	===================
	*/
	void MemoryManager::free(const mem_ptr memory)
	{
		size_t index = g_MemoryTrack.find(memory);
		if(index != nothing)
			g_MemoryTrack.remove(index);

		freeNoTrack(memory);
	}

	/*
	========================
	MemoryChunk::MemoryChunk
	========================
	*/
	MemoryChunk::MemoryChunk(mem_ptr data, size_t size)
		: m_Data(data),
		  m_Size(size)
	{ }

	/*
	===================
	MemoryChunk::create
	===================
	*/
	bool MemoryChunk::alloc(MemoryChunk &dest, size_t size)
	{
		mem_ptr data = mem::alloc(size);

		if(data == nullptr)
			return false;

		dest.m_Data = data;
		dest.m_Size = size;

		return true;
	}

}
