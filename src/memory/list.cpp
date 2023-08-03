#include <DE/memory/list.hpp>

namespace de {

	/*
	==========
	List::List
	==========
	*/
	List::List()
		: m_Data(nullptr), m_ElementSize(0), m_NumberOfElements(0), m_Capacity(0), m_CapacityStep(10)
	{ }

	/*
	==========
	List::List
	==========
	*/
	List::List(size_t elementSize, uint16_t capacityStep)
		: m_Data(nullptr), m_ElementSize(elementSize), m_NumberOfElements(0), m_Capacity(0), m_CapacityStep(capacityStep)
	{ }

	/*
	=============
	List::addCopy
	=============
	*/
	bool List::addCopy(void *toAdd)
	{
		if(growIfNeeded() != MemoryStatus::OK)
			return false;
		
		memcpy((uint8_t *) m_Data + m_NumberOfElements * m_ElementSize, toAdd, m_ElementSize);
		m_NumberOfElements++;

		return true;
	}

	/*
	=============
	List::getCopy
	=============
	*/
	bool List::getCopy(size_t index, mem_ptr dest)
	{
		if(index >= m_NumberOfElements)
			return false;

		memcpy(dest, (uint8_t *) m_Data + index * m_ElementSize, m_ElementSize);

		return true;
	}

	/*
	============
	List::getPtr
	============
	*/
	void List::setCopy(size_t index, void *toSet)
	{
		memcpy((uint8_t *) m_Data + index, toSet, m_ElementSize);
	}

	/*
	==========
	List::find
	==========
	*/
	size_t List::find(void *toSearch)
	{
		uint8_t *ptr = (uint8_t *) m_Data;
		size_t elementSize = m_ElementSize;
		size_t numberOfElements = m_NumberOfElements;
		size_t index = 0;

		while(index < numberOfElements) {
			if(memcmp(ptr, toSearch, elementSize) == 0)
				return index;

			ptr += elementSize;
			index++;
		}

		return nothing;
	}

	/*
	==========
	List::free
	==========
	*/
	void List::free()
	{
		mem::freeNoTrack(m_Data);
		m_Data = nullptr;
		m_NumberOfElements = 0;
	}

	/*
	============
	List::remove
	============
	*/
	void List::remove(size_t index)
	{
		// Obtient le nombre d'éléments pour atteindre la fin de la liste.
		size_t diff = m_NumberOfElements - index;

		memmove((uint8_t *) m_Data + index, (uint8_t *) m_Data + index + 1, diff * m_ElementSize);
	}

	/*
	==================
	List::growIfNeeded
	==================
	*/
	MemoryStatus::Code List::growIfNeeded()
	{
		// Si le nombre d'éléments présents dans la liste est supérieur à la capacité,
		// on augmente celle-ci du pas attribué.
		if(m_NumberOfElements >= m_Capacity) {
			size_t newCapacity = m_Capacity + m_CapacityStep;
			mem_ptr ptr = mem::reallocNoTrack(m_Data, newCapacity * m_ElementSize);

			if(ptr == nullptr)
				return MemoryStatus::ReallocError;

			m_Data = ptr;
			m_Capacity = newCapacity;
		}

		return MemoryStatus::OK;
	}

}
