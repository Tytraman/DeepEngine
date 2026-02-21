#ifndef DEEP_ENGINE_D3D_ERROR_HPP
#define DEEP_ENGINE_D3D_ERROR_HPP

#include "deep_d3d_export.h"
#include <DeepLib/object.hpp>
#include <DeepLib/string/string.hpp>

#include <d3d11.h>

namespace deep
{
    namespace D3D
    {
        class DEEP_D3D_API error : public object
        {
          public:
            error() = delete;
            error(const ref<ctx> &context, HRESULT hr) noexcept;

            constexpr HRESULT get_result() const noexcept;
            const char *get_error_name() const noexcept;
            string get_error_string() const noexcept;

          private:
            HRESULT m_result;
        };

        inline constexpr HRESULT error::get_result() const noexcept
        {
            return m_result;
        }
    } // namespace D3D
} // namespace deep

#ifdef _DEBUG
#define DEEP_DX_CHECK(x, _ref_context, _device)                                \
    {                                                                          \
        HRESULT hr = (x);                                                      \
        if (FAILED(hr))                                                        \
        {                                                                      \
            if (hr == DXGI_ERROR_DEVICE_REMOVED)                               \
            {                                                                  \
                HRESULT reason       = _device->GetDeviceRemovedReason();      \
                deep::D3D::error err = deep::D3D::error(_ref_context, reason); \
                _ref_context->out() << err.get_error_name() << ": "            \
                                    << *err.get_error_string() << "\r\n";      \
            }                                                                  \
            else                                                               \
            {                                                                  \
                deep::D3D::error err = deep::D3D::error(_ref_context, hr);     \
                _ref_context->out() << err.get_error_name() << ": "            \
                                    << *err.get_error_string() << "\r\n";      \
            }                                                                  \
            DebugBreak();                                                      \
        }                                                                      \
    }
#else
#define DEEP_DX_CHECK(x, _ref_context, _device) (x);
#endif

#endif
