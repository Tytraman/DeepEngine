#ifndef DEEP_ENGINE_D3D_VERTEX_BUFFER_HPP
#define DEEP_ENGINE_D3D_VERTEX_BUFFER_HPP

#include "deep_d3d_export.h"
#include "D3D/resource.hpp"

namespace deep
{
    namespace D3D
    {
        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11Buffer>;

        class DEEP_D3D_API vertex_buffer : public resource
        {
          public:
            vertex_buffer()                                 = delete;
            vertex_buffer(const vertex_buffer &)            = delete;
            vertex_buffer &operator=(const vertex_buffer &) = delete;

            virtual void bind() noexcept override;
            virtual void draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context) noexcept override;

            virtual bool can_draw() const noexcept override;

          protected:
            Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

          protected:
            using resource::resource;

          public:
            friend class resource_factory;
        };
    } // namespace D3D
} // namespace deep

#endif
