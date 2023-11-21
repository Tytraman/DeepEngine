#ifndef __DEEP_ENGINE_MUTEX_HPP__
#define __DEEP_ENGINE_MUTEX_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>

namespace de
{

#if DE_WINDOWS

    using mutex_handle = HANDLE;

    static constexpr mutex_handle invalid_mutex_handle = nullptr;

#else
#error Need implementation
#endif


    class DE_API mutex
    {

        public:
            static mutex_handle create(const char *name);
            static void close(mutex_handle mut);

        public:
            mutex() = delete;

    };

    inline void mutex::close(mutex_handle mut)
    {
#if DE_WINDOWS
        ReleaseMutex(mut);
        CloseHandle(mut);
#else
#error Need implementation
#endif
    }

}

#endif