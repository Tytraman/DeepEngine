#include "error.hpp"

#include <comdef.h>

namespace deep
{
    namespace D3D
    {
        error::error(const ref<ctx> &context, HRESULT hr) noexcept
                : object(context), m_result(hr)
        {
        }

        const char *error::get_error_name() const noexcept
        {
#define DEEP_GEN_CASE(hr_name) \
    case hr_name:              \
        return #hr_name;

            switch (m_result)
            {
                default:
                    return "UNKNOWN";
                    DEEP_GEN_CASE(D3D11_ERROR_FILE_NOT_FOUND)
                    DEEP_GEN_CASE(D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS)
                    DEEP_GEN_CASE(D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS)
                    DEEP_GEN_CASE(D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD)
                    DEEP_GEN_CASE(DXGI_ERROR_INVALID_CALL)
                    DEEP_GEN_CASE(DXGI_ERROR_WAS_STILL_DRAWING)
                    DEEP_GEN_CASE(E_FAIL)
                    DEEP_GEN_CASE(E_INVALIDARG)
                    DEEP_GEN_CASE(E_OUTOFMEMORY)
                    DEEP_GEN_CASE(E_NOTIMPL)
                    DEEP_GEN_CASE(S_FALSE)
                    DEEP_GEN_CASE(S_OK)
                    DEEP_GEN_CASE(DXGI_ERROR_DEVICE_HUNG)
                    DEEP_GEN_CASE(DXGI_ERROR_DEVICE_REMOVED)
                    DEEP_GEN_CASE(DXGI_ERROR_DEVICE_RESET)
                    DEEP_GEN_CASE(DXGI_ERROR_DRIVER_INTERNAL_ERROR)
                    DEEP_GEN_CASE(E_NOINTERFACE)
            }

#undef DEEP_GEN_CASE
        }

        string error::get_error_string() const noexcept
        {
            _com_error err = _com_error(m_result);

            return string(get_context(), err.ErrorMessage());
        }
    } // namespace D3D
} // namespace deep
