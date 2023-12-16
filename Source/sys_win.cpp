#include <DE/def.hpp>

#if DE_WINDOWS

#include <DE/sys_win.hpp>

namespace deep
{
    DWORD sys_win::m_TLSIndex  = TLS_OUT_OF_INDEXES;
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) 
{
    switch(fdwReason)
    {
        default: 
            break;

        // Quand la DLL est attachée à un processus.
        case DLL_PROCESS_ATTACH: 
        {
            deep::sys_win::m_TLSIndex = TlsAlloc();

            if(deep::sys_win::m_TLSIndex == TLS_OUT_OF_INDEXES)
                return FALSE;

            [[fallthrough]];
        }
        // Quand on un nouveau thread est attaché.
        case DLL_THREAD_ATTACH:
        {

        } break;
        case DLL_THREAD_DETACH:
        {

        } break;
        // Quand la DLL est détachée du processus.
        case DLL_PROCESS_DETACH:
        {

        } break;
    }

    return TRUE;
}

#endif  // DE_WINDOWS
