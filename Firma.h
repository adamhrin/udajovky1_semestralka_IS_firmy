#pragma once
#include "Sklad.h"
//#include "Dodavatel.h"
#include "OhlasenaZasielka.h"
#include "structures\heap_monitor.h"

class Firma
{
public:
	Firma();
	~Firma();
	void zacniProgram();
	

private:

	void vypisDnesnyDatum();
	void zaradDoVozovehoParku();
	void vypisZoznamVozidiel();
	void pridajDodavatela();
	void vypisDodavatelov();
	void pridajDen();
	void ohlasZasielku();
	void zaevidujZasielku();
	void naplnVozidlaPaletami();
	void odovzdajPaletyZakaznikom();
	void vratVozidlaDoSkladuAOdovzdajNeprevzatePaley();
	void vyradVozidla();
	void vypisPaletyNaSklade();
	void vyhladajDodavatelaNajviacNeprevzatychPaliet();
	void vypisNezrealizovanePalety();
	void nacitajZoSuboru();
	void zapisDoSuboru();
	Datum* strToDatum(std::string strDatum);
	void nacitajNeprezvatePalety();

private:
	Sklad* sklad_;
	Datum* aktualnyDatum_;
	ArrayList<Vozidlo*>* vozidlaFiremne_;
	ArrayList<Dodavatel*>* dodavatelia_;
	ArrayList<OhlasenaZasielka*>* ohlaseneZasielky_;
	int flagPocetStlaceni6_;
	int flagPocetStlaceni7_;
	int flagPocetStlaceni8_;
	int flagPocetStlaceni9_;
	int pocitadloPrikazov_;
	int momentalnyPocetPrikazov_;
	std::string priponaZapis_;

};

