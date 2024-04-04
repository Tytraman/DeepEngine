#ifndef __DEEP_ENGINE_SYS_WIN_HPP__
#define __DEEP_ENGINE_SYS_WIN_HPP__

#include "core/def.hpp"
#include "core/templates/hash_table.hpp"
#include "core/templates/list.hpp"
#include "core/string/string.hpp"

#if DE_WINDOWS

#include <crtdbg.h>

DE_API BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

namespace deep
{
    
    class DE_API sys_win
    {

        friend BOOL WINAPI ::DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

        private:
            static DWORD m_TLSIndex;

        public:
            static DWORD getTlsIndex();

        public:
            sys_win() = delete;

    };

    inline DWORD sys_win::getTlsIndex() 
    {
        return m_TLSIndex;
    }

}

#endif	// DE_WINDOWS

#endif