#include "DeepEngine/Scripting/dot_net_host.hpp"

namespace deep
{
    const native_char *dot_net_host::g_managed_assembly = DEEP_TEXT_NATIVE("DeepManaged.dll");

    dot_net_host::dot_net_host()
            : m_lib(nullptr),
              m_host_handle(nullptr),
              m_init_fn(nullptr),
              m_get_runtime_delegate_fn(nullptr),
              m_close_fn(nullptr),
              m_load_assembly_and_get_function_pointer_fn(nullptr),
              m_initialize_fn(nullptr),
              m_shutdown_fn(nullptr)
    {
    }

    bool dot_net_host::init(const native_char *config_path) noexcept
    {
        static const native_char *managed_entry_type_name = DEEP_TEXT_NATIVE("DeepManaged.EntryPoint, DeepManaged");

        if (!load_hostfxr())
        {
            return false;
        }

        int rc = m_init_fn(config_path, nullptr, &m_host_handle);

        if (rc != 0)
        {
            return false;
        }

        rc = m_get_runtime_delegate_fn(m_host_handle, hdt_load_assembly_and_get_function_pointer, (void **) &m_load_assembly_and_get_function_pointer_fn);

        if (rc != 0 || m_load_assembly_and_get_function_pointer_fn == nullptr)
        {
            return false;
        }

        m_initialize_fn = get_function<initialize_fn>(managed_entry_type_name, DEEP_TEXT_NATIVE("Initialize"));
        m_shutdown_fn   = get_function<shutdown_fn>(managed_entry_type_name, DEEP_TEXT_NATIVE("Shutdown"));

        if (m_initialize_fn == nullptr ||
            m_shutdown_fn == nullptr)
        {
            return false;
        }

        if (m_initialize_fn() != 0)
        {
            return false;
        }

        return true;
    }

    void dot_net_host::shutdown() noexcept
    {
        if (m_shutdown_fn != nullptr)
        {
            m_shutdown_fn();
        }

        if (m_host_handle != nullptr && m_close_fn != nullptr)
        {
            m_close_fn(m_host_handle);
        }

        if (m_lib != nullptr)
        {
            FreeLibrary(m_lib);
        }

        m_lib         = nullptr;
        m_host_handle = nullptr;

        m_init_fn                                   = nullptr;
        m_get_runtime_delegate_fn                   = nullptr;
        m_close_fn                                  = nullptr;
        m_load_assembly_and_get_function_pointer_fn = nullptr;

        m_initialize_fn = nullptr;
        m_shutdown_fn   = nullptr;
    }

    bool dot_net_host::load_hostfxr() noexcept
    {
        char_t buffer[MAX_PATH];
        size_t buffer_size = sizeof(buffer) / sizeof(char_t);

        int rc = get_hostfxr_path(buffer, &buffer_size, nullptr);

        if (rc != 0)
        {
            return false;
        }

        m_lib = LoadLibraryW(buffer);

        if (m_lib == nullptr)
        {
            return false;
        }

        m_init_fn                 = (hostfxr_initialize_for_runtime_config_fn) GetProcAddress(m_lib, "hostfxr_initialize_for_runtime_config");
        m_get_runtime_delegate_fn = (hostfxr_get_runtime_delegate_fn) GetProcAddress(m_lib, "hostfxr_get_runtime_delegate");
        m_close_fn                = (hostfxr_close_fn) GetProcAddress(m_lib, "hostfxr_close");

        return (m_init_fn != nullptr &&
                m_get_runtime_delegate_fn != nullptr &&
                m_close_fn != nullptr);
    }
} // namespace deep
