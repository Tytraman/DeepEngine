#ifndef __DEEP_ENGINE_HASH_TABLE_HPP__
#define __DEEP_ENGINE_HASH_TABLE_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>
#include <DE/string_utils.hpp>
#include <DE/memory/list.hpp>

namespace de {

	using HashFunction = uint64_t (*)(const char *str);
	using HashTableFreeElementFunction = void (*)(mem_ptr element);

	class DE_API HashTableIterator {

		public:
			HashTableIterator(List *lists, size_t currentList, size_t size);

			HashTableIterator &operator++();
			HashTableIterator operator++(int);

			mem_ptr operator*();

			bool operator==(const HashTableIterator &other) const;
			bool operator!=(const HashTableIterator &other) const;

		private:
			List *m_Lists;
			size_t m_CurrentList;
			size_t m_Size;
			size_t m_Index;

	};

	/*
	=============================
	HashTableIterator::operator++
	=============================
	*/
	inline HashTableIterator HashTableIterator::operator++(int)
	{
		HashTableIterator it = *this;

		++(*this);

		return it;
	}

	/*
	============================
	HashTableIterator::operator*
	============================
	*/
	inline mem_ptr HashTableIterator::operator*()
	{
		return m_Lists[m_CurrentList].getPtr(m_Index);
	}

	/*
	=============================
	HashTableIterator::operator==
	=============================
	*/
	inline bool HashTableIterator::operator==(const HashTableIterator &other) const
	{
		return m_CurrentList == other.m_CurrentList;
	}

	/*
	=============================
	HashTableIterator::operator!=
	=============================
	*/
	inline bool HashTableIterator::operator!=(const HashTableIterator &other) const
	{
		return m_CurrentList != other.m_CurrentList;
	}

	class DE_API HashTable {

		public:
			HashTable(size_t elementSize, size_t size, HashFunction hashFunction = StringUtils::hash, HashTableFreeElementFunction freeElementFunction = nullptr);

			mem_ptr insertCopy(const char *key, mem_ptr value, bool updateIfExists = true);
			mem_ptr insertCopy(uint64_t key, mem_ptr value, bool updateIfExists = true);

			void destroy(const char *key);
			void destroy(uint64_t key);

			mem_ptr getPtr(const char *key);
			mem_ptr getPtr(uint64_t key);

			size_t size() const;

			void freeElements();

			//===== Iterator =====// 

			HashTableIterator begin();
			HashTableIterator end();

			//===== Getters =====// 

			size_t getNumberOfElements() const;

		private:
			HashFunction m_HashFunction;
			HashTableFreeElementFunction m_FreeElementFunction;
			List *m_Lists;
			size_t m_Size;
			size_t m_ElementSize;
			size_t m_NumberOfElements;
			size_t m_BucketsUsed;

	};

	/*
	=====================
	HashTable::insertCopy
	=====================
	*/
	inline mem_ptr HashTable::insertCopy(const char *key, mem_ptr value, bool updateIfExists)
	{
		return insertCopy(m_HashFunction(key), value, updateIfExists);
	}

	/*
	==================
	HashTable::destroy
	==================
	*/
	inline void HashTable::destroy(const char *key)
	{
		destroy(m_HashFunction(key));
	}

	/*
	===============
	HashTable::size
	===============
	*/
	inline size_t HashTable::size() const
	{
		return m_Size;
	}

	/*
	==============
	HashTable::end
	==============
	*/
	inline HashTableIterator HashTable::end()
	{
		return HashTableIterator(m_Lists, m_Size, m_Size);
	}

	/*
	==============================
	HashTable::getNumberOfElements
	==============================
	*/
	inline size_t HashTable::getNumberOfElements() const
	{
		return m_NumberOfElements;
	}

}

#endif