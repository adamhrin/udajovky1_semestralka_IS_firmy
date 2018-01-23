#include "Region.h"
#include "structures\list\array_list.h"
#include "structures\priority_queue_unsorted_array_list.h"
#include "structures\heap_monitor.h"

using namespace structures;

Region::Region(int cislo)
{
	cislo_ = cislo;
	paletyPrvejTriedy_ = new ArrayList<Heap<Paleta*>*>();
	paletyDruhejTriedy_ = new Heap<Paleta*>();
}

Region::Region(Region& other)
{
	cislo_ = other.cislo_;
	paletyPrvejTriedy_ = other.paletyPrvejTriedy_;
	paletyDruhejTriedy_ = other.paletyDruhejTriedy_;
}

Region::Region()
{
}

Region::~Region()
{
	for each (Heap<Paleta*>* aktPriorFront in *paletyPrvejTriedy_)
	{
		int pocetPalietPrveTriedy = aktPriorFront->size();
		for (int i = 0; i < pocetPalietPrveTriedy; i++)
		{
			delete aktPriorFront->pop();
		}
		delete aktPriorFront;
	}
	delete paletyPrvejTriedy_;


	int pocetPalietDruheTriedy = paletyDruhejTriedy_->size();
	for (int i = 0; i < pocetPalietDruheTriedy; i++)
	{
		delete paletyDruhejTriedy_->pop();
	}
	delete paletyDruhejTriedy_;
}

int Region::getCislo()
{
	return cislo_;
}

/// O(n), n pocet prioritnych frontov paliet prvej triedy
void Region::pridajPaletuPrvejTriedy(int priorita, Paleta& paleta)
{
	int index = 0;
	for each (/*PriorityQueueUnsortedArrayList<Paleta*>**/ Heap<Paleta*>* aktPriorFront in *paletyPrvejTriedy_)
	{

		/// prisla mi paleta prvej triedy, ktora ma datum dorucenia. postupne prejdem vsetky PF
		/// aby som zistil, ci uz palety s rovnakym datumom dorucenia nemam na sklade. 
		/// ak ano, pridam tuto paletu k nim
		/// ak nie, idem dalej a kontorujem tuto podmienku
		/// ak sa ocitnem na konci zoznamu PF, vytvorim novy PF, a paletu dam tam (v tomto novom PF budu uz len palety s vacsim datumom dorucenia)
		if ((*(aktPriorFront->peek())->getDatumDorucenia()) == *paleta.getDatumDorucenia())
		{
			aktPriorFront->push(paleta.getHmotnost(), &paleta);
			return;
		}
		if ((*(aktPriorFront->peek())->getDatumDorucenia()) > *paleta.getDatumDorucenia())
		{
			break;
		}
		index++;
	}
	/*PriorityQueueUnsortedArrayList<Paleta*>**/ Heap<Paleta*>* novyPriorFront = new /*PriorityQueueUnsortedArrayList<Paleta*>();*/ Heap<Paleta*>();
	novyPriorFront->push(paleta.getHmotnost(), &paleta);
	if (index == paletyPrvejTriedy_->size())
	{
		paletyPrvejTriedy_->add(novyPriorFront);
	}
	else 
	{
		paletyPrvejTriedy_->insert(novyPriorFront, index);
	}
}

/// O(n), n pocet prioritnych frontov paliet druhej triedy
void Region::pridajPaletuDruhejTriedy(Paleta& paleta)
{
	paletyDruhejTriedy_->push(paleta.getPrioritaDruheTriedy(), &paleta);
}


Paleta * Region::peekPaletyPrvejTriedyNaNajmensiDatum()
{
	if (paletyPrvejTriedy_->size() == 0)
	{
		return nullptr;
	}
	return (*paletyPrvejTriedy_)[0]->peek();
}

int Region::getPocetPalietPrvejTriedyNaNajmensiDatum()
{
	return (*paletyPrvejTriedy_)[0]->size();
}

Paleta * Region::popPaletuPrvejTriedyNaNajmensiDatum()
{
	return (*paletyPrvejTriedy_)[0]->pop();
}

void Region::vymazPriorFrontNaNajmensiDatumPrveTriedy()
{
	delete paletyPrvejTriedy_->removeAt(0);
}

Paleta * Region::popPaletyDruhejTriedySNajvacsouPrioritou()
{
	return paletyDruhejTriedy_->pop();
}

Paleta * Region::peekPaletyDruhejTriedySNajvacsouPrioritou()
{
	if (paletyDruhejTriedy_->size() == 0)
	{
		return nullptr;
	}
	return paletyDruhejTriedy_->peek();
}

int Region::getPocetPalietDruhejTriedySNajvacsouPrioritou()
{
	return paletyDruhejTriedy_->size();
}
