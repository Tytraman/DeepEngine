#ifndef DEEP_ENGINE_D3D_GRAPHICS_HPP
#define DEEP_ENGINE_D3D_GRAPHICS_HPP

#include "deep_d3d_export.h"
#include <DeepCore/types.hpp>
#include <d3d11.h>

namespace deep
{
    namespace D3D
    {
        class DEEP_D3D_API graphics
        {
          public:
            graphics()                            = delete;
            graphics(const graphics &)            = delete;
            graphics &operator=(const graphics &) = delete;

            static graphics create(window_handle win) noexcept;

            void destroy() noexcept;

          protected:
            graphics(window_handle win) noexcept;

          private:
            window_handle m_window_handle;
            ID3D11Device *m_device;
            IDXGISwapChain *m_swap_chain;
            ID3D11DeviceContext *m_device_context;
        };
    } // namespace D3D
} // namespace deep

#endif
