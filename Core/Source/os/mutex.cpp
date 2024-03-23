#include "os/mutex.hpp"

namespace deep
{

    mutex_handle mutex::create(const char *name)
    {
        mutex_handle handle;

#if DE_WINDOWS
        handle = CreateMutexA(nullptr, true, name);
        if(GetLastError() != ERROR_SUCCESS)
            return invalid_mutex_handle;
#else
#error Need implementation
#endif
        
        return handle;
    }


}
