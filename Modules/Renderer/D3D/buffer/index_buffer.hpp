#ifndef DEEP_ENGINE_D3D_INDEX_BUFFER_HPP
#define DEEP_ENGINE_D3D_INDEX_BUFFER_HPP

#include "deep_d3d_export.h"
#include <DeepLib/object.hpp>
#include <d3d11.h>
#include <wrl.h>

namespace deep
{
    namespace D3D
    {
#ifndef DEEP_D3D_BUFFER
#define DEEP_D3D_BUFFER
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11Buffer>;
#endif

        class DEEP_D3D_API index_buffer : public object
        {
          public:
            index_buffer()                                = delete;
            index_buffer(const index_buffer &)            = delete;
            index_buffer &operator=(const index_buffer &) = delete;

            ID3D11Buffer *get() const noexcept;
            ID3D11Buffer *const *get_address() const noexcept;

            uint16 count() const noexcept;

          protected:
            Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
            uint16 m_count;

          protected:
            using object::object;

          public:
            friend class resource_factory;
            friend class device_context;
        };
    } // namespace D3D
} // namespace deep

#endif
