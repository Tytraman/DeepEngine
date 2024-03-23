#include "core/safe_integer.hpp"

namespace deep
{

    /*
    ======================
    safe_int64::safe_int64
    ======================
    */
    safe_int64::safe_int64(value64 value)
        : m_Value(0)
    {
        set(value);
    }

    /*
    ======================
    safe_int64::safe_int64
    ======================
    */
    safe_int64::safe_int64(const safe_int64 &other)
        : m_Value(0)
    {
        set(other.get());
    }

}
