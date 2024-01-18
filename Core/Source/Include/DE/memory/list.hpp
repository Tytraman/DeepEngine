#ifndef __DEEP_ENGINE_LIST_HPP__
#define __DEEP_ENGINE_LIST_HPP__

#include <DE/memory/collection.hpp>

namespace deep
{

    template<typename Type>
    class list_iterator : public iiterator<Type, list_iterator<Type>>
    {

        public:
            using TypePtr = Type*;
            using ConstTypePtr = const Type*;
            using TypeRef = Type&;
            using ConstTypeRef = const Type&;

        public:
            list_iterator(TypePtr ptr);

            list_iterator<Type> &operator++()   override;
            list_iterator<Type> operator++(int) override;

            list_iterator<Type> &operator--()   override;
            list_iterator<Type> operator--(int) override;

            TypeRef operator[](size_t index) override;
            TypeRef operator*()              override;

            bool operator==(const list_iterator<Type> &other) const override;
            bool operator!=(const list_iterator<Type> &other) const override;

    };

    /*
    ==================================
    list_iterator<Type>::list_iterator
    ==================================
    */
    template<typename Type>
    list_iterator<Type>::list_iterator(TypePtr ptr)
        : iiterator<Type, list_iterator<Type>>(ptr)
    { }

    /*
    ===============================
    list_iterator<Type>::operator++
    ===============================
    */
    template<typename Type>
    list_iterator<Type> &list_iterator<Type>::operator++()
    {
        m_Ptr++;

        return *this;
    }

    /*
    ===============================
    list_iterator<Type>::operator++
    ===============================
    */
    template<typename Type>
    list_iterator<Type> list_iterator<Type>::operator++(int)
    {
        // Retourne une copie de l'état avant modification de l'itérateur.
        auto it = *this;

        ++(*this);

        return it;
    }

    /*
    ===============================
    list_iterator<Type>::operator--
    ===============================
    */
    template<typename Type>
    list_iterator<Type> &list_iterator<Type>::operator--()
    {
        m_Ptr--;

        return *this;
    }

    /*
    ===============================
    list_iterator<Type>::operator--
    ===============================
    */
    template<typename Type>
    list_iterator<Type> list_iterator<Type>::operator--(int)
    {
        // Retourne une copie de l'état avant modification de l'itérateur.
        auto it = *this;

        --(*this);

        return it;
    }

    /*
    ===============================
    list_iterator<Type>::operator[]
    ===============================
    */
    template<typename Type>
    typename list_iterator<Type>::TypeRef list_iterator<Type>::operator[](size_t index)
    {
        return *(m_Ptr + index);
    }

    /*
    ==============================
    list_iterator<Type>::operator*
    ==============================
    */
    template<typename Type>
    typename list_iterator<Type>::TypeRef list_iterator<Type>::operator*()
    {
        return *m_Ptr;
    }

    /*
    ===============================
    list_iterator<Type>::operator==
    ===============================
    */
    template<typename Type>
    bool list_iterator<Type>::operator==(const list_iterator<Type> &other) const
    {
        return m_Ptr == other.m_Ptr;
    }

    /*
    ===============================
    list_iterator<Type>::operator!=
    ===============================
    */
    template<typename Type>
    bool list_iterator<Type>::operator!=(const list_iterator<Type> &other) const
    {
        return m_Ptr != other.m_Ptr;
    }

    template<typename Type>
    class list : public icollection<Type, list_iterator<Type>>
    {
        public:
            using TypePtr = Type*;
            using ConstTypePtr = const Type*;
            using TypeRef = Type&;
            using ConstTypeRef = const Type&;

        public:
            list(uint32_t capacityStep = 10);
            ~list();

            void init(uint32_t capacityStep = 10);

            bool add()                     override;
            bool add(ConstTypeRef element) override;
            bool add(Type &&element)       override;

            bool remove(size_t index) override;

            size_t find(const Type &toSearch) const override;

            bool reserve(size_t numberOfElements) override;
            void fill_with_byte(uint8_t value);

            void empty() override;
            void free()  override;

            list_iterator<Type> begin() override;
            list_iterator<Type> end()   override;

            size_t   get_capacity()         const;
            uint32_t get_capacity_step()     const;
            TypeRef operator[](size_t index);

            void set_count(size_t count);
            void set_capacity_step(size_t size);

        protected:
            Type     *m_Data;
            size_t    m_Capacity;
            uint32_t  m_CapacityStep;

            bool grow_if_needed();

    };

    /*
    ================
    list<Type>::list
    ================
    */
    template<typename Type>
    list<Type>::list(uint32_t capacityStep)
        : icollection<Type, list_iterator<Type>>(),
          m_Data(nullptr),
          m_Capacity(0),
          m_CapacityStep(capacityStep)
    { }

    /*
    =================
    list<Type>::~list
    =================
    */
    template<typename Type>
    list<Type>::~list()
    {
        if(m_Data != nullptr)
            free();
    }

    template<typename Type>
    void list<Type>::init(uint32_t capacityStep)
    {
        m_Data = nullptr;
        m_Capacity = 0;
        m_CapacityStep = capacityStep;
        m_NumberOfElements = 0;
    }

    /*
    ===============
    list<Type>::add
    ===============
    */
    template<typename Type>
    bool list<Type>::add(ConstTypeRef element)
    {
        // Augmente la taille du buffer si nécessaire.
        if(!grow_if_needed())
            return false;

        // Déplace ou copie l'élément dans la case mémoire.
        m_Data[m_NumberOfElements] = element;
        m_NumberOfElements++;

        return true;
    }

