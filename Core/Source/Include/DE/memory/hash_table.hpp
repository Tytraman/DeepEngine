#ifndef __DEEP_ENGINE_HASH_TABLE_HPP__
#define __DEEP_ENGINE_HASH_TABLE_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>
#include <DE/string_utils.hpp>
#include <DE/memory/list.hpp>
#include <DE/string.hpp>

namespace deep
{

	template<typename Value>
    struct hash_entry;

    template<typename Value>
    class hash_table;

    template<typename Value>
    class hash_table_iterator;

    using hash_function = uint64_t (*)(const char *str);

    template<typename Value>
    using hash_table_free_element_callback = void (*)(Value &val);

    template<typename Value>
    struct hash_entry
    {
        uint64_t key;
        Value value;
        hash_entry<Value> *next;
        hash_entry<Value> *previous;

        hash_entry();
        hash_entry(uint64_t key, const Value &value, hash_entry<Value> *previous);
    };

    /*
	=============================
	hash_entry<Value>::hash_entry
	=============================
	*/
    template<typename Value>
    hash_entry<Value>::hash_entry()
        : key(-1),
          value(),
          next(nullptr),
          previous(nullptr)
    { }

    /*
	=============================
	hash_entry<Value>::hash_entry
	=============================
	*/
    template<typename Value>
    hash_entry<Value>::hash_entry(uint64_t _key, const Value &_value, hash_entry<Value> *_previous)
        : key(_key),
          value(_value),
          next(nullptr),
          previous(_previous)
    { }

    template<typename Value>
    class hash_table
    {

        public:
            hash_table(size_t size = 10, hash_function hashfunction = string_utils::hash, hash_table_free_element_callback<Value> freeElementCallback = nullptr);

            void init(size_t size = 10, hash_function hashfunction = string_utils::hash, hash_table_free_element_callback<Value> freeElementCallback = nullptr);

            hash_entry<Value> &insert(const char *key, const Value &value);
            hash_entry<Value> &insert(uint64_t key, const Value &value);

            hash_entry<Value> *operator[](const char *str);
            hash_entry<Value> *operator[](uint64_t key);

            hash_entry<Value> *first(uint64_t *bucket);
            hash_entry<Value> *last(uint64_t *bucket, uint64_t *count);

            bool remove(const char *str);
            bool remove(uint64_t key);

            void clear();

            hash_table_iterator<Value> begin();
            hash_table_iterator<Value> end();

            size_t size() const;
            size_t getNumberOfElements() const;

            hash_function getHashFunction() const;

        private:
            hash_entry<Value> **m_Table;
            size_t m_Size;
            size_t m_NumberOfElements;
            hash_function m_HashFunction;
            hash_table_free_element_callback<Value> m_FreeElementCallback;

            friend hash_table_iterator<Value>;

    };

    /*
	=============================
	hash_table<Value>::hash_table
	=============================
	*/
    template<typename Value>
    hash_table<Value>::hash_table(size_t size, hash_function hashfunction, hash_table_free_element_callback<Value> freeElementCallback)
        : m_Table(new hash_entry<Value>*[size]),
          m_Size(size),
          m_NumberOfElements(0),
          m_HashFunction(hashfunction),
          m_FreeElementCallback(freeElementCallback)
    {
        size_t index;

        for(index = 0; index < m_Size; ++index)
            m_Table[index] = nullptr;
    }

    /*
	=======================
	hash_table<Value>::init
	=======================
	*/
    template<typename Value>
    void hash_table<Value>::init(size_t size, hash_function hashfunction, hash_table_free_element_callback<Value> freeElementCallback)
    {
        m_Table = new hash_entry<Value>*[size];
        m_Size = size;
        m_NumberOfElements = 0;
        m_HashFunction = hashfunction;
        m_FreeElementCallback = freeElementCallback;

        size_t index;

        for(index = 0; index < m_Size; ++index)
            m_Table[index] = nullptr;
    }

    /*
	=========================
	hash_table<Value>::insert
	=========================
	*/
    template<typename Value>
    hash_entry<Value> &hash_table<Value>::insert(const char *key, const Value &value)
    {
        return insert(m_HashFunction(key), value);
    }

