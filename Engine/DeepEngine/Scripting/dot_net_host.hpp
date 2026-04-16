#ifndef DEEP_ENGINE_DOT_NET_HOST_HPP
#define DEEP_ENGINE_DOT_NET_HOST_HPP

#include "DeepEngine/deep_engine_export.h"
#include <DeepCore/types.hpp>

#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>

namespace deep
{
    class DEEP_ENGINE_API dot_net_host
    {
      public:
        using initialize_fn = int32(__stdcall *)();
        using shutdown_fn   = void(__stdcall *)();

      public:
        dot_net_host();

        dot_net_host(const dot_net_host &)            = delete;
        dot_net_host &operator=(const dot_net_host &) = delete;

        bool init(const native_char *config_path) noexcept;
        void shutdown() noexcept;

        template <typename TFunc>
        TFunc get_function(const native_char *type_name, const native_char *method_name) const noexcept;

      private:
        bool load_hostfxr() noexcept;

      protected:
        HMODULE m_lib;
        hostfxr_handle m_host_handle;
        hostfxr_initialize_for_runtime_config_fn m_init_fn;
        hostfxr_get_runtime_delegate_fn m_get_runtime_delegate_fn;
        hostfxr_close_fn m_close_fn;
        load_assembly_and_get_function_pointer_fn m_load_assembly_and_get_function_pointer_fn;

      protected:
        initialize_fn m_initialize_fn;
        shutdown_fn m_shutdown_fn;

      private:
        static const native_char *g_managed_assembly;
    };

    template <typename TFunc>
    inline TFunc dot_net_host::get_function(const native_char *type_name, const native_char *method_name) const noexcept
    {
        void *func_ptr = nullptr;

        if (m_load_assembly_and_get_function_pointer_fn == nullptr)
        {
            return nullptr;
        }

        int rc = m_load_assembly_and_get_function_pointer_fn(
                g_managed_assembly,
                type_name,
                method_name,
                UNMANAGEDCALLERSONLY_METHOD,
                nullptr,
                &func_ptr);

        if (rc != 0 || func_ptr == nullptr)
        {
            return nullptr;
        }

        return reinterpret_cast<TFunc>(func_ptr);
    }
} // namespace deep

#endif
