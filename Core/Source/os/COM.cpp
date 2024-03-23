#include "os/COM.hpp"
#include "core/string_utils.hpp"
#include "core/list.hpp"

namespace deep
{

    COM::COM()
        : m_Loc(nullptr),
          m_SVC(nullptr)
    { }

    COM *COM::get_singleton()
    {
        static COM singleton;

        return &singleton;
    }

    bool COM::init()
    {
#if DE_WINDOWS
        HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if(result != S_OK)
            return false;


        result = CoInitializeSecurity(
                                    nullptr,
                                    -1,
                                    nullptr,
                                    nullptr,
                                    RPC_C_AUTHN_LEVEL_DEFAULT,
                                    RPC_C_IMP_LEVEL_IMPERSONATE,
                                    nullptr,
                                    EOAC_NONE,
                                    nullptr);
        if(result != S_OK)
        {
            CoUninitialize();
            return false;
        }

        result = CoCreateInstance(
                                CLSID_WbemLocator,
                                nullptr,
                                CLSCTX_INPROC_SERVER,
                                IID_IWbemLocator,
                                (LPVOID *) &m_Loc);
        if(result != S_OK)
        {
            CoUninitialize();
            return false;
        }

        result = m_Loc->ConnectServer(
                                    BSTR(L"ROOT\\cimv2"),    //namespace
                                    nullptr,                 // User name 
                                    nullptr,                 // User password
                                    0,                       // Locale 
                                    0,                       // Security flags
                                    0,                       // Authority 
                                    0,                       // Context object 
                                    &m_SVC);                 // IWbemServices proxy
        if(result != S_OK)
        {
            m_Loc->Release();
            CoUninitialize();
            return false;
        }

        result = CoSetProxyBlanket(
                                m_SVC,
                                RPC_C_AUTHN_WINNT,
                                RPC_C_AUTHZ_NONE,
                                nullptr,
                                RPC_C_AUTHN_LEVEL_CALL,
                                RPC_C_IMP_LEVEL_IMPERSONATE,
                                nullptr,
                                EOAC_NONE
                            );
        if(result != S_OK)
        {
            m_SVC->Release();
            m_Loc->Release();
            CoUninitialize();
            return false;
        }
#endif

        return true;
    }

    void COM::shutdown()
    {
#if DE_WINDOWS
        if(m_SVC != nullptr)
            m_SVC->Release();
        if(m_Loc != nullptr)
            m_Loc->Release();

        CoUninitialize();
#endif
    }

    bool COM::query_cpu_info(string &destName, uint16_t *architecture, uint8_t *addressWidth)
    {
#if DE_WINDOWS
        IEnumWbemClassObject *enumerator = nullptr;
        HRESULT result = m_SVC->ExecQuery(BSTR(L"WQL"), BSTR(L"SELECT * FROM Win32_Processor"), WBEM_FLAG_FORWARD_ONLY, nullptr, &enumerator);
        if(result != S_OK)
            return false;

        ULONG count;

        IWbemClassObject *obj = nullptr;

        result = enumerator->Next(WBEM_INFINITE, 1, &obj, &count);
        if(result != S_OK)
        {
            enumerator->Release();
            return false;
        }

        _variant_t val;
        result = obj->Get(BSTR(L"Name"), 0, &val, nullptr, nullptr);
        
        size_t len = string_utils::length(val.bstrVal);
        char *dest = (char *) mem::alloc(len + 1);
        string_utils::wchar_to_char(dest, val.bstrVal);
        destName.set_length(len);
        destName.set_ptr(dest);
        destName.rtrim(' ');

        VariantClear(&val);

        if(architecture != nullptr)
        {
            result = obj->Get(BSTR(L"Architecture"), 0, &val, nullptr, nullptr);
            *architecture = static_cast<uint16_t>(val.uintVal);
            VariantClear(&val);
        }

        if(addressWidth != nullptr)
        {
            result = obj->Get(BSTR(L"AddressWidth"), 0, &val, nullptr, nullptr);
            *addressWidth = static_cast<uint8_t>(val.uintVal);
            VariantClear(&val);
        }

        obj->Release();
        enumerator->Release();
#else
#error Need implementation.
#endif

        return true;
    }

}