    /*
	=========================
	hash_table<Value>::insert
	=========================
	*/
    template<typename Value>
    hash_entry<Value> &hash_table<Value>::insert(uint64_t key, const Value &value)
    {
        uint64_t bucket = key % m_Size;

        // Cherche dans la table s'il y a déjà la clé.
        hash_entry<Value> *n = operator[](key);

        // Si la clé n'y est pas, l'ajoute.
        if(n == nullptr)
        {
            n = m_Table[bucket];

            if(n == nullptr)
            {
                n = new hash_entry<Value>(key, value, nullptr);

                m_Table[bucket] = n;

                m_NumberOfElements++;
                return *n;
            }

            while(n->next != nullptr)
                n = n->next;

            n->next = new hash_entry<Value>(key, value, n);
            m_NumberOfElements++;

            n = n->next;
        }
        // Si la clé y est, modifie seulement la valeur.
        else
        {
            n->value = value;
        }

        return *n;
    }

    /*
	=============================
	hash_table<Value>::operator[]
	=============================
	*/
    template<typename Value>
    hash_entry<Value> *hash_table<Value>::operator[](const char *str)
    {
        return operator[](m_HashFunction(str));
    }

    /*
	=============================
	hash_table<Value>::operator[]
	=============================
	*/
    template<typename Value>
    hash_entry<Value> *hash_table<Value>::operator[](uint64_t key)
    {
        uint64_t bucket = key % m_Size;

        hash_entry<Value> *n = m_Table[bucket];

        if(n == nullptr)
            return nullptr;

        // Itère à travers toutes les entrées du bucket.
        // TODO: faire une recherche binaire.
        while(n != nullptr)
        {
            if(n->key == key)
                return n;

            n = n->next;
        }

        return nullptr;
    }

    /*
	========================
	hash_table<Value>::first
	========================
	*/
    template<typename Value>
    hash_entry<Value> *hash_table<Value>::first(uint64_t *bucket)
    {
        *bucket = 0;

        if(m_Table == nullptr)
            return nullptr;

        hash_entry<Value> *n = m_Table[0];
        size_t index;

        for(index = 0; index < m_Size; ++index)
        {
            n = m_Table[index];

            if(n != nullptr)
                break;
        }

        *bucket = index;

        return n;
    }

    /*
	=======================
	hash_table<Value>::last
	=======================
	*/
    template<typename Value>
    hash_entry<Value> *hash_table<Value>::last(uint64_t *bucket, uint64_t *count)
    {
        *bucket = 0;
        *count = 0;

        if(m_Table == nullptr)
            return nullptr;

        hash_entry<Value> *n = m_Table[0];
        size_t index;
        uint64_t c = 0;

        for(index = 0; index < m_Size; ++index)
        {
            n = m_Table[index];
            c = 0;

            if(n != nullptr)
            {
                while(n->next != nullptr)
                {
                    n = n->next;
                    c++;
                }
            }
        }

        *bucket = index;
        *count = c;

        return n;
    }

    /*
	=========================
	hash_table<Value>::remove
	=========================
	*/
    template<typename Value>
    bool hash_table<Value>::remove(const char *str)
    {
        return remove(m_HashFunction(str));
    }

    /*
	=========================
	hash_table<Value>::remove
	=========================
	*/
    template<typename Value>
    bool hash_table<Value>::remove(uint64_t key)
    {
        // Récupère l'entrée à supprimer.
        hash_entry<Value> *entry = operator[](key);

        // S'il n'y a d'entrée, quitte la procédure.
        if(entry == nullptr)
            return false;

        // Déplace la prochaine entrée dans celle précédente.
        if(entry->previous != nullptr)
        {
            entry->previous->next = entry->next;
            entry->next->previous = entry->previous;
        }
        else
        {
            uint64_t bucket = key % m_Size;
            m_Table[bucket] = entry->next;

            if(entry->next != nullptr)
                entry->next->previous = nullptr;
        }

        delete entry;

        return true;
    }

    /*
	========================
	hash_table<Value>::clear
	========================
	*/
    template<typename Value>
    void hash_table<Value>::clear()
    {
        hash_table_iterator<Value> b = begin();
        hash_table_iterator<Value> e = end();

        if(m_FreeElementCallback != nullptr)
        {
            for(; b != e; ++b)
            {
                m_FreeElementCallback(b->value);
            }
        }

        m_NumberOfElements = 0;
    }

    /*
	========================
	hash_table<Value>::begin
	========================
	*/
    template<typename Value>
    hash_table_iterator<Value> hash_table<Value>::begin()
    {
        uint64_t bucket;
        auto val = first(&bucket);
        return hash_table_iterator<Value>(this, val, bucket, 0);
    }