    /*
    ===============
    list<Type>::add
    ===============
    */
    template<typename Type>
    bool list<Type>::add(Type &&element)
    {
        // Augmente la taille du buffer si nécessaire.
        if(!grow_if_needed())
            return false;

        // Déplace ou copie l'élément dans la case mémoire.
        m_Data[m_NumberOfElements] = rvalue_cast(element);
        m_NumberOfElements++;

        return true;
    }

    /*
    ===============
    list<Type>::add
    ===============
    */
    template<typename Type>
    bool list<Type>::add()
    {
        if(!grow_if_needed())
            return false;

        m_NumberOfElements++;

        return true;
    }

    /*
    ==================
    list<Type>::remove
    ==================
    */
    template<typename Type>
    bool list<Type>::remove(size_t index)
    {
        if(index >= m_NumberOfElements)
            return false;

        // Obtient le nombre d'éléments pour atteindre la fin de la liste.
        size_t diff = m_NumberOfElements - index - 1;
        size_t size = sizeof(Type);
        size_t numberOfBytesToMove = diff * size;

        memmove(m_Data + index, m_Data + (index + 1), numberOfBytesToMove);

        m_NumberOfElements--;

        return true;
    }

    /*
    ================
    list<Type>::find
    ================
    */
    template<typename Type>
    size_t list<Type>::find(const Type &toSearch) const
    {
        // Pointeur vers le tableau des éléments de la liste.
        uint8_t *ptr = (uint8_t *) m_Data;
        size_t elementSize = sizeof(Type);
        size_t numberOfElements = m_NumberOfElements;
        size_t index = 0;
        ConstTypePtr element = &toSearch;

        // Vérifie pour chaque élément de la liste si la donnée est la même que celle recherchée.
        while(index < numberOfElements) {
            if(memcmp(ptr, element, elementSize) == 0)
                return index;

            ptr += elementSize;
            index++;
        }

        return list::nothing;
    }

    /*
    ===================
    list<Type>::reserve
    ===================
    */
    template<typename Type>
    bool list<Type>::reserve(size_t numberOfElements)
    {
        if(numberOfElements == m_NumberOfElements)
            return true;

        size_t newCapacity = (numberOfElements / m_CapacityStep + 1) * m_CapacityStep;
        mem_ptr ptr = mem::realloc_no_track(m_Data, newCapacity * sizeof(Type));

        if(ptr == nullptr)
            return false;

        m_Data = static_cast<TypePtr>(ptr);
        m_Capacity = newCapacity;
        m_NumberOfElements = numberOfElements;

        return true;
    }

    /*
    ==========================
    list<Type>::fill_with_byte
    ==========================
    */
    template<typename Type>
    void list<Type>::fill_with_byte(uint8_t value)
    {
        memset(m_Data, value, m_NumberOfElements * sizeof(Type));
    }

    /*
    =================
    list<Type>::empty
    =================
    */
    template<typename Type>
    void list<Type>::empty()
    {
        m_Data = nullptr;
        m_NumberOfElements = 0;
        m_Capacity = 0;
    }

    /*
    ================
    list<Type>::free
    ================
    */
    template<typename Type>
    void list<Type>::free()
    {
        mem::free_no_track(m_Data);
        m_Data = nullptr;
        m_NumberOfElements = 0;
        m_Capacity = 0;
    }

    /*
    =================
    list<Type>::begin
    =================
    */
    template<typename Type>
    list_iterator<Type> list<Type>::begin()
    {
        return list_iterator(m_Data);
    }

    /*
    ===============
    list<Type>::end
    ===============
    */
    template<typename Type>
    list_iterator<Type> list<Type>::end()
    {
        return list_iterator(m_Data + m_NumberOfElements);
    }

    /*
    =======================
    list<Type>::get_capacity
    =======================
    */
    template<typename Type>
    size_t list<Type>::get_capacity() const
    {
        return m_Capacity;
    }

    /*
    ===========================
    list<Type>::get_capacity_step
    ===========================
    */
    template<typename Type>
    uint32_t list<Type>::get_capacity_step() const
    {
        return m_Capacity;
    }

    /*
    ======================
    list<Type>::operator[]
    ======================
    */
    template<typename Type>
    typename list<Type>::TypeRef list<Type>::operator[](size_t index)
    {
        return m_Data[index];
    }

    /*
    ========================
    list<Type>::grow_if_needed
    ========================
    */
    template<typename Type>
    bool list<Type>::grow_if_needed()
    {
        // Si le nombre d'éléments présents dans la liste est supérieur à la capacité,
        // on augmente celle-ci du pas attribué.
        if(m_NumberOfElements >= m_Capacity)
        {
            size_t newCapacity = m_Capacity + m_CapacityStep;
            mem_ptr ptr = mem::realloc_no_track(m_Data, newCapacity * sizeof(Type));

            if(ptr == nullptr)
                return false;

            m_Data = (TypePtr) ptr;
            m_Capacity = newCapacity;
        }

        return true;
    }

    template<typename Type>
    inline void list<Type>::set_count(size_t count)
    {
        m_NumberOfElements = count;
    }

    template<typename Type>
    inline void list<Type>::set_capacity_step(size_t size)
    {
        m_CapacityStep = size;
    }

}

#endif