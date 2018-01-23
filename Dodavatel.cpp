#include "Dodavatel.h"
#include "Paleta.h"
#include "structures\heap_monitor.h"

using namespace structures;

Dodavatel::Dodavatel(std::string nazov, std::string sidlo)
{
	obchodnyNazov_ = nazov;
	adresaSidla_ = sidlo;
	neprevzatePalety_ = new LinkedList<Paleta*>();
}

Dodavatel::Dodavatel(Dodavatel& other)
{
	obchodnyNazov_ = other.obchodnyNazov_;
	adresaSidla_ = other.adresaSidla_;
}

Dodavatel::~Dodavatel()
{
	for each (Paleta* aktPaleta in *neprevzatePalety_)
	{
		delete aktPaleta;
	}
	delete neprevzatePalety_;
}

std::string Dodavatel::getObchodnyNazov()
{
	return obchodnyNazov_;
}

std::string Dodavatel::getAdresaSidla()
{
	return adresaSidla_;
}

void Dodavatel::pridajNeprevzatuPaletu(Paleta & paleta)
{
	neprevzatePalety_->add(&paleta);
}

int Dodavatel::getPocetNeprevzatychPalietVDanomCasovomObdobi(Datum& datOd, Datum& datDo)
{
	int pocet = 0;
	for each (Paleta* aktPaleta in *neprevzatePalety_)
	{
		if ((datOd < *(aktPaleta->getDatumNeprevzatia()) && *(aktPaleta->getDatumNeprevzatia()) < datDo)
			  ||  datOd == *(aktPaleta->getDatumNeprevzatia()) 
			  ||  datDo == *(aktPaleta->getDatumNeprevzatia())) /// datum je bud medzi hranicami, alebo je v jednej alebo v druhej hranici
		{
			pocet++; // zrata kolko paliet padne do urceneho datumu
		}
	}
	return pocet;
}

LinkedList<Paleta*>* Dodavatel::getNeprevziatePaletyPointer()
{
	return neprevzatePalety_;
}


