#ifndef __DEEP_ENGINE_COM_HPP__
#define __DEEP_ENGINE_COM_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/string/string.hpp"

#if DE_WINDOWS
// Copié-collé de la doc Microsoft : https://learn.microsoft.com/en-us/windows/win32/wmisdk/creating-a-connection-to-a-wmi-namespace
#define _WIN32_DCOM
#include <windows.h>
#include <comdef.h>
#include <wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#endif

namespace deep
{

    class COM
    {

        public:
            DE_API static COM *get_singleton();

            DE_API bool init();
            DE_API void shutdown();

            DE_API bool query_cpu_info(
                                    string &destName,
                                    uint16_t *architecture,
                                    uint8_t *addressWidth);

        private:
            COM();

#if DE_WINDOWS
            IWbemLocator *m_Loc;
            IWbemServices *m_SVC;
#endif


    };

}

#endif