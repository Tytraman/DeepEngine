#ifndef __DEEP_ENGINE_LIST_HPP__
#define __DEEP_ENGINE_LIST_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>

namespace de {

	/// @class	List
	/// @brief	Permet de stocker une liste d'éléments de façon dynamique.
	/// @remark Cette classe a été créé avec comme idée de ne stocker que des valeurs "simples", c'est-à-dire des types primitifs ou des structures.
	///			Le stockage d'objets complexes est possible mais l'ajout de ceux-ci dans une liste sera donc plus long.
	class DE_API List {

		private:
			mem_ptr m_Data;				///< Bloc de données brutes contenant la liste des éléments.
			size_t m_ElementSize;		///< Taille en octet d'un seul élément de la liste.
			size_t m_NumberOfElements;	///< Contient le nombre d'éléments présent dans la liste.
			size_t m_Capacity;			///< Contient la capacité actuelle de la liste, permettant d'économiser une /c réallocation en allouant plus d'espace que nécessaire.
			uint16_t m_CapacityStep;	///< Contient le nombre d'éléments maximum ajoutés à la capacité lorsque celle-ci devient saturée.

		public:
			List();
			List(size_t elementSize, uint16_t capacityStep = 10);

			/// @brief			Ajoute dans la liste une copie de l'élément pointé.
			/// @param toAdd	Pointeur de l'élément à ajouter.
			/// @return			/c true si la procédure a réussi.
			bool addCopy(void *toAdd);

			/// @brief			Copie l'élément à l'index spécifié vers l'adresse passée.
			/// @param index	Index de l'élément à récupérer.
			/// @return			/c true si l'élément a pu être récupéré.
			bool getCopy(size_t index, mem_ptr dest);

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

			/// @brief Libère la mémoire utilisée par la liste.
			void free();
			

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
			MemoryStatus::Code growIfNeeded();

	};

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