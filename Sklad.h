#pragma once
#include "Paleta.h"
#include "Vozidlo.h"
#include "OhlasenaZasielka.h"
#include "Region.h"
#include "Dodavatel.h"
#include "structures\list\array_list.h"
#include <cstdlib>
#include "structures\heap_monitor.h"
#include "structures\heap.h"


using namespace structures;

class Sklad
{
public:
	Sklad();
	~Sklad();
	void pridajVozidlo(Vozidlo& pridavaneVozidlo);
	void zaevidujZasielku(OhlasenaZasielka& zasielka);
	void naplnVozidlaPaletami();
	void pripravRegionyNaDnesPrveTriedy(Datum& aktualnyDatum);
	int getPocetRegionov();
	void pripravRegionyDruheTriedy();
	LinkedList<Vozidlo*>* getVozidlaNaCestePointer();
	void vratVozidlaDoSkladuAOdovzdajNeprevzatePaley(ArrayList<Dodavatel*>* dodavateliaPointer);
	void vyradVozidla(ArrayList<Vozidlo*>* vozidlaFiremnePointer);
	void vypisPaletyNaSklade();
	void vypisNezrealizovanePalety();
	void zapisRegionyDoSuboru();
	void zapisZaevidovanePaletyDoSuboru();
	void zapisPaletyAVozidlaNaCeste(Datum& dnesnyDatum);
	void zapisVozidlaVSklade();
	void zapisVozidlaNaOdpis();
	void zapisNezrealizovanePalety();
	void nacitajVozidlaDoSkladu(ArrayList<Vozidlo*>* vozidlaFiremnePointer);
	void nacitajVozidlaAPaletyNaCeste(ArrayList<Vozidlo*>* vozidlaFiremnePointer);
	void nacitajVozidlaNaOdpis(ArrayList<Vozidlo*>* vozidlaFiremnePointer);
	void nacitajNezrealizovanePalety();
	void nacitajRegiony();
	void nacitajPaletyDoSkladu();
	void nacitajRegionyPrveTriedyNaDnes();
	void nacitajRegionyDruheTriedy();

	Datum * strToDatum(std::string strDatum);
	
private:
	bool pokusSaNalozitPaletu(Paleta& vybranaPaleta);
    void nalozPaletyDruhejTriedy();

private:
	Heap<Vozidlo*>* vozidlaVSklade_;
	ArrayList<Region*> *regiony_;
	Heap<Region*>* regionyPodlaNajlahsichPalietPrvejTriedy_;
	Heap<Region*>* regionyPodlaNajlahsichPalietDruhejTriedy_;
	int MAXNosnost_;
	LinkedList<Paleta*>* nezrealizovanePalety_;
	LinkedList<Vozidlo*>* vozidlaNaCestu_;
	ArrayList<Paleta*>* paletyNaSklade_;
	ExplicitQueue<Vozidlo*>* vozidlaNaOdpis_;
	std::string priponaZapis_;
};

