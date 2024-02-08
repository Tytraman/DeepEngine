#ifndef __DEEP_ENGINE_MEMORY_HPP__
#define __DEEP_ENGINE_MEMORY_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/common.hpp>

#include <stddef.h>
#include <utility>
#include <new>

namespace deep
{

#if DE_WINDOWS
    using mem_ptr = LPVOID;
#else

#endif

    enum class code : uint8_t
    {
        OK,
        ProcessHeapError,
        AllocError,
        ReallocError
    };

    template<typename Type>
    class list;

    /// @class memory_manager
    /// @brief Permet une gestion plus simple de la mémoire.
    class memory_manager
    {

        public:

            /// @brief      Alloue un espace mémoire sans le traquer avec le gestionnaire de mémoire.
            /// @param size Nombre d'octets à allouer pour l'espace mémoire. 
            /// @return     L'adresse vers l'espace mémoire ou \c nullptr si une erreur est survenue.
            DE_API static mem_ptr alloc_no_track(size_t size);

            /// @brief      Alloue un espace mémoire en le traquant avec le gesionnaire de mémoire. <i>(Recommandé pour éviter les memory leaks)</i>.
            /// @param size Nombre d'octets à allouer pour l'espace mémoire.
            /// @return     L'adresse vers l'espace mémoire ou \c nullptr si une erreur ests survenue.
            DE_API static mem_ptr alloc(size_t size);

            /// @brief         Ré-alloue un espace mémoire sans le traquer avec le gestionnaire de mémoire.
            /// @param memory  Adresse de l'espace mémoire à ré-allouer.
            /// @param newSize Nombre d'octets à allouer pour l'espace mémoire. 
            /// @return        L'adresse vers le nouvel espace mémoire ou \c nullptr si une erreur est survenue.
            DE_API static mem_ptr realloc_no_track(mem_ptr memory, size_t newSize);

            /// @brief         Ré-alloue un espace mémoire en le traquant avec le gestionnaire de mémoire.
            /// @param memory  Adresse de l'espace mémoire à ré-allouer.
            /// @param newSize Nombre d'octets à allouer pour l'espace mémoire. 
            /// @return        L'adresse vers le nouvel espace mémoire ou \c nullptr si une erreur est survenue.
            DE_API static mem_ptr realloc(mem_ptr memory, size_t newSize);

            /// @brief        Libère un espace mémoire qui a été créé sans être traqué.
            /// @param memory Adresse de l'espace mémoire à libérer.
            DE_API static void free_no_track(mem_ptr memory);

            /// @brief        Libère un espace mémoire qui a été créé en étant traqué.
            /// @param memory Adresse de l'espace mémoire à libérer.
            DE_API static void free(const mem_ptr memory);

            template<typename Type, typename... Args>
            static Type *alloc_type(Args&&... args);

        private:
            static list<mem_ptr> g_MemoryTrack;

    };


    template<typename Type, typename... Args>
    inline Type *memory_manager::alloc_type(Args&&... args)
    {
        Type *obj = (Type *) mem::alloc(sizeof(Type));
        new(obj) Type(std::forward<Args>(args)...);
        
        return obj;
    }

    using mem = memory_manager;

    class DE_API memory_chunk
    {

        public:
            memory_chunk(mem_ptr data = nullptr, size_t size = 0);
            memory_chunk(const memory_chunk &other);

            static bool alloc(memory_chunk &dest, size_t size);
            void free();

            mem_ptr data();
            size_t size() const;

            void set_data(mem_ptr data);
            void set_size(size_t size);

        private:
            mem_ptr m_Data;
            size_t  m_Size;

    };

    /*
    ==================
    memory_chunk::free
    ==================
    */
    inline void memory_chunk::free()
    {
        if(m_Data != nullptr)
            mem::free(m_Data);

        m_Data = nullptr;
        m_Size = 0;
    }

    /*
    ==================
    memory_chunk::data
    ==================
    */
    inline mem_ptr memory_chunk::data()
    {
        return m_Data;
    }

    /*
    ==================
    memory_chunk::size
    ==================
    */
    inline size_t memory_chunk::size() const
    {
        return m_Size;
    }

    /*
    ======================
    memory_chunk::set_data
    ======================
    */
    inline void memory_chunk::set_data(mem_ptr data)
    {
        m_Data = data;
    }

    /*
    ======================
    memory_chunk::set_size
    ======================
    */
    inline void memory_chunk::set_size(size_t size)
    {
        m_Size = size;
    }

    template<typename Type>
    class unique_ptr
    {