    /*
	======================
	hash_table<Value>::end
	======================
	*/
    template<typename Value>
    hash_table_iterator<Value> hash_table<Value>::end()
    {
        uint64_t bucket;
        uint64_t count;
        auto val = last(&bucket, &count);
        return hash_table_iterator<Value>(this, val, bucket, count);
    }

    /*
	=======================
	hash_table<Value>::size
	=======================
	*/
    template<typename Value>
    size_t hash_table<Value>::size() const
    {
        return m_Size;
    }

    /*
	======================================
	hash_table<Value>::getNumberOfElements
	======================================
	*/
    template<typename Value>
    size_t hash_table<Value>::getNumberOfElements() const
    {
        return m_NumberOfElements;
    }

    /*
	==================================
	hash_table<Value>::getHashFunction
	==================================
	*/
    template<typename Value>
    hash_function hash_table<Value>::getHashFunction() const
    {
        return m_HashFunction;
    }


    template<typename Value>
    class hash_table_iterator
    {

        public:
            hash_table_iterator() = delete;
            hash_table_iterator(hash_table<Value> *hashtable, hash_entry<Value> *hashentry, uint64_t bucket, uint64_t count);

            hash_entry<Value> &operator*();
            hash_entry<Value> *operator->();

            hash_table_iterator<Value> &operator++();
            hash_table_iterator<Value> operator++(int);

            bool operator==(const hash_table_iterator<Value> &other) const;
            bool operator!=(const hash_table_iterator<Value> &other) const;

            uint64_t getCurrentBucket() const;

        private:
            hash_table<Value> *m_HashTable;
            hash_entry<Value> *m_CurrentEntry;
            uint64_t m_CurrentBucket;
            uint64_t m_Count;

    };

    /*
	===============================================
	hash_table_iterator<Value>::hash_table_iterator
	===============================================
	*/
    template<typename Value>
    hash_table_iterator<Value>::hash_table_iterator(hash_table<Value> *hashtable, hash_entry<Value> *hashentry, uint64_t bucket, uint64_t count)
        : m_HashTable(hashtable),
          m_CurrentEntry(hashentry),
          m_CurrentBucket(bucket),
          m_Count(count)
    { }

    /*
	=====================================
	hash_table_iterator<Value>::operator*
	=====================================
	*/
    template<typename Value>
    hash_entry<Value> &hash_table_iterator<Value>::operator*()
    {
        return *m_CurrentEntry;
    }

    /*
	======================================
	hash_table_iterator<Value>::operator->
	======================================
	*/
    template<typename Value>
    hash_entry<Value> *hash_table_iterator<Value>::operator->()
    {
        return m_CurrentEntry;
    }

    /*
	======================================
	hash_table_iterator<Value>::operator++
	======================================
	*/
    template<typename Value>
    hash_table_iterator<Value> &hash_table_iterator<Value>::operator++()
    {
        if(m_CurrentEntry->next != nullptr)
        {
            m_CurrentEntry = m_CurrentEntry->next;
            m_Count++;
        }
        else
        {
            m_CurrentBucket++;
            if(m_CurrentBucket >= m_HashTable->m_Size)
                return *this;

            m_Count = 0;
            while(m_CurrentBucket < m_HashTable->m_Size)
            {
                m_CurrentEntry = m_HashTable->m_Table[m_CurrentBucket];
                if(m_CurrentEntry != nullptr)
                    break;

                m_CurrentBucket++;
            }
        }

        return *this;
    }

    /*
	======================================
	hash_table_iterator<Value>::operator++
	======================================
	*/
    template<typename Value>
    hash_table_iterator<Value> hash_table_iterator<Value>::operator++(int)
    {
        auto temp = *this;

        ++(*this);

        return temp;
    }

    /*
	======================================
	hash_table_iterator<Value>::operator==
	======================================
	*/
    template<typename Value>
    bool hash_table_iterator<Value>::operator==(const hash_table_iterator<Value> &other) const
    {
        return  (m_CurrentBucket == other.m_CurrentBucket) &&
                (m_Count == other.m_Count);
    }

    /*
	======================================
	hash_table_iterator<Value>::operator!=
	======================================
	*/
    template<typename Value>
    bool hash_table_iterator<Value>::operator!=(const hash_table_iterator<Value> &other) const
    {
        return  (m_CurrentBucket != other.m_CurrentBucket) ||
                (m_Count != other.m_Count);
    }

    /*
	============================================
	hash_table_iterator<Value>::getCurrentBucket
	============================================
	*/
    template<typename Value>
    uint64_t hash_table_iterator<Value>::getCurrentBucket() const
    {
        return m_CurrentEntry;
    }

}

#endif