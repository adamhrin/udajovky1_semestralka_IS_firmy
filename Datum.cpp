#include "Datum.h"
#include "structures\heap_monitor.h"

///Konstruktor skontroluje, ci takyto datum moze existovat, ak nie, vytvori sa defaultny datum 1.1.2000
Datum::Datum(int den, int mesiac, int rok)
{
	if ((den > 0) && (den <= getPocetDniVMesiaci(mesiac)))
	{
		den_ = den;
	}
	else
	{
		nastavDefault(); // nastavi sa default
		return;
	}

	if ((mesiac > 0) && (mesiac <= 12))
	{
		mesiac_ = mesiac;
	}
	else
	{
		nastavDefault(); // nastavi sa default
		return;
	}
	
	if ((rok > 1950) && (rok < 2100))
	{
		rok_ = rok;
	}
	else
	{
		nastavDefault(); // nastavi sa default
		return;
	}

}

Datum::Datum(Datum& other)
{
	den_ = other.den_;
	mesiac_ = other.mesiac_;
	rok_ = other.rok_;
}

std::string Datum::to_string()
{
	std::string denUpraveny;
	std::string mesiacUpraveny;

	if (den_ < 10)
	{
		denUpraveny = "0" + std::to_string(den_);
	}
	else
	{
		denUpraveny = std::to_string(den_);
	}

	if (mesiac_ < 10)
	{
		mesiacUpraveny = "0" + std::to_string(mesiac_);
	}
	else
	{
		mesiacUpraveny = std::to_string(mesiac_);
	}

	return denUpraveny + "." + mesiacUpraveny + "." + std::to_string(rok_);
}

bool Datum::operator==(Datum& druhy)
{
	return ((den_ == druhy.den_) && (mesiac_ == druhy.mesiac_) && (rok_ == druhy.rok_));
}

bool Datum::operator<(Datum& druhy)
{
	if (rok_ < druhy.rok_)
	{
		return true;
	}
	else if ((rok_ == druhy.rok_) && (mesiac_ < druhy.mesiac_))
	{
		return true;
	}
	else if ((rok_ == druhy.rok_) && (mesiac_ == druhy.mesiac_) && (den_ < druhy.den_))
	{
		return true;
	}
	return false;
}

bool Datum::operator>(Datum& druhy)
{
	if (rok_ > druhy.rok_)
	{
		return true;
	}
	else if ((rok_ == druhy.rok_) && (mesiac_ > druhy.mesiac_))
	{
		return true;
	}
	else if ((rok_ == druhy.rok_) && (mesiac_ == druhy.mesiac_) && (den_ > druhy.den_))
	{
		return true;
	}
	return false;
}

void Datum::pridajDen()
{
	if (mesiac_ == 12 && den_ == 31)
	{
		den_ = 1;
		mesiac_ = 1;
		rok_++;
	}
	else if (den_ == getPocetDniVMesiaci(mesiac_))
	{
		den_ = 1;
		mesiac_++;
	}
	else
	{
		den_++;
	}
}


int Datum::getPocetDniVMesiaci(int mesiac)
{
	switch (mesiac) 
	{
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;

	case 2:
		if (jePriestupnyRok())
		{
			return 29;
		}
		else
		{
			return 28;
		}

	default:
		return 31;
	}
		
}

bool Datum::jePriestupnyRok()
{
	if (rok_ % 400 == 0)
	{
		return true;
	}

	if (rok_ % 100 == 0)
	{
		return false;
	}

	return rok_ % 4 == 0;
}

void Datum::nastavDefault()
{
	den_ = 1;
	mesiac_ = 1;
	rok_ = 2000;
}

int Datum::getDen()
{
	return den_;
}

int Datum::getMesiac()
{
	return mesiac_;
}

int Datum::getRok()
{
	return rok_;
}

Datum& Datum::operator=(const Datum& other)
{
	if (this != &other)
	{
		den_ = other.den_;
		mesiac_ = other.mesiac_;
		rok_ = other.rok_;
	}
	return *this;
}

Datum::~Datum()
{
}
