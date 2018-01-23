#pragma once
#include <string>
#include <cstdio>
#include "structures\heap_monitor.h"

class Datum
{
public:
	Datum(int den, int mesiac, int rok);
	Datum(Datum& other);
	~Datum();
	std::string to_string();
	bool operator==(Datum & druhy);
	bool operator<(Datum& druhy);
	bool operator>(Datum& druhy);
	void pridajDen();
	int getPocetDniVMesiaci(int mesiac);
	bool jePriestupnyRok();
	void nastavDefault();
	int getDen();
	int getMesiac();
	int getRok();

	Datum & operator=(const Datum & other);

private:
	/// den vytvorenia datumu
	int den_;

	/// mesiac vytvorenia datumu
	int mesiac_;

	/// rok vytvorenia datumu
	int rok_;
};


