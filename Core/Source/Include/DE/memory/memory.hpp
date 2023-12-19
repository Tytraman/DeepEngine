#ifndef __DEEP_ENGINE_MEMORY_HPP__
#define __DEEP_ENGINE_MEMORY_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/common.hpp>

#include <utility>

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
    class DE_API memory_manager
    {

        public:

            /// @brief      Alloue un espace mémoire sans le traquer avec le gestionnaire de mémoire.
            /// @param size Nombre d'octets à allouer pour l'espace mémoire. 
            /// @return     L'adresse vers l'espace mémoire ou \c nullptr si une erreur est survenue.
            static mem_ptr allocNoTrack(size_t size);

            /// @brief      Alloue un espace mémoire en le traquant avec le gesionnaire de mémoire. <i>(Recommandé pour éviter les memory leaks)</i>.
            /// @param size Nombre d'octets à allouer pour l'espace mémoire.
            /// @return     L'adresse vers l'espace mémoire ou \c nullptr si une erreur ests survenue.
            static mem_ptr alloc(size_t size);

            /// @brief         Ré-alloue un espace mémoire sans le traquer avec le gestionnaire de mémoire.
            /// @param memory  Adresse de l'espace mémoire à ré-allouer.
            /// @param newSize Nombre d'octets à allouer pour l'espace mémoire. 
            /// @return        L'adresse vers le nouvel espace mémoire ou \c nullptr si une erreur est survenue.
            static mem_ptr reallocNoTrack(mem_ptr memory, size_t newSize);

            /// @brief         Ré-alloue un espace mémoire en le traquant avec le gestionnaire de mémoire.
            /// @param memory  Adresse de l'espace mémoire à ré-allouer.
            /// @param newSize Nombre d'octets à allouer pour l'espace mémoire. 
            /// @return        L'adresse vers le nouvel espace mémoire ou \c nullptr si une erreur est survenue.
            static mem_ptr realloc(mem_ptr memory, size_t newSize);

            /// @brief        Libère un espace mémoire qui a été créé sans être traqué.
            /// @param memory Adresse de l'espace mémoire à libérer.
            static void freeNoTrack(mem_ptr memory);

            /// @brief        Libère un espace mémoire qui a été créé en étant traqué.
            /// @param memory Adresse de l'espace mémoire à libérer.
            static void free(const mem_ptr memory);

        private:
            static list<mem_ptr> g_MemoryTrack;

    };

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

            void setData(mem_ptr data);
            void setSize(size_t size);

        private:
            mem_ptr m_Data;
            size_t  m_Size;

    };

    /*
    =================
    memory_chunk::free
    =================
    */
    inline void memory_chunk::free()
    {
        mem::free(m_Data);
        m_Data = nullptr;
        m_Size = 0;
    }

    /*
    =================
    memory_chunk::data
    =================
    */
    inline mem_ptr memory_chunk::data()
    {
        return m_Data;
    }

    /*
    =================
    memory_chunk::size
    =================
    */
    inline size_t memory_chunk::size() const
    {
        return m_Size;
    }

    /*
    ====================
    memory_chunk::setData
    ====================
    */
    inline void memory_chunk::setData(mem_ptr data)
    {
        m_Data = data;
    }

    /*
    ====================
    memory_chunk::setSize
    ====================
    */
    inline void memory_chunk::setSize(size_t size)
    {
        m_Size = size;
    }

    template<typename T>
    class unique_ptr
    {

        public:
            /// @brief      Instancie un pointeur unique.
            /// @remark     Le constructeur est explicite pour éviter de confondre une instanciation avec l'opération = et ainsi avoir une meilleure compréhension du code.
            /// @param ptr  Une adresse pointant sur une zone mémoire allouée avec \ref mem::alloc.
            explicit unique_ptr(T *ptr = nullptr);

            /// @brief Destructeur du pointeur unique. Permet de libérer la mémoire automatiquement et éviter des memory leaks.
            ~unique_ptr();

            unique_ptr(unique_ptr &&other);
            unique_ptr &operator=(unique_ptr &&other) noexcept;
            unique_ptr &operator=(T *ptr);

            /// @brief      Surcharge de l'opérateur new pour attribuer un espace mémoire grâce à \ref mem::alloc.
            /// @param size
            /// @return 
            void *operator new(size_t size);

            /// @brief  Retourne la référence vers l'objet pointé.
            /// @return La référence de l'objet pointé.
            T &operator*();

            /// @brief  Retourne le pointeur interne après l'avoir défini à \c nullptr.
            /// @return Le pointeur interne.
            T *release();

            /// @brief      Modifie la valeur du pointeur interne après avoir \ref mem::free celui déjà présent.
            /// @param ptr  Le nouveau pointeur.
            void reset(T *ptr);

            /// @brief  Retourne le pointeur interne.
            /// @return Le pointeur interne.
            T *get() const;

            /// @brief  Retourne le pointeur interne.
            /// @return Le pointeur interne.
            T *operator->() const;

            void assign(T *ptr);

        public:
            unique_ptr(const unique_ptr &other) = delete;             // Il ne faut pas que la classe puisse copier un autre pointeur unique sinon elle perd tout son intérêt.
            unique_ptr &operator=(const unique_ptr &other) = delete;

        private:
            T *m_Ptr;

    };

    /*
    =======================
    unique_ptr<T>::unique_ptr
    =======================
    */
    template<typename T>
    unique_ptr<T>::unique_ptr(T *ptr)
        : m_Ptr(ptr)
    { }

    /*
    ========================
    unique_ptr<T>::~unique_ptr
    ========================
    */
    template<typename T>
    unique_ptr<T>::~unique_ptr()
    {
        if(m_Ptr != nullptr)
            mem::free(m_Ptr);
    }

    /*
    =======================
    unique_ptr<T>::unique_ptr
    =======================
    */
    template<typename T>
    unique_ptr<T>::unique_ptr(unique_ptr &&other)
        : m_Ptr(other.release())
    { }

    /*
    =======================
    unique_ptr<T>::operator=
    =======================
    */
    template<typename T>
    unique_ptr<T> &unique_ptr<T>::operator=(unique_ptr &&other) noexcept
    {
        if(this != &other)
            reset(other.release());

        return *this;
    }

    /*
    =======================
    unique_ptr<T>::operator=
    =======================
    */
    template<typename T>
    unique_ptr<T> &unique_ptr<T>::operator=(T *ptr)
    {
        if(m_Ptr != ptr)
            reset(ptr);

        return *this;
    }

    /*
    ========================
    unique_ptr<T>::~unique_ptr
    ========================
    */
    template<typename T>
    void *unique_ptr<T>::operator new(size_t size)
    {
        return mem::alloc(size);
    }

    /*
    =======================
    unique_ptr<T>::operator*
    =======================
    */
    template<typename T>
    T &unique_ptr<T>::operator*()
    {
        return *m_Ptr;
    }

    /*
    =====================
    unique_ptr<T>::release
    =====================
    */
    template<typename T>
    T *unique_ptr<T>::release()
    {
        return exchange(m_Ptr, nullptr);
    }

    /*
    ===================
    unique_ptr<T>::reset
    ===================
    */
    template<typename T>
    void unique_ptr<T>::reset(T *ptr)
    {
        if(m_Ptr != nullptr)
            mem::free(m_Ptr);

        m_Ptr = ptr;
    }

    /*
    =================
    unique_ptr<T>::get
    =================
    */
    template<typename T>
    T *unique_ptr<T>::get() const
    {
        return m_Ptr;
    }

    /*
    =========================
    unique_ptr<T>::operator->
    =========================
    */
    template<typename T>
    T *unique_ptr<T>::operator->() const
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