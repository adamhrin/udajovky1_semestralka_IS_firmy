#include "OhlasenaZasielka.h"
#include "structures\heap_monitor.h"



OhlasenaZasielka::OhlasenaZasielka()
{
	paletyPrvejTriedy_ = new ArrayList<Paleta*>();
	paletyDruhejTriedy_ = new ArrayList<Paleta*>();
}

OhlasenaZasielka::OhlasenaZasielka(Datum& datumPrichodu, Dodavatel& dodavatel)
{
	datumPrichodu_ = new Datum(datumPrichodu);
	dodavatel_ = &dodavatel;
	paletyPrvejTriedy_ = new ArrayList<Paleta*>();
	paletyDruhejTriedy_ = new ArrayList<Paleta*>();
}

OhlasenaZasielka::OhlasenaZasielka(OhlasenaZasielka& other)
{
	datumPrichodu_ = new Datum(*other.datumPrichodu_);
	dodavatel_ = new Dodavatel(*other.dodavatel_);

	paletyPrvejTriedy_ = new ArrayList<Paleta*>();
	for each (Paleta* aktPaleta in *other.paletyPrvejTriedy_)
	{
		paletyPrvejTriedy_->add(new Paleta(*aktPaleta));
	}

	paletyDruhejTriedy_ = new ArrayList<Paleta*>();
	for each (Paleta* aktPaleta in *other.paletyDruhejTriedy_)
	{
		paletyDruhejTriedy_->add(new Paleta(*aktPaleta));
	}
}

OhlasenaZasielka::~OhlasenaZasielka()
{
	for each (Paleta* aktPaleta in *paletyPrvejTriedy_)
	{
		delete aktPaleta;
	}
	delete paletyPrvejTriedy_;

	for each (Paleta* aktPaleta in *paletyDruhejTriedy_)
	{
		delete aktPaleta;
	}
	delete paletyDruhejTriedy_;

	delete datumPrichodu_;
	paletyPrvejTriedy_ = nullptr;
	paletyDruhejTriedy_ = nullptr;
	datumPrichodu_ = nullptr;
}

void OhlasenaZasielka::pridajPaletu(Paleta & paleta)
{
	if (paleta.jePrvejTriedy())
	{
		paletyPrvejTriedy_->add(&paleta);
	} 
	else
	{
		paletyDruhejTriedy_->add(&paleta);
	}
}

void OhlasenaZasielka::setDatumPrichodu(Datum& datumPrichodu)
{
	datumPrichodu_ = new Datum(datumPrichodu);
}

void OhlasenaZasielka::setDodavatel(Dodavatel & dodavatel)
{
	dodavatel_ = &dodavatel;
}

Paleta * OhlasenaZasielka::getPaletaPrvejTriedy(int index)
{
	return (*paletyPrvejTriedy_)[index];
}

Paleta * OhlasenaZasielka::getPaletaDruhejTriedy(int index)
{
	return (*paletyDruhejTriedy_)[index];
}

int OhlasenaZasielka::getSizePaletyPrvejTriedy()
{
	return paletyPrvejTriedy_->size();
}

int OhlasenaZasielka::getSizePaletyDruhejTriedy()
{
	return paletyDruhejTriedy_->size();
}

Datum * OhlasenaZasielka::getDatumPrichodu()
{
	return datumPrichodu_;
}

Dodavatel * OhlasenaZasielka::getDodavatel()
{
	return dodavatel_;
}

int OhlasenaZasielka::getPocetPalietPrvejTriedy()
{
	return paletyPrvejTriedy_->size();
}

int OhlasenaZasielka::getPocetPalietDruhejTriedy()
{
	return paletyDruhejTriedy_->size();
}
