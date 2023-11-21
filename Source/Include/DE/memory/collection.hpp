#ifndef __DEEP_ENGINE_COLLECTION_HPP__
#define __DEEP_ENGINE_COLLECTION_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>

namespace de
{
    template<typename Type, typename Iterator>
    class icollection;

    template<typename Type, typename Iterator>
    class iiterator;


    template<typename Type, typename Iterator>
    class icollection
    {

        public:
            static constexpr size_t nothing = -1;

        public:
            icollection();

            virtual bool add()                    = 0;
            virtual bool add(const Type &element) = 0;
            virtual bool add(Type &&element)      = 0;

            virtual bool remove(size_t index) = 0;

            virtual size_t find(const Type &element) const = 0;

            virtual bool reserve(size_t numberOfElements) = 0;

            virtual void empty() = 0;
            virtual void free()  = 0;

            virtual Iterator begin() = 0;
            virtual Iterator end()   = 0;

            size_t count() const;

        protected:
            size_t m_NumberOfElements;

    };

    template<typename Type, typename Iterator>
    icollection<Type, Iterator>::icollection()
        : m_NumberOfElements(0)
    { }

    template<typename Type, typename Iterator>
    size_t icollection<Type, Iterator>::count() const
    {
        return m_NumberOfElements;
    }

    template<typename Type, typename Iterator>
    class iiterator
    {

        public:
            iiterator(Type *ptr);

            virtual Iterator &operator++()   = 0;
            virtual Iterator operator++(int) = 0;

            virtual Iterator &operator--()   = 0;
            virtual Iterator operator--(int) = 0;

            virtual Type &operator[](size_t index) = 0;
            virtual Type &operator*()              = 0;

            virtual bool operator==(const Iterator &other) const = 0;
            virtual bool operator!=(const Iterator &other) const = 0;

        protected:
            Type *m_Ptr;

    };

    template<typename Type, typename Iterator>
    iiterator<Type, Iterator>::iiterator(Type *ptr)
        : m_Ptr(ptr)
    { }

}

#endif