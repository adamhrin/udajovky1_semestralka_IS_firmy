#include "Paleta.h"
#include "structures\heap_monitor.h"




Paleta::Paleta(int region, int hmotnost, Datum& datumDorucenia, Datum& datumPrichoduDoSkladu, bool jePaletaPrvejTriedy, std::string dodavatel)
{
	region_ = region;
	hmotnost_ = hmotnost;
	datumDorucenia_ = new Datum(datumDorucenia);
	datumPrichoduDoSkladu_ = new Datum(datumPrichoduDoSkladu);
	jePaletaPrvejTriedy_ = jePaletaPrvejTriedy;
	dodavatel_ = dodavatel;
	datumNeprevzatia_ = new Datum(1, 1, 1980);
}

Paleta::Paleta(Paleta& other)
{
	region_ = other.region_;
	hmotnost_ = other.hmotnost_;
	datumDorucenia_ = new Datum(*other.datumDorucenia_);
	datumPrichoduDoSkladu_ = new Datum(*other.datumPrichoduDoSkladu_);
	jePaletaPrvejTriedy_ = other.jePaletaPrvejTriedy_;
	dodavatel_ = other.dodavatel_;
	datumNeprevzatia_ = new Datum(*other.datumNeprevzatia_);
}

Paleta::~Paleta()
{
	delete datumDorucenia_;
	delete datumPrichoduDoSkladu_;
	delete datumNeprevzatia_;
	datumDorucenia_ = nullptr;
	datumPrichoduDoSkladu_ = nullptr;
	datumNeprevzatia_ = nullptr;
}

void Paleta::setPrevzata(bool jePrevzata)
{
	jePrevzata_ = jePrevzata;
}

bool Paleta::getPrevzata()
{
	return jePrevzata_;
}

bool Paleta::jePrvejTriedy()
{
	return jePaletaPrvejTriedy_;
}

std::string Paleta::getDodavatel()
{
	return dodavatel_;
}

int Paleta::getRegion()
{
	return region_;
}

int Paleta::getHmotnost()
{
	return hmotnost_;
}

Datum * Paleta::getDatumDorucenia()
{
	return datumDorucenia_;
}

Datum * Paleta::getDatumPrichoduDoSkladu()
{
	return datumPrichoduDoSkladu_;
}

int Paleta::getPrioritaDruheTriedy()
{
	/// priorita je datum prichodu palety do skladu * velke cislo + hmotnost palety
	int priorita = (datumPrichoduDoSkladu_->getRok() * 10000)
		+ (datumPrichoduDoSkladu_->getMesiac() * 100)
		+ (datumPrichoduDoSkladu_->getDen())
		+ hmotnost_;
	return priorita;
}

Datum * Paleta::getDatumNeprevzatia()
{
	return datumNeprevzatia_;
}

void Paleta::setDatumNeprevzatia(Datum & datumNeprevzatia)
{
	*datumNeprevzatia_ = datumNeprevzatia;
}




