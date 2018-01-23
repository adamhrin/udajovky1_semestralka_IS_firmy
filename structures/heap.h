#pragma once

#include "priority_queue_list.h"
#include "list\array_list.h"
#include <exception>

namespace structures
{
	/// <summary> Prioritny front implementovany haldou. Implementujucim typom haldy je struktura ArrayList. </summary>
	/// <typeparam name = "T"> Typ dat ukladanych v prioritnom fronte. </typepram>
	template<typename T>
	class Heap : public PriorityQueueList<T>
	{
	public:
		/// <summary> Konstruktor. </summary>
		Heap();

		/// <summary> Kopirovaci konstruktor. </summary>
		/// <param name = "other"> Halda, z ktorej sa prevezmu vlastnosti. </param>
		Heap(const Heap<T>& other);

		/// <summary> Operacia klonovania. Vytvori a vrati duplikat haldy. </summary>
		/// <returns> Ukazovatel na klon struktury. </returns>
		Structure* clone() const override;

		/// <summary> Operator priradenia. </summary>
		/// <param name = "other"> Prioritny front implementovany zoznamom, z ktoreho ma prebrat vlastnosti. </param>
		/// <returns> Adresa, na ktorej sa tento prioritny front nachadza po priradeni. </returns>
		/// <exception cref="std::logic_error"> Vyhodena, ak vstupny parameter nie je halda. </exception>  
		/// <remarks> Vyuziva typovy operator priradenia. </remarks>
		virtual PriorityQueueList<T>& operator=(const PriorityQueueList<T>& other) override;

		/// <summary> Operator priradenia pre haldu. </summary>
		/// <param name = "other"> Halda, z ktorej ma prebrat vlastnosti. </param>
		/// <returns> Adresa, na ktorej sa tato halda nachadza po priradeni. </returns>
		virtual Heap<T>& operator=(const Heap<T>& other);

		/// <summary> Vlozi prvok s danou prioritou do haldy. </summary>
		/// <param name = "priority"> Priorita vkladaneho prvku. </param>
		/// <param name = "data"> Vkladany prvok. </param>
		void push(const int priority, const T& data) override;

		/// <summary> Odstrani prvok s najvacsou prioritou z haldy. </summary>
		/// <returns> Odstraneny prvok. </returns>
		/// <exception cref="std::logic_error"> Vyhodena, ak je halda prazdna. </exception>  
		T pop() override;

	protected:
		/// <summary> Vrati index v ArrayList-e, na ktorom sa nachadza prvok s najvacsou prioritou. </summary>
		/// <returns> Index prvku s najvacsou prioritou. </returns>
		/// <exception cref="std::logic_error"> Vyhodena, ak je ArrayList prazdny. </exception>  
		int indexOfPeek() const override;

	private:
		/// <summary> Vrati index v ArrayList-e, na ktorom by sa mal nachadzat otec daneho prvku. </summary>
		/// <param name = "index"> Index prvku, ktoreho otca chceme vratit. </param>
		/// <returns> Index, na ktorom ba sa mal nachadzat otec prvku. </returns>
		int getParentIndex(const int index);

		/// <summary> Vrati index v ArrayList-e, na ktorom by mal mat dany prvok syna s najvacsou prioritou. </summary>
		/// <param name = "index"> Index prvku, ktoreho syna chceme vratit. </param>
		/// <returns> Index, na ktorom by sa mal nachadzat syn prvku. </returns>
		int getGreaterSonIndex(const int index);
	};

	template<typename T>
	Heap<T>::Heap() :
		PriorityQueueList<T>(new ArrayList<PriorityQueueItem<T>*>())
	{
	}

	template<typename T>
	Heap<T>::Heap(const Heap<T>& other) :
		Heap<T>()
	{
		*this = other;
	}

	template<typename T>
	Structure* Heap<T>::clone() const
	{
		return new Heap<T>(*this);
	}

	template<typename T>
	inline PriorityQueueList<T>& Heap<T>::operator=(const PriorityQueueList<T>& other)
	{
		return *this = dynamic_cast<const Heap<T>&>(other);
	}

	template<typename T>
	inline Heap<T>& Heap<T>::operator=(const Heap<T>& other)
	{
		if (this != &other)
		{
			PriorityQueueList<T>::operator=(other);
		}
		return *this;
	}

	template<typename T>
	void Heap<T>::push(const int priority, const T& data)
	{
		PriorityQueueItem<T>* newItem = new PriorityQueueItem<T>(priority, data);
		list_->add(newItem);

		int curr = size() - 1;
		int parent = getParentIndex(curr);
		while (curr > 0 && (*list_)[curr]->getPriority() < (*list_)[parent]->getPriority())
		{
			DSRoutines::swap((*list_)[curr], (*list_)[parent]);
			curr = parent;
			parent = getParentIndex(curr);
		}
	}

	template<typename T>
	T Heap<T>::pop()
	{
		PriorityQueueItem<T>* root = list_->removeAt(0);
		T rootData = root->accessData();
		delete root;
		size_t sizeAfterPop = size();
		if (size() > 1)
		{
			list_->insert(list_->removeAt(sizeAfterPop - 1), 0);
		}

		int curr = 0;
		int greaterSon = getGreaterSonIndex(curr);
		while (greaterSon > 0 && greaterSon < size() && (*list_)[curr]->getPriority() > (*list_)[greaterSon]->getPriority())
		{
			DSRoutines::swap((*list_)[curr], (*list_)[greaterSon]);
			curr = greaterSon;
			greaterSon = getGreaterSonIndex(curr);
		}
		return rootData;
	}

	template<typename T>
	inline int Heap<T>::getParentIndex(const int index)
	{
		return (index + 1) / 2 - 1;
	}

	template<typename T>
	inline int Heap<T>::getGreaterSonIndex(const int index)
	{
		int leftIndex = (index + 1) * 2 - 1;
		int rightIndex = (index + 1) * 2;
		
		int leftPriority = leftIndex < size() ? (*list_)[leftIndex]->getPriority() : INT_MAX;
		int rightPriority = rightIndex < size() ? (*list_)[rightIndex]->getPriority() : INT_MAX;

		if (leftPriority == INT_MAX && rightPriority == INT_MAX)
		{
			return -1;
		} 
		else
		{
			return leftPriority < rightPriority ? leftIndex : rightIndex;
		}
	}

	template<typename T>
	inline int Heap<T>::indexOfPeek() const
	{
		if (isEmpty())
		{
			return -1;
		}
		return 0;
		// return isEmpty() ? -1 : 0;
	}
}