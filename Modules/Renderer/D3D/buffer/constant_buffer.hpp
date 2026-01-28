#ifndef DEEP_ENGINE_D3D_CONSTANT_BUFFER_HPP
#define DEEP_ENGINE_D3D_CONSTANT_BUFFER_HPP

#include "deep_d3d_export.h"
#include "D3D/resource.hpp"

namespace deep
{
    namespace D3D
    {
        class device_context;

        template class DEEP_D3D_API Microsoft::WRL::ComPtr<ID3D11Buffer>;

        class DEEP_D3D_API constant_buffer : public resource
        {
          public:
            constant_buffer()                                   = delete;
            constant_buffer(const constant_buffer &)            = delete;
            constant_buffer &operator=(const constant_buffer &) = delete;

            virtual void bind() noexcept override;
            virtual void draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context) noexcept override;

            void update(const void *data, const device_context &dc) noexcept;

            virtual bool can_draw() const noexcept override;

            ID3D11Buffer *get() const noexcept;
            ID3D11Buffer *const *get_address() const noexcept;

            uint32 get_bytes_size() const noexcept;

          protected:
            Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
            uint32 m_bytes_size;

          protected:
            using resource::resource;

          public:
            friend class resource_factory;
        };
    } // namespace D3D
} // namespace deep

#endif
