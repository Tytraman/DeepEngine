#ifndef __DEEP_ENGINE_DEEP_ENGINE_HPP__
#define __DEEP_ENGINE_DEEP_ENGINE_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/common.hpp"
#include "core/core.hpp"
#include "core/ref_counted.hpp"
#include "core/safe_integer.hpp"
#include "core/memory.hpp"
#include "core/camera.hpp"

#include "core/templates/collection.hpp"
#include "core/templates/hash_table.hpp"
#include "core/templates/list.hpp"
#include "core/templates/pair.hpp"
#include "core/templates/stack.hpp"

#include "core/string/string_utils.hpp"
#include "core/string/string.hpp"

#include "core/resources/resource_manager.hpp"
#include "core/resources/resource.hpp"
#include "core/resources/resource_loader.hpp"
#include "core/resources/image.hpp"
#include "core/resources/text.hpp"
#include "core/resources/bmp_loader.hpp"
#include "core/resources/text_loader.hpp"

#include "core/config/settings.hpp"

#include "io/stream.hpp"
#include "io/memory_stream.hpp"
#include "io/file_stream.hpp"
#include "io/fd_stream.hpp"
#include "io/text_writer.hpp"
#include "io/stream_writer.hpp"
#include "io/string_writer.hpp"

#include "maths/vec.hpp"
#include "maths/mat.hpp"

#include "file/file.hpp"
#include "file/file_object.hpp"

#include "audio/audio.hpp"

#include "ecs/ecs.hpp"
#include "ecs/entity.hpp"
#include "ecs/component.hpp"
#include "ecs/system.hpp"
#include "ecs/scene.hpp"

#include "graphics/graphic.hpp"
#include "graphics/renderer.hpp"
#include "graphics/material.hpp"
#include "graphics/shape.hpp"

#include "gui/deimgui.hpp"

#include "hardware/cpu.hpp"

#include "image/image.hpp"
#include "image/bmp.hpp"
#include "image/png.hpp"

#include "os/sys_win.hpp"
#include "os/win32.hpp"
#include "os/window.hpp"
#include "os/file_dialog.hpp"
#include "os/COM.hpp"
#include "os/mutex.hpp"
#include "os/key.hpp"
#include "os/events.hpp"

#endif