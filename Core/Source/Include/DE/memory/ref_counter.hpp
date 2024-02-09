#ifndef __DEEP_ENGINE_REF_COUNTER_HPP__
#define __DEEP_ENGINE_REF_COUNTER_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/memory/memory.hpp"

#include <cassert>

namespace deep
{

    /// @brief  Compte le nombre de référence faite à l'objet, libère la mémoire automatiquement une fois le compteur à 0.
    /// @remark Pour compter les références d'une classe, celle-ci doit hériter de cette classe.
    class reference_counter
    {

        public:
            reference_counter();

            /// @brief Ajoute 1 au compteur de références.
            void take() const;

            /// @brief Retire 1 au compteur de références, s'il atteint 0, libère la mémoire.
            void drop() const;

        private:
            mutable size_t m_Count;

    };

    /*
    =======================
    reference_counter::take
    =======================
    */
    inline void reference_counter::take() const
    {
        m_Count++;
    }

    /*
    =======================
    reference_counter::drop
    =======================
    */
    inline void reference_counter::drop() const
    {
        assert(m_Count > 0);

        m_Count--;

        if(m_Count == 0)
        {
            mem::free((mem_ptr) this);
        }
    }

    template<typename Type>
    class ref_counter
    {

        public:
            ref_counter(Type *ptr);
            ref_counter(const ref_counter<Type> &other);
            ~ref_counter();

            ref_counter<Type> &operator=(Type *ptr);
            ref_counter<Type> &operator=(const ref_counter<Type> &other);

            Type *operator->() const;
            Type &operator*() const;

            bool operator==(Type *ptr) const;
            bool operator==(const ref_counter<Type> &other) const;

            Type *get() const;

        private:
            Type *m_Ptr;

        public:
            ref_counter() = delete;


    };

    /*
    ==============================
    ref_counter<Type>::ref_counter
    ==============================
    */
    template<typename Type>
    ref_counter<Type>::ref_counter(Type *ptr)
        : m_Ptr(ptr)
    {
        static_assert(is_base_of<reference_counter, Type>, "The specified class needs to inherit from class reference_counter.");

        if(ptr != nullptr)
        {
            ptr->take();
        }
    }

    /*
    ==============================
    ref_counter<Type>::ref_counter
    ==============================
    */
    template<typename Type>
    ref_counter<Type>::ref_counter(const ref_counter<Type> &other)
        : m_Ptr(other.m_Ptr)
    {
        if(other.m_Ptr != nullptr)
        {
            other.m_Ptr->take();
        }
    }

    template<typename Type>
    ref_counter<Type>::~ref_counter()
    {
        if(m_Ptr != nullptr)
        {
            m_Ptr->drop();
        }
    }

    /*
    ============================
    ref_counter<Type>::operator=
    ============================
    */
    template<typename Type>
    ref_counter<Type> &ref_counter<Type>::operator=(Type *ptr)
    {
        if(ptr != nullptr)
        {
            ptr->take();
        }

        if(m_Ptr != nullptr)
        {
            m_Ptr->drop();
        }

        m_Ptr = ptr;

        return *this;
    }

    /*
    ============================
    ref_counter<Type>::operator=
    ============================
    */
    template<typename Type>
    ref_counter<Type> &ref_counter<Type>::operator=(const ref_counter<Type> &other)
    {
        *this = other.m_Ptr;

        return *this;
    }

    /*
    =============================
    ref_counter<Type>::operator->
    =============================
    */
    template<typename Type>
    Type *ref_counter<Type>::operator->() const
    {
        return m_Ptr;
    }

    /*
    ============================
    ref_counter<Type>::operator*
    ============================
    */
    template<typename Type>
    Type &ref_counter<Type>::operator*() const
    {
        return *m_Ptr;
    }

    /*
    =============================
    ref_counter<Type>::operator==
    =============================
    */
    template<typename Type>
    bool ref_counter<Type>::operator==(Type *ptr) const
    {
        return m_Ptr == ptr;
    }

    /*
    =============================
    ref_counter<Type>::operator==
    =============================
    */
    template<typename Type>
    bool ref_counter<Type>::operator==(const ref_counter<Type> &other) const
    {
        return m_Ptr == other.m_Ptr;
    }

    /*
    ======================
    ref_counter<Type>::get
    ======================
    */
    template<typename Type>
    Type *ref_counter<Type>::get() const
    {
        return m_Ptr;
    }

}


#endif