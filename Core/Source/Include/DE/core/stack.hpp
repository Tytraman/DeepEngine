#ifndef __DEEP_ENGINE_STACK_HPP__
#define __DEEP_ENGINE_STACK_HPP__

#include "DE/core/def.hpp"
#include "DE/core/types.hpp"
#include "DE/core/list.hpp"

namespace deep
{

    template<typename Type>
    class stack
    {

        public:
            bool add(const Type &element);
            bool add(Type &&element);

            Type *get();
            bool pop();

            bool any() const;

            size_t count() const;

        protected:
            list<Type> m_List;

    };

    template<typename Type>
    inline bool stack<Type>::add(const Type &element)
    {
        return m_List.add(element);
    }

    template<typename Type>
    inline bool stack<Type>::add(Type &&element)
    {
        return m_List.add(rvalue_cast(element));
    }

    template<typename Type>
    Type *stack<Type>::get()
    {
        size_t count = m_List.count();
        if(count == 0)
            return nullptr;

        return &m_List[count - 1];
    }

    template<typename Type>
    bool stack<Type>::pop()
    {
        return m_List.remove(m_List.count() - 1);
    }

    template<typename Type>
    bool stack<Type> ::any() const
    {
        return m_List.count() > 0;
    }

    template<typename Type>
    inline size_t stack<Type>::count() const
    {
        return m_List.count();
    }

}

#endif