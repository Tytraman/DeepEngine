#include "D3D/texture.hpp"

namespace deep
{
    namespace D3D
    {
        ID3D11ShaderResourceView *texture::get() const
        {
            return m_texture_view.Get();
        }
    } // namespace D3D
} // namespace deep
