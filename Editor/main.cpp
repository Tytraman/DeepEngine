#include "DeepEngine/engine.hpp"

int main(int /*argv*/, const char * /*argc*/[])
{
    deep::ref<deep::engine> eng = deep::engine::create();

    if (!eng.is_valid())
    {
        return 1;
    }

    eng->run();

    eng->get_context()->out() << "~Goodbye~\r\n";

    return 0;
}
