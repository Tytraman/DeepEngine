#include <DE/def.hpp>

#if DE_WINDOWS

#include <DE/sys_win.hpp>

namespace de
{
    DWORD sys_win::m_TLSIndex  = TLS_OUT_OF_INDEXES;

    local_thread_var *sys_win::getLocalThreadVar(bool createIfNotExisting) 
    {
        LPVOID data = TlsGetValue(m_TLSIndex);

        if(data == NULL) 
        {
            if(!createIfNotExisting)
                return NULL;

            data = createLocalThreadVar();
            if(data == NULL)
                return NULL;
        }

        return (local_thread_var *) data;
    }

    local_thread_var *sys_win::createLocalThreadVar()
    {
        local_thread_var *data = static_cast<local_thread_var *>(LocalAlloc(LPTR, sizeof(local_thread_var)));

        if(data == NULL)
            return NULL;

        data->functionCallbackList.init();
        data->functionCallNumber.init();

        if(!TlsSetValue(m_TLSIndex, data)) 
        {
            LocalFree((HLOCAL) data);
            return NULL;
        }

        return (local_thread_var *) data;
    }
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) 
{
    de::local_thread_var *local;

    switch(fdwReason)
    {
        default: 
            break;

        // Quand la DLL est attachée à un processus.
        case DLL_PROCESS_ATTACH: 
        {
            de::sys_win::m_TLSIndex = TlsAlloc();

            if(de::sys_win::m_TLSIndex == TLS_OUT_OF_INDEXES)
                return FALSE;

            [[fallthrough]];
        }
        // Quand on un nouveau thread est attaché.
        case DLL_THREAD_ATTACH:
        {
            local = de::sys_win::createLocalThreadVar();
        } break;
        case DLL_THREAD_DETACH:
        {
            local = de::sys_win::getLocalThreadVar();

            if(local != NULL)
            {
                LocalFree((HLOCAL) local);
            }
        } break;
        // Quand la DLL est détachée du processus.
        case DLL_PROCESS_DETACH:
        {
            local = de::sys_win::getLocalThreadVar();

            if(local != NULL)
            {
                LocalFree((HLOCAL) local);
            }

            TlsFree(de::sys_win::m_TLSIndex);
        } break;
    }

    return TRUE;
}

#endif  // DE_WINDOWS
