// main.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include "Datum.h"
#include "structures\array\array.h"
#include "structures\list\array_list.h"
//#include "structures\priority_queue_unsorted_array_list.h"
#include "VykonavacPrikazov.h"
#include "structures\heap_monitor.h"
#include "Sklad.h"
#include "Firma.h"
#include "structures\list\linked_list.h"

using namespace structures;

int main()
{
	initHeapMonitor(); /// detects memory leaks!

	Firma* firma = new Firma();

	firma->zacniProgram();

	delete firma;
	firma = nullptr;

	return 0;
}


