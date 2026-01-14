#ifndef DEEP_ENGINE_D3D_VERTEX_BUFFER_HPP
#define DEEP_ENGINE_D3D_VERTEX_BUFFER_HPP

#include "deep_d3d_export.h"
#include "D3D/resource.hpp"

#include <DeepLib/object.hpp>

namespace deep
{
    namespace D3D
    {
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11Buffer>;

        class DEEP_D3D_API vertex_buffer : public object
        {
          public:
            vertex_buffer()                                 = delete;
            vertex_buffer(const vertex_buffer &)            = delete;
            vertex_buffer &operator=(const vertex_buffer &) = delete;

            ID3D11Buffer *get() const noexcept;
            ID3D11Buffer *const *get_address() const noexcept;

          protected:
            Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
            uint32 m_stride;
            uint32 m_offset;

          protected:
            using object::object;

          public:
            friend class resource_factory;
            friend class device_context;
        };
    } // namespace D3D
} // namespace deep

#endif
