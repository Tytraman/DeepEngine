#ifndef DEEP_ENGINE_IMGUI_CHAT_HPP
#define DEEP_ENGINE_IMGUI_CHAT_HPP

#include "DeepEngine/deep_engine_export.h"
#include "DeepEngine/GUI/imgui_drawable.hpp"

#include <DeepLib/string/string.hpp>
#include <DeepLib/collection/list.hpp>

#include <imgui.h>

namespace deep
{
    class DEEP_ENGINE_API imgui_chat : public imgui_drawable
    {
      public:
        struct message
        {
            deep::string text;
            ImVec4 color;
        };

      public:
        imgui_chat()                              = delete;
        imgui_chat(const imgui_chat &)            = delete;
        imgui_chat &operator=(const imgui_chat &) = delete;

        virtual void draw() override;

      protected:
        list<message> m_history;

      protected:
        imgui_chat(const ref<ctx> &context, bool enabled) noexcept;

      public:
        friend class memory_manager;
    };
} // namespace deep

#endif
