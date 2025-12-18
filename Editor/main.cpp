#include "DeepEngine/engine.hpp"

#include <stdio.h>

// Temporaire

bool keydown(deep::vkey key, void *data)
{
    const char *name = deep::vkeys::get_name(key);

    // printf("Key pressed: %s\n", name);

    return true;
}

bool text_input(deep::native_char character, void *data)
{
    // printf("%c", character);

    return true;
}

bool mouse_button_down(deep::core_window::mouse_button button, deep::int64 relative_position_x, deep::int64 relative_position_y, void *data)
{
    printf("Clicked at: X: %lld Y: %lld\n", relative_position_x, relative_position_y);

    return true;
}

int main(int /*argv*/, const char * /*argc*/[])
{
    deep::engine eng = deep::engine::create();

    if (!eng.is_valid())
    {
        return 1;
    }

    deep::ref<deep::window> window = eng.get_window();

    window->set_keydown_callcack(keydown);
    window->set_text_input_callback(text_input);
    window->set_mouse_button_down_callback(mouse_button_down);

    eng.run();

    return 0;
}
