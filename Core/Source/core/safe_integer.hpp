#ifndef __DEEP_ENGINE_SAFE_INTEGER_HPP__
#define __DEEP_ENGINE_SAFE_INTEGER_HPP__

#include "core/def.hpp"
#include "core/types.hpp"

namespace deep
{

#if DE_WINDOWS
    using value64 = LONG64;
    using safe_value64 = volatile LONG64;
#else
#error Need implementation.
#endif

    class safe_int64
    {

        public:
            DE_API safe_int64(value64 value = 0);
            DE_API safe_int64(const safe_int64 &other);

            DE_API safe_int64 &operator=(value64 value);
            DE_API safe_int64 &operator=(const safe_int64 &other);

            DE_API safe_int64 &operator++();    // Pre-increment
            DE_API safe_int64 operator++(int);  // Post-increment
            DE_API safe_int64 &operator+=(value64 value);
            DE_API safe_int64 &operator+=(const safe_int64 &other);

            DE_API safe_int64 &operator--();    // Pre-decrement
            DE_API safe_int64 operator--(int);  // Post-decrement
            DE_API safe_int64 &operator-=(value64 value);
            DE_API safe_int64 &operator-=(const safe_int64 &other);

            DE_API safe_int64 &operator&=(value64 value);
            DE_API safe_int64 &operator&=(const safe_int64 &other);
            DE_API safe_int64 &operator|=(value64 value);
            DE_API safe_int64 &operator|=(const safe_int64 &other);
            DE_API safe_int64 &operator^=(value64 value);
            DE_API safe_int64 &operator^=(const safe_int64 &other);

            DE_API void increment();
            DE_API void add(value64 value);
            DE_API void add(const safe_int64 &other);

            DE_API void decrement();
            DE_API void subtract(value64 value);
            DE_API void subtract(const safe_int64 &other);

            DE_API void bitwise_and(value64 value);
            DE_API void bitwise_and(const safe_int64 &other);
            DE_API void bitwise_or(value64 value);
            DE_API void bitwise_or(const safe_int64 &other);
            DE_API void bitwise_xor(value64 value);
            DE_API void bitwise_xor(const safe_int64 &other);

            DE_API value64 get() const;
            DE_API void set(value64 value);

        private:
            DE_ALIGN(8) mutable safe_value64 m_Value;

    };

    /*
    =====================
    safe_int64::operator=
    =====================
    */
    inline safe_int64 &safe_int64::operator=(value64 value)
    {
        set(value);

        return *this;
    }

    /*
    =====================
    safe_int64::operator=
    =====================
    */
    inline safe_int64 &safe_int64::operator=(const safe_int64 &other)
    {
        set(other.get());

        return *this;
    }

    /*
    ======================
    safe_int64::operator++
    ======================
    */
    inline safe_int64 &safe_int64::operator++()
    {
        increment();

        return *this;
    }

    /*
    ======================
    safe_int64::operator++
    ======================
    */
    inline safe_int64 safe_int64::operator++(int)
    {
        safe_int64 tmp = *this;

        ++(*this);

        return tmp;
    }

    /*
    ======================
    safe_int64::operator+=
    ======================
    */
    inline safe_int64 &safe_int64::operator+=(value64 value)
    {
        add(value);

        return *this;
    }

    /*
    ======================
    safe_int64::operator+=
    ======================
    */
    inline safe_int64 &safe_int64::operator+=(const safe_int64 &other)
    {
        add(other);

        return *this;
    }

    /*
    ======================
    safe_int64::operator--
    ======================
    */
    inline safe_int64 &safe_int64::operator--()
    {
        decrement();

        return *this;
    }

    /*
    ======================
    safe_int64::operator--
    ======================
    */
    inline safe_int64 safe_int64::operator--(int)
    {
        safe_int64 tmp = *this;

        --(*this);

        return tmp;
    }

    /*
    ======================
    safe_int64::operator-=
    ======================
    */
    inline safe_int64 &safe_int64::operator-=(value64 value)
    {
        subtract(value);

        return *this;
    }

    /*
    ======================
    safe_int64::operator-=
    ======================
    */
    inline safe_int64 &safe_int64::operator-=(const safe_int64 &other)
    {
        subtract(other);

        return *this;
    }

    /*
    ======================
    safe_int64::operator&=
    ======================
    */
    inline safe_int64 &safe_int64::operator&=(value64 value)
    {
        bitwise_and(value);

        return *this;
    }

