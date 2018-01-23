#pragma once
#include "Datum.h"
// #include "Dodavatel.h"
#include "structures\heap_monitor.h"

//class Dodavatel; // FORWARD DECLARATION

class Paleta
{
public:
	Paleta(int region, int hmotnost, Datum& datumDorucenia, Datum& datumPrichoduDoSkladu, bool jePaletaPrvejTriedy, std::string dodavatel);
	Paleta(Paleta& other);
	~Paleta();
	void setPrevzata(bool jePrevzata);
	bool getPrevzata();
	bool jePrvejTriedy();
	std::string getDodavatel();
	int getRegion();
	int getHmotnost();
	Datum* getDatumDorucenia();
	Datum * getDatumPrichoduDoSkladu();
	int getPrioritaDruheTriedy();
	Datum* getDatumNeprevzatia();
	void setDatumNeprevzatia(Datum& datumNeprevzatia);

private:
	//region kde ma byt paleta dorucena
	int region_;
	int hmotnost_;
	Datum* datumDorucenia_;
	Datum* datumPrichoduDoSkladu_;
	bool jePaletaPrvejTriedy_;
	bool jePrevzata_;
	std::string dodavatel_;
	Datum* datumNeprevzatia_;
};

