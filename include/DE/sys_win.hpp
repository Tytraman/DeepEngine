#ifndef __DEEP_ENGINE_SYS_WIN_HPP__
#define __DEEP_ENGINE_SYS_WIN_HPP__

#include <DE/def.h>
#include <string>
#include <vector>
#include <unordered_map>

#if DE_WINDOWS

DE_API BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

namespace de {

	struct LocalThreadVar {
		std::vector<std::string> *functionCallbackList;
		std::unordered_map<std::string, size_t> *functionCallNumber;
	};
	
	class DE_API SysWin {

		friend BOOL WINAPI ::DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

		private:
			static DWORD m_TLSIndex;

		public:
			static LocalThreadVar *getLocalThreadVar(bool createIfNotExisting = false);
			static LocalThreadVar *createLocalThreadVar();

			static DWORD getTlsIndex();

	};

	inline DWORD SysWin::getTlsIndex() {
		return m_TLSIndex;
	}

}

#endif	// DE_WINDOWS

#endif