#ifndef __DEEP_ENGINE_PAIR_HPP__
#define __DEEP_ENGINE_PAIR_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>

namespace de
{

    template<typename Type1, typename Type2>
    class pair
    {

        public:
            pair();
            pair(const Type1 &value1, const Type2 &value2);

            pair<Type1, Type2> &operator=(const pair<Type1, Type2> &other);

            Type1 &value1();
            Type2 &value2();

        private:
            Type1 m_Value1;
            Type2 m_Value2;

    };

    template<typename Type1, typename Type2>
    pair<Type1, Type2>::pair()
        : m_Value1(0),
          m_Value2(0)
    { }

    template<typename Type1, typename Type2>
    pair<Type1, Type2>::pair(const Type1 &value1, const Type2 &value2)
        : m_Value1(value1),
          m_Value2(value2)
    { }

    template<typename Type1, typename Type2>
    pair<Type1, Type2> &pair<Type1, Type2>::operator=(const pair<Type1, Type2> &other)
    {
        m_Value1 = other.m_Value1;
        m_Value2 = other.m_Value2;

        return *this;
    }

    template<typename Type1, typename Type2>
    Type1 &pair<Type1, Type2>::value1()
    {
        return m_Value1;
    }

    template<typename Type1, typename Type2>
    Type2 &pair<Type1, Type2>::value2()
    {
        return m_Value2;
    }

}

#endif