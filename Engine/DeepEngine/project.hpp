#ifndef DEEP_ENGINE_PROJECT_HPP
#define DEEP_ENGINE_PROJECT_HPP

#include "DeepEngine/deep_engine_export.h"

#include <DeepLib/object.hpp>
#include <DeepLib/memory/ref_counted.hpp>
#include <DeepLib/string/string_native.hpp>
#include <DeepLib/stream/file_stream.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace deep
{
    class engine;

    class DEEP_ENGINE_API project : public object
    {
      public:
        project()                           = delete;
        project(const project &)            = delete;
        project &operator=(const project &) = delete;

        static ref<project> create(const ref<ctx> &context, string_native &folder_path, const char *name, ref<project> &current_proj) noexcept;
        static ref<project> open(const ref<ctx> &context, string_native &folder_path, ref<project> &current_proj) noexcept;

        void load_settings(ref<engine> &eng) noexcept;

        bool save_settings(const ref<engine> &eng) noexcept;

      protected:
        project(const ref<ctx> &context) noexcept;

      private:
        string_native m_folder_path;
        ref<file_stream> m_settings_stream;
        json m_settings;

      public:
        friend memory_manager;
    };
} // namespace deep

#endif
