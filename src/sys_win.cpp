#include <DE/def.h>

#if DE_WINDOWS

#include <DE/sys_win.hpp>

DWORD de::SysWin::_tlsIndex = TLS_OUT_OF_INDEXES;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    de::LocalThreadVar *local;

    switch(fdwReason) {
        default: break;
        case DLL_PROCESS_ATTACH: {
            de::SysWin::_tlsIndex = TlsAlloc();

            if(de::SysWin::_tlsIndex == TLS_OUT_OF_INDEXES)
                return FALSE;
        }
        case DLL_THREAD_ATTACH: {
            local = de::SysWin::createLocalThreadVar();
            local->functionCallbackList = new std::vector<std::string>();
            local->functionCallNumber = new std::unordered_map<std::string, size_t>();
        } break;
        case DLL_THREAD_DETACH: {
            local = de::SysWin::getLocalThreadVar();

            if(local != NULL) {
                delete local->functionCallbackList;
                delete local->functionCallNumber;
                LocalFree((HLOCAL) local);
            }
        } break;
        case DLL_PROCESS_DETACH: {
            local = de::SysWin::getLocalThreadVar();

            if(local != NULL) {
                delete local->functionCallbackList;
                delete local->functionCallNumber;
                LocalFree((HLOCAL) local);
            }

            TlsFree(de::SysWin::_tlsIndex);
        } break;
    }

    return TRUE;
}

de::LocalThreadVar *de::SysWin::getLocalThreadVar(bool createIfNotExisting) {
    LPVOID data = TlsGetValue(_tlsIndex);

    if(data == NULL) {
        if(!createIfNotExisting)
            return NULL;

        data = createLocalThreadVar();
        if(data == NULL)
            return NULL;
    }

    return (LocalThreadVar *) data;
}

de::LocalThreadVar *de::SysWin::createLocalThreadVar() {
    LPVOID data = LocalAlloc(LPTR, sizeof(LocalThreadVar));

    if(data == NULL)
        return NULL;

    if(!TlsSetValue(_tlsIndex, data)) {
        LocalFree((HLOCAL) data);
        return NULL;
    }

    return (LocalThreadVar *) data;
}

#endif  // DE_WINDOWS
