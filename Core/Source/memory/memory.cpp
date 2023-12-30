#include <DE/memory/memory.hpp>
#include <DE/memory/list.hpp>

#include <stdio.h>

namespace deep
{

    list<mem_ptr> memory_manager::g_MemoryTrack;

    /*
    ============================
    memory_manager::allocNoTrack
    ============================
    */
    mem_ptr memory_manager::allocNoTrack(size_t size)
    {

#if DE_WINDOWS
        HANDLE processHeap = GetProcessHeap();

        if(processHeap == NULL)
            return nullptr;

        return HeapAlloc(processHeap, HEAP_ZERO_MEMORY, size);
#else
#error Implementation needed
#endif
    }

    /*
    =====================
    memory_manager::alloc
    =====================
    */
    mem_ptr memory_manager::alloc(size_t size)
    {
        mem_ptr memory = allocNoTrack(size);

        if(memory == nullptr)
            return nullptr;

        g_MemoryTrack.add(memory);
        return memory;
    }

    /*
    ==============================
    memory_manager::reallocNoTrack
    ==============================
    */
    mem_ptr memory_manager::reallocNoTrack(mem_ptr memory, size_t newSize)
    {
#if DE_WINDOWS
        if(memory == nullptr)
            return allocNoTrack(newSize);

        HANDLE processHeap = GetProcessHeap();

        if(processHeap == NULL)
            return nullptr;
        
        return HeapReAlloc(processHeap, HEAP_ZERO_MEMORY, memory, newSize);
#else

#endif
    }

    /*
    =======================
    memory_manager::realloc
    =======================
    */
    mem_ptr memory_manager::realloc(mem_ptr memory, size_t newSize)
    {
        mem_ptr mem = reallocNoTrack(memory, newSize);

        if(mem == nullptr)
            return nullptr;

        size_t index = g_MemoryTrack.find(memory);

        if(index != list<mem_ptr>::nothing)
            g_MemoryTrack[index] = mem;

        return mem;
    }

    /*
    ===========================
    memory_manager::freeNoTrack
    ===========================
    */
    void memory_manager::freeNoTrack(mem_ptr memory)
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
    ====================
    memory_manager::free
    ====================
    */
    void memory_manager::free(const mem_ptr memory)
    {
        size_t index = g_MemoryTrack.find(memory);
        if(index != nothing)
            g_MemoryTrack.remove(index);

        freeNoTrack(memory);
    }

    /*
    ==========================
    memory_chunk::memory_chunk
    ==========================
    */
    memory_chunk::memory_chunk(mem_ptr data, size_t size)
        : m_Data(data),
          m_Size(size)
    { }

    /*
    ==========================
    memory_chunk::memory_chunk
    ==========================
    */
    memory_chunk::memory_chunk(const memory_chunk &other)
    {
        m_Data = mem::alloc(other.m_Size);

        if(m_Data != nullptr)
        {
            m_Size = other.m_Size;
            memcpy(m_Data, other.m_Data, m_Size);
        }
        else
            m_Size = 0;
    }

    /*
    ====================
    memory_chunk::create
    ====================
    */
    bool memory_chunk::alloc(memory_chunk &dest, size_t size)
    {
        mem_ptr data = mem::alloc(size);

        if(data == nullptr)
            return false;

        dest.m_Data = data;
        dest.m_Size = size;

        return true;
    }

}