        public:
            /// @brief      Instancie un pointeur unique.
            /// @remark     Le constructeur est explicite pour éviter de confondre une instanciation avec l'opération = et ainsi avoir une meilleure compréhension du code.
            /// @param ptr  Une adresse pointant sur une zone mémoire allouée avec \ref mem::alloc.
            explicit unique_ptr(Type *ptr = nullptr);

            /// @brief Destructeur du pointeur unique. Permet de libérer la mémoire automatiquement et éviter des memory leaks.
            ~unique_ptr();

            unique_ptr(unique_ptr &&other);
            unique_ptr &operator=(unique_ptr &&other) noexcept;
            unique_ptr &operator=(Type *ptr);

            /// @brief      Surcharge de l'opérateur new pour attribuer un espace mémoire grâce à \ref mem::alloc.
            /// @param size
            /// @return 
            void *operator new(size_t size);

            /// @brief  Retourne la référence vers l'objet pointé.
            /// @return La référence de l'objet pointé.
            Type &operator*();

            /// @brief  Retourne le pointeur interne après l'avoir défini à \c nullptr.
            /// @return Le pointeur interne.
            Type *release();

            /// @brief      Modifie la valeur du pointeur interne après avoir \ref mem::free celui déjà présent.
            /// @param ptr  Le nouveau pointeur.
            void reset(Type *ptr);

            /// @brief  Retourne le pointeur interne.
            /// @return Le pointeur interne.
            Type *get() const;

            /// @brief  Retourne le pointeur interne.
            /// @return Le pointeur interne.
            Type *operator->() const;

            void assign(Type *ptr);

        public:
            unique_ptr(const unique_ptr &other) = delete;             // Il ne faut pas que la classe puisse copier un autre pointeur unique sinon elle perd tout son intérêt.
            unique_ptr &operator=(const unique_ptr &other) = delete;

        private:
            Type *m_Ptr;

    };

    /*
    ============================
    unique_ptr<Type>::unique_ptr
    ============================
    */
    template<typename Type>
    unique_ptr<Type>::unique_ptr(Type *ptr)
        : m_Ptr(ptr)
    { }

    /*
    =============================
    unique_ptr<Type>::~unique_ptr
    =============================
    */
    template<typename Type>
    unique_ptr<Type>::~unique_ptr()
    {
        if(m_Ptr != nullptr)
            mem::free(m_Ptr);
    }

    /*
    ============================
    unique_ptr<Type>::unique_ptr
    ============================
    */
    template<typename Type>
    unique_ptr<Type>::unique_ptr(unique_ptr &&other)
        : m_Ptr(other.release())
    { }

    /*
    ===========================
    unique_ptr<Type>::operator=
    ===========================
    */
    template<typename Type>
    unique_ptr<Type> &unique_ptr<Type>::operator=(unique_ptr &&other) noexcept
    {
        if(this != &other)
            reset(other.release());

        return *this;
    }

    /*
    ===========================
    unique_ptr<Type>::operator=
    ===========================
    */
    template<typename Type>
    unique_ptr<Type> &unique_ptr<Type>::operator=(Type *ptr)
    {
        if(m_Ptr != ptr)
            reset(ptr);

        return *this;
    }

    /*
    ==========================
    unique_ptr<T>::~unique_ptr
    ==========================
    */
    template<typename Type>
    void *unique_ptr<Type>::operator new(size_t size)
    {
        return mem::alloc(size);
    }

    /*
    ===========================
    unique_ptr<Type>::operator*
    ===========================
    */
    template<typename Type>
    Type &unique_ptr<Type>::operator*()
    {
        return *m_Ptr;
    }

    /*
    =========================
    unique_ptr<Type>::release
    =========================
    */
    template<typename Type>
    Type *unique_ptr<Type>::release()
    {
        return exchange(m_Ptr, nullptr);
    }

    /*
    =======================
    unique_ptr<Type>::reset
    =======================
    */
    template<typename Type>
    void unique_ptr<Type>::reset(Type *ptr)
    {
        if(m_Ptr != nullptr)
            mem::free(m_Ptr);

        m_Ptr = ptr;
    }

    /*
    =====================
    unique_ptr<Type>::get
    =====================
    */
    template<typename Type>
    Type *unique_ptr<Type>::get() const
    {
        return m_Ptr;
    }

    /*
    ============================
    unique_ptr<Type>::operator->
    ============================
    */
    template<typename Type>
    Type *unique_ptr<Type>::operator->() const
    {
        return m_Ptr;
    }

    /*
    ========================
    unique_ptr<Type>::assign
    ========================
    */
    template<typename Type>
    void unique_ptr<Type>::assign(Type *ptr)
    {
        m_Ptr = ptr;
    }

}

#endif