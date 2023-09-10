#ifndef __DEEP_ENGINE_LIST_HPP__
#define __DEEP_ENGINE_LIST_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>

namespace de {

	class DE_API ListIterator {

		public:
			ListIterator(mem_ptr ptr, size_t elementSize);

			ListIterator &operator++();
			ListIterator operator++(int);

			ListIterator &operator--();
			ListIterator operator--(int);

			mem_ptr operator[](size_t index);
			mem_ptr operator*();

			bool operator==(const ListIterator &other) const;
			bool operator!=(const ListIterator &other) const;

		private:
			mem_ptr m_Ptr;
			size_t m_ElementSize;

	};

	/*
	========================
	ListIterator::operator++
	========================
	*/
	inline ListIterator &ListIterator::operator++()
	{
		m_Ptr = (uint8_t *) m_Ptr + m_ElementSize;

		return *this;
	}

	/*
	========================
	ListIterator::operator++
	========================
	*/
	inline ListIterator ListIterator::operator++(int)
	{
		// Retourne une copie de l'état avant modification de l'itérateur.
		ListIterator it = *this;

		++(*this);

		return it;
	}

	/*
	========================
	ListIterator::operator--
	========================
	*/
	inline ListIterator &ListIterator::operator--()
	{
		m_Ptr = (uint8_t *) m_Ptr - m_ElementSize;

		return *this;
	}

	/*
	========================
	ListIterator::operator--
	========================
	*/
	inline ListIterator ListIterator::operator--(int)
	{
		// Retourne une copie de l'état avant modification de l'itérateur.
		ListIterator it = *this;

		--(*this);

		return it;
	}

	/*
	========================
	ListIterator::operator[]
	========================
	*/
	inline mem_ptr ListIterator::operator[](size_t index)
	{
		return (uint8_t *) m_Ptr + index * m_ElementSize;
	}

	/*
	=======================
	ListIterator::operator*
	=======================
	*/
	inline mem_ptr ListIterator::operator*()
	{
		return m_Ptr;
	}

	/*
	========================
	ListIterator::operator==
	========================
	*/
	inline bool ListIterator::operator==(const ListIterator &other) const
	{
		return m_Ptr == other.m_Ptr;
	}

	/*
	========================
	ListIterator::operator!=
	========================
	*/
	inline bool ListIterator::operator!=(const ListIterator &other) const
	{
		return m_Ptr != other.m_Ptr;
	}

	/// @class	List
	/// @brief	Permet de stocker une liste d'éléments de façon dynamique.
	/// @remark Cette classe a été créé avec comme idée de ne stocker que des valeurs "simples", c'est-à-dire des types primitifs ou des structures.
	///			Le stockage d'objets complexes est possible mais l'ajout de ceux-ci dans une liste sera donc plus long.
	class DE_API List {

		public:
			List();
			List(size_t elementSize, uint16_t capacityStep = 10);

			/// @brief       Ajoute dans la liste une copie de l'élément pointé.
			/// @param toAdd Pointeur de l'élément à ajouter.
			/// @return      /c true si la procédure a réussi.
			bool addCopy(void *toAdd);

			/// @brief  S'assure d'ajouter un emplacement mémoire si nécessaire.
			/// @return \c false si une erreur est survenue.
			bool addNothing();

			/// @brief                  Réserve un nombre d'emplacements mémoire minimum.
			/// @param numberOfElements Le nombre d'éléments minimum que la liste doit contenir.
			/// @return                 \c false si une erreur est survenue.
			bool reserve(size_t numberOfElements);

			/// @brief			Copie l'élément à l'index spécifié vers l'adresse passée.
			/// @param index	Index de l'élément à récupérer.
			/// @return			/c true si l'élément a pu être récupéré.
			bool getCopy(size_t index, mem_ptr dest);

			mem_ptr getPtr(size_t index);

			/// @brief			Défini la valeur d'un élément de la liste.
			/// @param index	Index de l'élément à définir.
			/// @param toSet	Pointeur de l'élément à copier.
			void setCopy(size_t index, void *toSet);

			/// @brief			Supprime l'élément à l'index spécifié.
			/// @param index	Index de l'élément à supprimer.
			void remove(size_t index);

			/// @brief			Cherche si un élément est dans la liste.
			/// @param toSearch Pointeur de l'élément à chercher.
			/// @return			Index dans la liste de l'élément trouvé ou /ref nothing si aucun élément n'a été trouvé.
			size_t find(void *toSearch);

			void empty();

			/// @brief Libère la mémoire utilisée par la liste.
			void free();

			//===== Iterator =====// 

			ListIterator begin();
			ListIterator end();

			//===== Getters =====// 

			/// @brief	Récupère la taille en octet d'un élément.
			/// @return La taille en octet d'un élément.
			size_t getElementSize() const;

			/// @brief	Récupère le nombre d'éléments présents dans la liste.
			/// @return Le nombre d'éléments dans la liste.
			size_t getNumberOfElements() const;

			/// @brief	Récupère la capacité actuelle de la liste.
			/// @return La capacité actuelle de la liste.
			size_t getCapacity() const;

			mem_ptr getData();

			//===== Getters =====// 

			void setElementSize(size_t size);

		private:
			mem_ptr  m_Data;                ///< Bloc de données brutes contenant la liste des éléments.
			size_t   m_ElementSize;         ///< Taille en octet d'un seul élément de la liste.
			size_t   m_NumberOfElements;    ///< Contient le nombre d'éléments présent dans la liste.
			size_t   m_Capacity;            ///< Contient la capacité actuelle de la liste, permettant d'économiser une /c réallocation en allouant plus d'espace que nécessaire.
			uint16_t m_CapacityStep;        ///< Contient le nombre d'éléments maximum ajoutés à la capacité lorsque celle-ci devient saturée.

			MemoryStatus::Code growIfNeeded();

	};

	/*
	===========
	List::empty
	===========
	*/
	inline void List::empty()
	{
		m_NumberOfElements = 0;
	}

	/*
	===========
	List::begin
	===========
	*/
	inline ListIterator List::begin()
	{
		return ListIterator(m_Data, m_ElementSize);
	}

	/*
	=========
	List::end
	=========
	*/
	inline ListIterator List::end()
	{
		return ListIterator((uint8_t *) m_Data + m_NumberOfElements * m_ElementSize, m_ElementSize);
	}

	/*
	====================
	List::getElementSize
	====================
	*/
	inline size_t List::getElementSize() const
	{
		return m_ElementSize;
	}

	/*
	=========================
	List::getNumberOfElements
	=========================
	*/
	inline size_t List::getNumberOfElements() const
	{
		return m_NumberOfElements;
	}

	/*
	=================
	List::getCapacity
	=================
	*/
	inline size_t List::getCapacity() const
	{
		return m_Capacity;
	}

	/*
	=============
	List::getData
	=============
	*/
	inline mem_ptr List::getData()
	{
		return m_Data;
	}

	/*
	====================
	List::setElementSize
	====================
	*/
	inline void List::setElementSize(size_t size)
	{
		m_ElementSize = size;
	}

}

#endif