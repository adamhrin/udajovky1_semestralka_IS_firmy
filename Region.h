#pragma once
#include "structures\list\array_list.h"
//#include "structures\priority_queue_unsorted_array_list.h"
#include "Paleta.h"
#include "structures\heap_monitor.h"
#include "structures\heap.h"

using namespace structures;

class Region
{
public:
	Region(int cislo);
	Region(Region & other);
	Region();
	~Region();
	int getCislo();
	void pridajPaletuPrvejTriedy(int priorita, Paleta& paleta);
	void pridajPaletuDruhejTriedy(Paleta & paleta);
	void vymazPriorFrontNaNajmensiDatumPrveTriedy();
	Paleta* popPaletuPrvejTriedyNaNajmensiDatum();
	Paleta* peekPaletyPrvejTriedyNaNajmensiDatum();
	int getPocetPalietPrvejTriedyNaNajmensiDatum();

	Paleta* popPaletyDruhejTriedySNajvacsouPrioritou();
	Paleta* peekPaletyDruhejTriedySNajvacsouPrioritou();
	int getPocetPalietDruhejTriedySNajvacsouPrioritou();
	

private:
	int cislo_;
	ArrayList<Heap<Paleta*>*>* paletyPrvejTriedy_;
	Heap<Paleta*>* paletyDruhejTriedy_;
};

