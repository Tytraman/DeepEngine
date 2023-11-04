#ifndef __DEEP_ENGINE_SYS_WIN_HPP__
#define __DEEP_ENGINE_SYS_WIN_HPP__

#include <DE/def.hpp>
#include <DE/memory/hash_table.hpp>
#include <DE/memory/list.hpp>
#include <DE/string.hpp>

#if DE_WINDOWS

DE_API BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

namespace de
{

	struct local_thread_var
    {
		list<string> functionCallbackList;
		hash_table<size_t> functionCallNumber;
	};
	
	class DE_API sys_win
    {

		friend BOOL WINAPI ::DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

		private:
			static DWORD m_TLSIndex;

		public:
			static local_thread_var *getLocalThreadVar(bool createIfNotExisting = false);
			static local_thread_var *createLocalThreadVar();

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