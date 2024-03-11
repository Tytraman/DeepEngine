#ifndef __DEEP_ENGINE_SYS_WIN_HPP__
#define __DEEP_ENGINE_SYS_WIN_HPP__

#include "DE/core/def.hpp"
#include "DE/core/hash_table.hpp"
#include "DE/core/list.hpp"
#include "DE/core/string.hpp"

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