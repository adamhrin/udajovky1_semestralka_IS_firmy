#pragma once
#include "string"
#include "structures\list\array_list.h"
#include "Paleta.h"
#include "structures\heap_monitor.h"

using namespace structures;

//class Paleta; // FORWARD DECLARATION
#include "structures\list\linked_list.h"

class Dodavatel
{
public:
	Dodavatel(std::string nazov, std::string sidlo);
	Dodavatel(Dodavatel & other);
	~Dodavatel();
	std::string getObchodnyNazov();
	std::string getAdresaSidla();
	void pridajNeprevzatuPaletu(Paleta& paleta);
	int getPocetNeprevzatychPalietVDanomCasovomObdobi(Datum & datOd, Datum & datDo);
	LinkedList<Paleta*>* getNeprevziatePaletyPointer();

private:
	std::string obchodnyNazov_;
	std::string adresaSidla_;
	LinkedList<Paleta*>* neprevzatePalety_;

};

