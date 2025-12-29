#include "DeepEngine/engine.hpp"

int main(int /*argv*/, const char * /*argc*/[])
{
    deep::engine eng = deep::engine::create();

    if (!eng.is_valid())
    {
        return 1;
    }

    eng.run();

    return 0;
}
