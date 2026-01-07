#include "shader_factory.hpp"
#include "D3D/error.hpp"
#include <DeepLib/memory/memory.hpp>

namespace deep
{
    namespace D3D
    {
        ref<vertex_shader> shader_factory::create_vertex_shader(const ref<ctx> &context, stream *input, const D3D11_INPUT_ELEMENT_DESC *ied, uint32 ied_count, Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept
        {
            vertex_shader *vs = mem::alloc_type<vertex_shader>(context.get(), context);

            if (vs == nullptr)
            {
                return ref<vertex_shader>();
            }

            usize bytes_size = input->get_length();
            usize bytes_read;

            uint8 *buff = mem::alloc<uint8>(context.get(), bytes_size);

            if (buff == nullptr)
            {
                mem::dealloc_type(context.get_memory_manager(), vs);

                return ref<vertex_shader>();
            }

            if (!input->read(buff, bytes_size, &bytes_read))
            {
                mem::dealloc(context.get(), buff);
                mem::dealloc_type(context.get_memory_manager(), vs);

                return ref<vertex_shader>();
            }

            DEEP_DX_CHECK(device->CreateVertexShader(buff, bytes_read, nullptr, &vs->m_shader), context, device)
            DEEP_DX_CHECK(device->CreateInputLayout(ied, ied_count, buff, bytes_read, &vs->m_input_layout), context, device)

            mem::dealloc(context.get(), buff);

            return ref<vertex_shader>(context.get(), vs);
        }
    } // namespace D3D
} // namespace deep
