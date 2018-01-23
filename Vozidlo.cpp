#include "Vozidlo.h"
#include <iostream>
#include "structures\heap_monitor.h"


/// SPZ moze byt hociaka kombinacia prave 7 charov
Vozidlo::Vozidlo(std::string SPZ, int nosnost, Datum& datumZaevidovania)
{
	SPZ_ = SPZ;	
	nosnost_ = nosnost;
	opotrebenie_ = 0;
	datumZaevidovania_ = new Datum(datumZaevidovania); /// ma byt
	palety_ = new ExplicitQueue<Paleta*>();
	region_ = 0;
	momentalnaHmotnost_ = 0;
}

Vozidlo::Vozidlo(Vozidlo& other) 
{
	
	SPZ_ = other.SPZ_;
	nosnost_ = other.nosnost_;
	opotrebenie_ = other.opotrebenie_;
	region_ = other.region_;
	momentalnaHmotnost_ = other.momentalnaHmotnost_;
	datumZaevidovania_ = new Datum(*other.datumZaevidovania_);
	
	*palety_ = *other.palety_;
}
Vozidlo::~Vozidlo()
{
	//////////////////////////////////////////////////////
	delete datumZaevidovania_;

	while (!palety_->isEmpty())
	{
		delete palety_->pop();
	}
	delete palety_;
	
	datumZaevidovania_ = nullptr;
	palety_ = nullptr;

}

void Vozidlo::pridajOpotrebenie(int kolko)
{
	opotrebenie_ += kolko;
}

void Vozidlo::pridajDalsiuPaletu(Paleta& paleta)
{
	zvysMomentalnuHmotnost(paleta.getHmotnost());
	palety_->push(&paleta);
}

void Vozidlo::pridajPrvuPaletu(Paleta & paleta)
{
	setRegion(paleta.getRegion());
	zvysMomentalnuHmotnost(paleta.getHmotnost());
	palety_->push(&paleta);
}

std::string Vozidlo::getSPZ()
{
	return SPZ_;
}

int Vozidlo::getNosnost()
{
	return nosnost_;
}

int Vozidlo::getOpotrebenie()
{
	return opotrebenie_;
}

Datum* Vozidlo::getDatumZaevidovania()
{
	return datumZaevidovania_;
}

void Vozidlo::setRegion(int region)
{
	region_ = region;
}

int Vozidlo::getRegion()
{
	return region_;
}

void Vozidlo::zvysMomentalnuHmotnost(int oKolko)
{
	momentalnaHmotnost_ += oKolko;
}

int Vozidlo::getMomentalnaHmotnost()
{
	return momentalnaHmotnost_;
}

void Vozidlo::znizMomentalnuHmotnost(int oKolko)
{
	momentalnaHmotnost_ -= oKolko;
}

ExplicitQueue<Paleta*>* Vozidlo::getPaletyPointer()
{
	return palety_;
}