    /*
    ======================
    safe_int64::operator&=
    ======================
    */
    inline safe_int64 &safe_int64::operator&=(const safe_int64 &other)
    {
        bitwise_and(other);

        return *this;
    }

    /*
    ======================
    safe_int64::operator|=
    ======================
    */
    inline safe_int64 &safe_int64::operator|=(value64 value)
    {
        bitwise_or(value);

        return *this;
    }

    /*
    ======================
    safe_int64::operator|=
    ======================
    */
    inline safe_int64 &safe_int64::operator|=(const safe_int64 &other)
    {
        bitwise_or(other);

        return *this;
    }

    /*
    ======================
    safe_int64::operator^=
    ======================
    */
    inline safe_int64 &safe_int64::operator^=(value64 value)
    {
        bitwise_xor(value);

        return *this;
    }

    /*
    ======================
    safe_int64::operator^=
    ======================
    */
    inline safe_int64 &safe_int64::operator^=(const safe_int64 &other)
    {
        bitwise_xor(other);

        return *this;
    }

    /*
    =====================
    safe_int64::increment
    =====================
    */
    inline void safe_int64::increment()
    {
#if DE_WINDOWS
        InterlockedIncrement64(&m_Value);
#else
#error Need implementation.
#endif
    }

    /*
    =====================
    safe_int64::decrement
    =====================
    */
    inline void safe_int64::decrement()
    {
#if DE_WINDOWS
        InterlockedDecrement64(&m_Value);
#else
#error Need implementation.
#endif
    }

    /*
    ===============
    safe_int64::add
    ===============
    */
    inline void safe_int64::add(value64 value)
    {
#if DE_WINDOWS
        InterlockedExchangeAdd64(&m_Value, value);
#else
#error Need implementation.
#endif
    }

    /*
    ===============
    safe_int64::add
    ===============
    */
    inline void safe_int64::add(const safe_int64 &other)
    {
        add(other.get());
    }

    /*
    ====================
    safe_int64::subtract
    ====================
    */
    inline void safe_int64::subtract(value64 value)
    {
#if DE_WINDOWS
        InterlockedExchangeAdd64(&m_Value, -value);
#else
#error Need implementation.
#endif
    }

    /*
    ====================
    safe_int64::subtract
    ====================
    */
    inline void safe_int64::subtract(const safe_int64 &other)
    {
        subtract(other.get());
    }

    /*
    =======================
    safe_int64::bitwise_and
    =======================
    */
    inline void safe_int64::bitwise_and(value64 value)
    {
#if DE_WINDOWS
        InterlockedAnd64(&m_Value, value);
#else
#error Need implementation.
#endif
    }

    /*
    =======================
    safe_int64::bitwise_and
    =======================
    */
    inline void safe_int64::bitwise_and(const safe_int64 &other)
    {
        bitwise_and(other.get());
    }

    /*
    ======================
    safe_int64::bitwise_or
    ======================
    */
    inline void safe_int64::bitwise_or(value64 value)
    {
#if DE_WINDOWS
        InterlockedOr64(&m_Value, value);
#else
#error Need implementation.
#endif
    }

    /*
    ======================
    safe_int64::bitwise_or
    ======================
    */
    inline void safe_int64::bitwise_or(const safe_int64 &other)
    {
        bitwise_or(other.get());
    }

    /*
    =======================
    safe_int64::bitwise_xor
    =======================
    */
    inline void safe_int64::bitwise_xor(value64 value)
    {
#if DE_WINDOWS
        InterlockedXor64(&m_Value, value);
#else
#error Need implementation.
#endif
    }

    /*
    =======================
    safe_int64::bitwise_xor
    =======================
    */
    inline void safe_int64::bitwise_xor(const safe_int64 &other)
    {
        bitwise_xor(other.get());
    }

    /*
    ===============
    safe_int64::get
    ===============
    */
    inline value64 safe_int64::get() const
    {
#if DE_WINDOWS
        return InterlockedExchange64(&m_Value, m_Value);
#else
#error Need implementation.
#endif
    }

    /*
    ===============
    safe_int64::set
    ===============
    */
    inline void safe_int64::set(value64 value)
    {
#if DE_WINDOWS
        InterlockedExchange64(&m_Value, value);
#else
#error Need implementation.
#endif
    }

}

#endif