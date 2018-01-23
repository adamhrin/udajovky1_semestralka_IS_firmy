#pragma once
#include "Datum.h"
#include "structures\list\array_list.h"
#include "Paleta.h"
#include "Dodavatel.h"
#include "structures\heap_monitor.h"

using namespace structures;

class OhlasenaZasielka
{
public:
	OhlasenaZasielka();
	OhlasenaZasielka(Datum & datumPrichodu, Dodavatel & dodavatel);
	OhlasenaZasielka(OhlasenaZasielka& other);
	~OhlasenaZasielka();
	void pridajPaletu(Paleta& paleta);
	void setDatumPrichodu(Datum& datumPrichodu);
	void setDodavatel(Dodavatel& dodavatel);
	Paleta* getPaletaPrvejTriedy(int index);
	Paleta* getPaletaDruhejTriedy(int index);
	int getSizePaletyPrvejTriedy();
	int getSizePaletyDruhejTriedy();
	Datum* getDatumPrichodu();
	Dodavatel* getDodavatel();
	int getPocetPalietPrvejTriedy();
	int getPocetPalietDruhejTriedy();

private:
	Datum* datumPrichodu_;
	Dodavatel* dodavatel_;
	ArrayList<Paleta*>* paletyPrvejTriedy_;
	ArrayList<Paleta*>* paletyDruhejTriedy_;
};

