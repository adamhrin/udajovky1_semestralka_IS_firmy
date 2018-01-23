#pragma once
#include "Datum.h"
#include "Paleta.h"
#include "structures\list\array_list.h"
#include "structures\queue\explicit_queue.h"
#include "structures\heap_monitor.h"
#include "Dodavatel.h"

using namespace structures;

class Vozidlo
{
public:
	Vozidlo(std::string SPZ, int nosnost_, Datum& datumZaevidovania);
	Vozidlo(Vozidlo& other);
	~Vozidlo();
	void pridajOpotrebenie(int kolko);
	void pridajDalsiuPaletu(Paleta& paleta);
	void pridajPrvuPaletu(Paleta& paleta);
	std::string getSPZ();
	int getNosnost();
	int getOpotrebenie();
	Datum * getDatumZaevidovania();
	void setRegion(int region);
	int getRegion();
	void zvysMomentalnuHmotnost(int oKolko);
	int getMomentalnaHmotnost();
	void znizMomentalnuHmotnost(int oKolko);

	ExplicitQueue<Paleta*>* getPaletyPointer();

private:
	/// SPZ je cislo i = 1,2,3...
	std::string SPZ_;
	int nosnost_;
	int opotrebenie_;
	int region_;
	int momentalnaHmotnost_;
	Datum* datumZaevidovania_;
	ExplicitQueue<Paleta*>* palety_;

};

