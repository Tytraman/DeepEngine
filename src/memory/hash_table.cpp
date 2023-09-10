#include <DE/memory/hash_table.hpp>


namespace de {

	/*
	====================================
	HashTableIterator::HashTableIterator
	====================================
	*/
	HashTableIterator::HashTableIterator(List *lists, size_t currentList, size_t size)
		: m_Lists(lists),
		  m_CurrentList(currentList),
		  m_Size(size),
		  m_Index(0)
	{ }

	/*
	=============================
	HashTableIterator::operator++
	=============================
	*/
	HashTableIterator &HashTableIterator::operator++()
	{
		List *list;
		size_t len;

retry:
		list = m_Lists + m_CurrentList;
		len = list->getNumberOfElements();

		// Index toujours dans la liste.
		if(m_Index < len - 1) {
			++m_Index;
			return *this;
		}

		// L'index n'est plus dans la liste.
		m_Index = 0;
		++m_CurrentList;

		while(m_CurrentList < m_Size) {
			if(m_Lists[m_CurrentList].getNumberOfElements() > 0)
				break;
			
			++m_CurrentList;
		}

		return *this;
	}

	/*
	====================
	HashTable::HashTable
	====================
	*/
	HashTable::HashTable(size_t elementSize, size_t size, HashFunction hashFunction, HashTableFreeElementFunction freeElementFunction)
		: m_HashFunction(hashFunction),
		  m_FreeElementFunction(freeElementFunction),
		  m_Lists((List *) mem::alloc(sizeof(List) * size)),
		  m_Size(size),
		  m_ElementSize(elementSize),
		  m_NumberOfElements(0),
		  m_BucketsUsed(0)
	{
		size_t i;
		List list(sizeof(uint64_t) + elementSize);

		for(i = 0; i < size; ++i)
			memcpy(m_Lists + i, &list, sizeof(list));
	}

	/*
	=====================
	HashTable::insertCopy
	=====================
	*/
	mem_ptr HashTable::insertCopy(uint64_t key, mem_ptr value, bool updateIfExists)
	{
		uint64_t bucket = key % m_Size;

		// S'il y a déjà un élément avec la même clé.
		mem_ptr ptr = getPtr(key);
		if(ptr != nullptr) {
			if(updateIfExists) {
				// Stock la valeur associé à la clé.
				memcpy((uint64_t *) ptr + 1, value, m_ElementSize);
			}

			return ptr;
		}

		size_t len = m_Lists[bucket].getNumberOfElements();

		// Ajoute un emplacement mémoire dans la liste.
		if(!m_Lists[bucket].addNothing())
			return nullptr;

		if(len == 0)
			m_BucketsUsed++;

		// Récupère l'emplacement mémoire nouvellement créé.
		ptr = m_Lists[bucket].getPtr(m_Lists[bucket].getNumberOfElements() - 1);

		// Stock le hash de la clé.
		*((uint64_t *) ptr) = key;

		ptr = (uint64_t *) ptr + 1;

		// Stock la valeur associé à la clé.
		memcpy(ptr, value, m_ElementSize);

		m_NumberOfElements++;

		return ptr;
	}

	/*
	==================
	HashTable::destroy
	==================
	*/
	void HashTable::destroy(uint64_t key)
	{
		uint64_t bucket = key % m_Size;
		size_t len = m_Lists[bucket].getNumberOfElements();
		size_t i;
		mem_ptr ptr;

		for(i = 0; i < len; ++i) {
			ptr = m_Lists[bucket].getPtr(i);
			if(ptr != nullptr && *((uint64_t *) ptr) == key) {
				if(m_FreeElementFunction != nullptr)
					m_FreeElementFunction(ptr);

				m_Lists[bucket].remove(i);
				--m_NumberOfElements;

				break;
			}
		}
	}

	/*
	=================
	HashTable::getPtr
	=================
	*/
	mem_ptr HashTable::getPtr(const char *key)
	{
		uint64_t hash = m_HashFunction(key);

		return getPtr(hash);
	}

	/*
	=================
	HashTable::getPtr
	=================
	*/
	mem_ptr HashTable::getPtr(uint64_t key)
	{
		uint64_t bucket = key % m_Size;
		List &list = m_Lists[bucket];

		for(mem_ptr ptr : list) {
			if(*((uint64_t *) ptr) == key)
				return (uint64_t *) ptr + 1;
		}

		return nullptr;
	}

	/*
	=======================
	HashTable::freeElements
	=======================
	*/
	void HashTable::freeElements()
	{
		// Inutile de continuer la procédure s'il n'y a pas de fonction callback.
		if(m_FreeElementFunction == nullptr)
			return;

		size_t size = m_Size;
		size_t len;
		size_t i, j;
		mem_ptr ptr;

		// Parcourt toutes les listes de la HashTable.
		for(i = 0; i < size; ++i) {
			len = m_Lists[i].getNumberOfElements();

			// Parcourt tous les éléments de la liste.
			for(j = 0; j < len; ++j) {
				ptr = m_Lists[i].getPtr(j);
				if(ptr != nullptr)
					m_FreeElementFunction(ptr);
			}

			// Libère la mémoire de la liste.
			m_Lists[i].free();
		}
	}

	/*
	================
	HashTable::begin
	================
	*/
	HashTableIterator HashTable::begin()
	{
		size_t i;
		size_t size = m_Size;

		// Parcourt toutes les listes à partir du début pour en trouver une qui a un élément.
		for(i = 0; i < size; ++i) {
			if(m_Lists[i].getNumberOfElements() != 0)
				break;
		}

		return HashTableIterator(m_Lists, i, size);
	}

	

}
