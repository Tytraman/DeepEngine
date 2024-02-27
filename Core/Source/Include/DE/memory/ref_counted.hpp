#ifndef __DEEP_ENGINE_REF_COUNTED_HPP__
#define __DEEP_ENGINE_REF_COUNTED_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/memory/memory.hpp"

#include <cassert>

namespace deep
{

    /// @brief  Compte le nombre de référence faite à l'objet.
    /// @remark Pour compter les références d'une classe, celle-ci doit hériter de cette classe.
    class ref_counted
    {

        public:
            ref_counted();

            /// @brief Ajoute 1 au compteur de références.
            void take() const;

            /// @brief Retire 1 au compteur de références.
            bool drop() const;

            /// @brief  Récupère le nombre de références qu'il y a vers l'objet.
            /// @return Nombre de références vers l'objet.
            size_t get_ref_count() const;

        private:
            mutable size_t m_Count;

    };

    /*
    =================
    ref_counted::take
    =================
    */
    inline void ref_counted::take() const
    {
        m_Count++;
    }

    /*
    =================
    ref_counted::drop
    =================
    */
    inline bool ref_counted::drop() const
    {
        assert(m_Count > 0);

        m_Count--;

        // Si le compteur atteint 0, l'objet doit être détruit.
        return m_Count == 0;
    }

    /*
    =================
    ref_counted::drop
    =================
    */
    inline size_t ref_counted::get_ref_count() const
    {
        return m_Count;
    }

    template<typename Type>
    class ref
    {

        public:
            ref(Type *ptr);
            ref(const ref<Type> &other);
            ~ref();

            void reference(const ref &from);
            void unreference();

            ref<Type> &operator=(const ref<Type> &other);

            Type *operator->() const;
            Type &operator*() const;

            bool operator==(const Type *ptr) const;
            bool operator!=(const Type *ptr) const;
            bool operator==(const ref<Type> &other) const;
            bool operator!=(const ref<Type> &other) const;

            bool is_valid() const;
            bool is_null() const;

            Type *get() const;

        private:
            Type *m_Ptr;

        public:
            ref() = delete;

    };

    /*
    ==============
    ref<Type>::ref
    ==============
    */
    template<typename Type>
    ref<Type>::ref(Type *ptr)
        : m_Ptr(ptr)
    {
        static_assert(is_base_of<ref_counted, Type>, "The specified class needs to inherit from class ref_counted.");

        if(ptr != nullptr)
        {
            ptr->take();
        }
    }

    /*
    ==============
    ref<Type>::ref
    ==============
    */
    template<typename Type>
    ref<Type>::ref(const ref<Type> &other)
        : m_Ptr(other.m_Ptr)
    {
        if(other.m_Ptr != nullptr)
        {
            other.m_Ptr->take();
        }
    }

    /*
    ===============
    ref<Type>::~ref
    ===============
    */
    template<typename Type>
    ref<Type>::~ref()
    {
        unreference();
    }

    /*
    ====================
    ref<Type>::reference
    ====================
    */
    template<typename Type>
    inline void ref<Type>::reference(const ref &from)
    {
        if(m_Ptr == from.m_Ptr)
        {
            return;
        }

        unreference();

        m_Ptr = from.m_Ptr;
        if(m_Ptr != nullptr)
        {
            m_Ptr->take();
        }
    }

    /*
    ======================
    ref<Type>::unreference
    ======================
    */
    template<typename Type>
    inline void ref<Type>::unreference()
    {
        if(is_valid() && m_Ptr->drop())
        {
            mem::free_type(m_Ptr);
        }
    }

    /*
    ====================
    ref<Type>::operator=
    ====================
    */
    template<typename Type>
    ref<Type> &ref<Type>::operator=(const ref<Type> &other)
    {
        reference(other);

        return *this;
    }

    /*
    =====================
    ref<Type>::operator->
    =====================
    */
    template<typename Type>
    Type *ref<Type>::operator->() const
    {
        return m_Ptr;
    }

    /*
    ====================
    ref<Type>::operator*
    ====================
    */
    template<typename Type>
    Type &ref<Type>::operator*() const
    {
        return *m_Ptr;
    }

    /*
    =====================
    ref<Type>::operator==
    =====================
    */
    template<typename Type>
    inline bool ref<Type>::operator==(const Type *ptr) const
    {
        return m_Ptr == ptr;
    }

    /*
    =====================
    ref<Type>::operator!=
    =====================
    */
    template<typename Type>
    inline bool ref<Type>::operator!=(const Type *ptr) const
    {
        return m_Ptr != ptr;
    }

    /*
    =====================
    ref<Type>::operator==
    =====================
    */
    template<typename Type>
    bool ref<Type>::operator==(const ref<Type> &other) const
    {
        return m_Ptr == other.m_Ptr;
    }

    /*
    =====================
    ref<Type>::operator!=
    =====================
    */
    template<typename Type>
    inline bool ref<Type>::operator!=(const ref<Type> &other) const
    {
        return m_Ptr != other.m_Ptr;
    }

    /*
    ===================
    ref<Type>::is_valid
    ===================
    */
    template<typename Type>
    inline bool ref<Type>::is_valid() const
    {
        return m_Ptr != nullptr;
    }

    /*
    ==================
    ref<Type>::is_null
    ==================
    */
    template<typename Type>
    inline bool ref<Type>::is_null() const
    {
        return m_Ptr == nullptr;
    }

    /*
    ==============
    ref<Type>::get
    ==============
    */
    template<typename Type>
    Type *ref<Type>::get() const
    {
        return m_Ptr;
    }

}


#endif