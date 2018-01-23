#include "Firma.h"
#include "Sklad.h"
#include "Vozidlo.h"
#include "structures\priority_queue_unsorted_array_list.h"
#include <iostream>
#include <fstream>
#include <time.h>  
#include "structures\heap_monitor.h"

using namespace structures;

Firma::Firma()
{

	/////////////////////////////////////
	priponaZapis_ = ""; ///"Zapis"
	/////////////////////////////////////

	sklad_ = new Sklad();
	aktualnyDatum_ = new Datum(12, 12, 2010);
	vozidlaFiremne_ = new ArrayList<Vozidlo*>();
	dodavatelia_ = new ArrayList<Dodavatel*>();
	ohlaseneZasielky_ = new ArrayList<OhlasenaZasielka*>();
	flagPocetStlaceni6_ = 0;
	flagPocetStlaceni7_ = 0;
	flagPocetStlaceni8_ = 0;
	flagPocetStlaceni9_ = 0;
	pocitadloPrikazov_ = 0;
	momentalnyPocetPrikazov_ = 0;
	
}

/// O(c*n), n pocet prvkov v jednotlivych zoznamoch, c pocet zoznamov
Firma::~Firma()
{
	for each (Vozidlo* aktVoz in *vozidlaFiremne_) // vymaze najprv vsetky objekty, na ktore v zozname pointuje, az potom vymaze samotny pointer
	{
		delete aktVoz;
	}
	delete vozidlaFiremne_;

	for each (OhlasenaZasielka* aktZas in *ohlaseneZasielky_)
	{
		delete aktZas;
	}
	delete ohlaseneZasielky_;

	for each (Dodavatel* aktDod in *dodavatelia_)
	{
		delete aktDod;
	}
	delete dodavatelia_;

	delete aktualnyDatum_;
	delete sklad_;

	sklad_ = nullptr;
	aktualnyDatum_ = nullptr;
	vozidlaFiremne_ = nullptr;
	dodavatelia_ = nullptr;
	ohlaseneZasielky_ = nullptr;
}

void Firma::zacniProgram()
{
	std::cout << std::endl;
	std::cout << "**************************************************\n";
	std::cout << "> Dobry den, vitajte v informacnom systeme FRIm\n";
	std::cout << "**************************************************\n";
	std::cout << std::endl;
	vypisDnesnyDatum();

	while (1)
	{
		std::cout << "\n> Stlacte tlacicu\n";
		int volba = 0;
		std::cin >> volba;
		switch (volba)
		{
		/// tlacidlo 0 - VYPISE DNESNY DATUM
		case 0:
		{
			pocitadloPrikazov_++;
			vypisDnesnyDatum();
			break;
		}
		/// tlacidlo 1 
		case 1:
		{
			pocitadloPrikazov_++;
			zaradDoVozovehoParku();
			break;
		}
		/// tlacidlo 2 
		case 2:
		{
			pocitadloPrikazov_++;
			vypisZoznamVozidiel();
			break;
		}

		/// tlacidlo 3 
		case 3:
		{
			pocitadloPrikazov_++;
			pridajDodavatela();
			break;
		}

		/// tlacidlo 4 
		case 4:
		{
			pocitadloPrikazov_++;
			vypisDodavatelov();
			break;
		}

		/// tlacidlo 5
		case 5:
		{
			pocitadloPrikazov_++;
			ohlasZasielku();
			break;
		}

		/// tlacidlo 6
		case 6:
		{
			pocitadloPrikazov_++;
			zaevidujZasielku();
			break;
		}
		/// tlacidlo 7
		case 7:
		{
			pocitadloPrikazov_++;
			naplnVozidlaPaletami();
			break;
		}

		/// tlacidlo 8
		case 8:
		{
			pocitadloPrikazov_++;
			odovzdajPaletyZakaznikom();
			break;
		}

		/// tlacidlo 9
		case 9:
		{
			pocitadloPrikazov_++;
			vratVozidlaDoSkladuAOdovzdajNeprevzatePaley();
			break;
		}

		/// tlacidlo 10
		case 10:
		{
			pocitadloPrikazov_++;
			vyradVozidla();
			break;
		}

		/// tlacidlo 11
		case 11:
		{
			pocitadloPrikazov_++;
			vypisPaletyNaSklade();
			break;
		}

		/// tlacidlo 12
		case 12:
		{
			pocitadloPrikazov_++;
			vyhladajDodavatelaNajviacNeprevzatychPaliet();
			break;
		}

		/// tlacidlo 13
		case 13:
		{
			pocitadloPrikazov_++;
			vypisNezrealizovanePalety();
			break;
		}
		/// tlacidlo 14 
		case 14:
		{	
			pocitadloPrikazov_++;
			pridajDen();
			break;
		}

		/// tlacidlo 15
		case 15:
		{
			pocitadloPrikazov_++;
			nacitajZoSuboru();
			break;
		}

		/// tlacidlo 16
		case 16:
		{
			pocitadloPrikazov_++;
			zapisDoSuboru();
			break;
		}

		/// tlacidlo 20 ukonci program
		case 20:
			return;

		default:
			std::cout << "Not implemented yet!\n";
		}
	}
	
}

/// TLACIDLO 0 - vypise dnesny datum
void Firma::vypisDnesnyDatum()
{
	std::cout << "> Dnes je: " << aktualnyDatum_->to_string() << std::endl;
}

///TLACIDLO 1
/// zaradi auto do vozoveho parku, kontroluje ci take auto uz neexistuje a ci je SPZ v spravnom formate
/// O(n) - n pocet uz pridanych aut, pretoze pred vlozenim auta musi skontrolovat unikatnost SPZ
void Firma::zaradDoVozovehoParku()
{
	bool pokracujCase = true;
	std::string strSPZ;
	std::string strNosnost;
	std::cout << "> Zadaj parametre vozidla:\n";

	do /// pyta si SPZ pokial nebude mat spravny format
	{
		std::cout << "> SPZ [prave 7 znakov]:\n";
		std::cin >> strSPZ;
		if (strSPZ.size() != 7)
		{
			std::cout << "SPZ vozidla nema prave 7 znakov: " << strSPZ << "\n";
		}
	} 
	while (strSPZ.size() != 7);

	for each (Vozidlo* aktVozidlo in *vozidlaFiremne_)
	{
		if (aktVozidlo->getSPZ() == strSPZ)  /// podmienka vrati 1 ak su SPZky rovnake
		{
			std::cout << "SPZ vozidla sa zhoduje s uz zaradenym vozidlom: " << strSPZ << "\n";
			std::cout << "AUTO SA NEZARADILO!\n";
			pokracujCase = false;
			break;
		}
	}

	if (pokracujCase)
	{
		std::cout << "> nosnost:\n";
		std::cin >> strNosnost;
		Vozidlo* noveVozidlo = new Vozidlo(strSPZ, stoi(strNosnost), *aktualnyDatum_);
		vozidlaFiremne_->add(noveVozidlo);
		sklad_->pridajVozidlo(*noveVozidlo);
		std::cout << "> Vozidlo bolo uspesne pridane";
	}
}

/// TLACIDLO 2
/// vypise zoznam vozidiel postupne ako boli zaradene, cize podla datumu zaradenia
/// O(n) - n pocet vozidiel vo vozovom parku
void Firma::vypisZoznamVozidiel()
{
	std::cout << "> Zoznam aut podla datumu zaradenia:\n";
	for each (Vozidlo* aktVozidlo in *vozidlaFiremne_)
	{
		std::cout << "SPZ: " << aktVozidlo->getSPZ() << "  |  ";
		std::cout << "Nosnost: " << aktVozidlo->getNosnost() << "  |  ";
		std::cout << "Opotrebenie: " << aktVozidlo->getOpotrebenie() << std::endl;
	}
}

/// TLACIDLO 3
/// prida do firmy noveho dodavatela
/// O(n) - n pocet dodavatelov
void Firma::pridajDodavatela()
{
	bool opakujPokus;
	int index; /// index v zozname, do ktoreho sa dodavatel ulozi
	std::string strNazov;
	std::string strSidlo;
	std::cout << "> Zadaj parametre dodavatela:\n";
	std::cin.ignore();

	do /// pyta si obchodny nazov, az kym nebude unikatny, ak je, do zoznamu ho zaradi v abecednom poradi
	{
		opakujPokus = false;
		index = 0;
		std::cout << "> Obchodny Nazov[Velke zaciatocne pismeno]:\n";
		//std::cin.ignore();
		std::getline(std::cin, strNazov);
		//std::cin.get(strNazov, 100);
		if (dodavatelia_->size() > 0)
		{
			for each (Dodavatel* aktDodavatel in *dodavatelia_)
			{
				if (strNazov == aktDodavatel->getObchodnyNazov())
				{
					std::cout << "Takyto dodavatel uz existuje! Skus znova.\n";
					opakujPokus = true;
					break;
				}
				else if (strNazov < aktDodavatel->getObchodnyNazov())
				{
					std::cout << "> Adresa Sidla:\n";
					std::getline(std::cin, strSidlo);
					Dodavatel* novyDodavatel = new Dodavatel(strNazov, strSidlo);
					dodavatelia_->insert(novyDodavatel, index);
					std::cout << "Dodavatel pridany\n";
					break;
				}
				index++;
			}
		}
		if (dodavatelia_->size() == 0 || dodavatelia_->size() == index)
		{
			std::cout << "> Adresa Sidla:\n";
			//std::cin >> strSidlo;
			std::getline(std::cin, strSidlo);
			Dodavatel* novyDodavatel = new Dodavatel(strNazov, strSidlo);
			dodavatelia_->add(novyDodavatel);
			std::cout << "Dodavatel pridany\n";
		}

	} while (opakujPokus);
}

/// TLACIDLO 4
/// vypise dodavatelov v abecednom poradi
/// O(n) - n pocet dodavatelov
void Firma::vypisDodavatelov()
{
	std::cout << "> Zoznam dodavatelov podla abecedy:\n";
	for each (Dodavatel* aktDod in *dodavatelia_)
	{
		std::cout << "Obchodny nazov: " << aktDod->getObchodnyNazov() << "  |  ";
		std::cout << "Adresa sidla: " << aktDod->getAdresaSidla() << std::endl;
	}
}

/// TLACIDLO 5
/// ohlasi informacie o zasielke od dodavatela
/// O(n+m), n pocet dodavatelov, m pocet paliet jednej zasielky
void Firma::ohlasZasielku()
{
	bool existujeDodavatel = false;
	std::cout << "> Ohlas prichod kamionu:\n";
	std::string strNazov;
	std::cout << "> Zadaj obchodny nazov dodavatela:\n";
	std::cin.ignore();
	std::getline(std::cin, strNazov);
	for each (Dodavatel* aktDodavatel in *dodavatelia_)
	{
		if (strNazov == aktDodavatel->getObchodnyNazov())
		{
			existujeDodavatel = true;
			std::string strDatum;
			std::cout << "> Zadaj datum prichodu [DD.MM.YYYY]:\n";
			std::cin >> strDatum;

			Datum* ohlasenyDatum = strToDatum(strDatum);
			if ((*ohlasenyDatum) < *aktualnyDatum_)
			{
				std::cout << "Ohlaseny datum je mensi ako aktualny - Zasielka sa neohlasila\n";
				delete ohlasenyDatum;
				break;
			}
			OhlasenaZasielka* ohlasenaZasielka = new OhlasenaZasielka(*ohlasenyDatum, *aktDodavatel);
			std::string pocetPaliet;
			std::cout << "> Zadaj pocet paliet:\n";
			std::cin >> pocetPaliet;
			for (int i = 0; i < stoi(pocetPaliet); i++)
			{
				int region;
				int hmotnost;
				int trieda;
				std::cout << std::to_string(i + 1) + ". paleta:\n";
				std::cout << "> region [cislo]:\n";
				std::cin >> region;
				std::cout << "> hmotnost:\n";
				std::cin >> hmotnost;
				std::cout << "> trieda [1/2]:\n";
				std::cin >> trieda;
				if (trieda == 1)
				{
					std::string strDatumDorucenia;
					std::cout << "> datum dorucenia[DD.MM.YYYY]:\n";
					std::cin >> strDatumDorucenia;
					Datum* datumDorucenia = strToDatum(strDatumDorucenia);
					Paleta* novaPaleta1 = new Paleta(region, hmotnost, *datumDorucenia, *ohlasenyDatum, 1, aktDodavatel->getObchodnyNazov());
					//ohlasenePaletyPrvejTriedy->add(novaPaleta1);
					ohlasenaZasielka->pridajPaletu(*novaPaleta1);
					delete datumDorucenia;
					std::cout << "Paleta pridana\n";
				}
				else /// nie je to paleta prvej triedy
				{
					Datum* lubovolnyDatumDorucenia = new Datum(1, 1, 1960);
					Paleta* novaPaleta2 = new Paleta(region, hmotnost, *lubovolnyDatumDorucenia, *ohlasenyDatum, 0, aktDodavatel->getObchodnyNazov());
					//ohlasenePaletyDruhejTriedy->add(novaPaleta2);
					ohlasenaZasielka->pridajPaletu(*novaPaleta2);
					delete lubovolnyDatumDorucenia;
					std::cout << "Paleta pridana\n";
				}
			}
			ohlaseneZasielky_->add(ohlasenaZasielka);
			std::cout << "ZASIELKA OHLASENA\n";
			delete ohlasenyDatum;
			break;
		}
	}
	if (!existujeDodavatel)
	{
		std::cout << "Takyto dodavatel neexistuje\n";
	}
}

Datum * Firma::strToDatum(std::string strDatum)
{
	int indexPrvejBodky = strDatum.find_first_of('.'); /// najde prvy vyskyt bodky
	std::string strDen = strDatum.substr(0, indexPrvejBodky); /// zoberie z datumu len den
	strDatum.erase(0, (indexPrvejBodky + 1)); /// oreze z datumu den
	int indexDruhejBodky = strDatum.find_first_of('.'); /// najde prvy vyskyt bodky v uz orezanom datume
	std::string strMesiac = strDatum.substr(0, indexDruhejBodky); /// zoberie z datumu len mesiac
	strDatum.erase(0, (indexDruhejBodky + 1)); /// oreze z datumu mesiac
	std::string strRok = strDatum; /// ostava uz len rok

	return new Datum(stoi(strDen), stoi(strMesiac), stoi(strRok));
}


/// TLACIDLO 6
/// zaeviduje zasielku dodavatela
/// zlozitost je v sklade
void Firma::zaevidujZasielku()
{
	if (flagPocetStlaceni6_ == 1) // ak uz bola sestka dnes stlacena
	{
		std::cout << "> Dnes uz nemas na zaevidovanie ziadne zasielky";
		return;
	}
	flagPocetStlaceni6_++;

	//int cisloZasielky = 1;
	bool existujeZasielkaNaDnes = false;
	//std::cout << "> Ocakavane zasielky na tento den [" << aktualnyDatum_->to_string() << "] od dodavatelov:\n";

	//ArrayList<OhlasenaZasielka*>* pomZas = new ArrayList<OhlasenaZasielka*>();
	for (int i = 0; i < ohlaseneZasielky_->size(); i++)
	{
		if (*(*ohlaseneZasielky_)[i]->getDatumPrichodu() == *aktualnyDatum_ )
		{
			existujeZasielkaNaDnes = true;
			//std::cout << std::to_string(cisloZasielky) << ". " << (aktOhlZas->getDodavatel())->getObchodnyNazov() << std::endl; \
			//OhlasenaZasielka& pomZasielka = *aktOhlZas;
			sklad_->zaevidujZasielku(*(*ohlaseneZasielky_)[i]);
			delete ohlaseneZasielky_->removeAt(i);
			i--;
			//cisloZasielky++;
		}
	}
	if (!existujeZasielkaNaDnes)
	{
		std::cout << "Na dnes nie su ohlasene ziadne zasielky";
		return;
	}
	sklad_->pripravRegionyDruheTriedy();
	std::cout << "Zasielky uspesne zaevidovane";

}

/// TLACIDLO 7
/// nalpni vozidla paletami podla pravidiel
void Firma::naplnVozidlaPaletami()
{
	if (flagPocetStlaceni7_ == 1) // ak uz bola sedmicka dnes stlacena
	{
		std::cout << "> Dnes uz si palety nalozil";
		return;
	}
	flagPocetStlaceni7_++;

	sklad_->naplnVozidlaPaletami();
}

/// TLACIDLO 8
/// odovzda palety zakaznikom
/// O(n*m), n pocet vozidiel, m max pocet paliet v jednom vozidle
void Firma::odovzdajPaletyZakaznikom()
{
	if (flagPocetStlaceni8_ == 1) // ak uz bola osmicka dnes stlacena
	{
		std::cout << "> Dnes uz si odovzdal vsetky zasielky zakaznikom";
		return;
	}
	flagPocetStlaceni8_++;

	srand(time(NULL));
	int nahodneCislo;
	LinkedList<Vozidlo*>* vozidlaNaCestePointer = sklad_->getVozidlaNaCestePointer();

	if (vozidlaNaCestePointer->isEmpty())
	{
		std::cout << "> Dnes si zatial nenalozil ziadne palety\n";
	}

	for each (Vozidlo* aktVozidlo in *vozidlaNaCestePointer)
	{
		int pocetPalietVoVozidle = aktVozidlo->getPaletyPointer()->size();
		int pocitadlo = 0;
		while (true) // nekonecna slucka kym neprejdem cez front size-krat
		{
			if (pocitadlo == pocetPalietVoVozidle)
			{
				break;
			}
			else
			{
				nahodneCislo = rand() % 100 + 1; /// vygeneruje nahodne cislo od 1 po 100
				Paleta* paleta = aktVozidlo->getPaletyPointer()->pop();
				if (nahodneCislo <= 5)
				{
					if (paleta->jePrvejTriedy())
					{
						
						std::cout << "> Zakaznik [region]: " << paleta->getRegion()
							<< " neprevzal paletu prvej triedy  |  hmotnost: " << paleta->getHmotnost()
							<< " dodavatel: " << paleta->getDodavatel() << std::endl;
					}
					else
					{
						std::cout << "> Zakaznik [region]: " << paleta->getRegion()
							<< " neprevzal paletu druhej triedy  |  hmotnost: " << paleta->getHmotnost()
							<< " dodavatel: " << paleta->getDodavatel() << std::endl;
					}
					paleta->setPrevzata(false);
					paleta->setDatumNeprevzatia(*aktualnyDatum_);
					aktVozidlo->getPaletyPointer()->push(paleta);
					pocitadlo++;
				}
				else
				{
					//paleta sa odovzda zakaznikovi - vymaze sa z vozidla
					aktVozidlo->znizMomentalnuHmotnost(paleta->getHmotnost());
					delete paleta;
					pocitadlo++;
				}
			}
		}
	}
		
	
	std::cout << "> Zakaznici prevzali palety\n";
}


/// TLACIDLO 9
void Firma::vratVozidlaDoSkladuAOdovzdajNeprevzatePaley()
{
	if (flagPocetStlaceni9_ == 1) // ak uz bola deviatka dnes stlacena
	{
		std::cout << "> Dnes uz vsetky vozidla prisli spat";
		return;
	}
	flagPocetStlaceni9_++;

	sklad_->vratVozidlaDoSkladuAOdovzdajNeprevzatePaley(dodavatelia_);
}

/// TLACIDLO 10
void Firma::vyradVozidla()
{
	sklad_->vyradVozidla(vozidlaFiremne_);
}

/// TLACIDLO 11
void Firma::vypisPaletyNaSklade()
{
	sklad_->vypisPaletyNaSklade();
}

/// TLACIDLO 12
/// O(2n), n pocet dodavatelov, pretoze najprv musim najst ten najvacsi pocet neprevzatych paliet vobec a potom vypisat tych dodavatelov co toto cislo maju
void Firma::vyhladajDodavatelaNajviacNeprevzatychPaliet()
{
	std::string datumOd;
	std::string datumDo;
	std::cin.ignore();
	std::cout << "> Zadaj v akom casovom obdobi chces zistit dodavatela s najviac neprevzatymi paletami:\n";
	std::cout << "  > Datum od [DD.MM.YYYY]:\n";
	std::cin >> datumOd;
	std::cout << "  > Datum do [DD.MM.YYYY]:\n";
	std::cin >> datumDo;
	Datum* datOd = strToDatum(datumOd);
	Datum* datDo = strToDatum(datumDo);

	if (*datOd > *datDo)
	{
		std::cout << "Datum od je vacsi ako datum do\n";
		delete datOd;
		delete datDo;
		return;
	}

	int maxPocetNeprevzatychPalietJednehoDodavatela = 0;
	int indexDodavatelov = 0;

	for each (Dodavatel* aktDod in *dodavatelia_)
	{
		if (aktDod->getPocetNeprevzatychPalietVDanomCasovomObdobi(*datOd, *datDo) > maxPocetNeprevzatychPalietJednehoDodavatela)
		{
			maxPocetNeprevzatychPalietJednehoDodavatela = aktDod->getPocetNeprevzatychPalietVDanomCasovomObdobi(*datOd, *datDo);
		}
	}

	if (maxPocetNeprevzatychPalietJednehoDodavatela == 0)
	{
		std::cout << "Ziadny dodavatel nema neprevzate palety";
		delete datOd;
		delete datDo;
		return;
	}

	for each (Dodavatel* aktDod in *dodavatelia_)
	{
		if (aktDod->getPocetNeprevzatychPalietVDanomCasovomObdobi(*datOd, *datDo) == maxPocetNeprevzatychPalietJednehoDodavatela)
		{
			std::cout << "Najviac neprevzatych paliet v tomto casovom obdobi ma: " << aktDod->getObchodnyNazov()
				<< " | pocet neprevzatych paliet: " << aktDod->getPocetNeprevzatychPalietVDanomCasovomObdobi(*datOd, *datDo) << std::endl;
		}
	}

	delete datOd;
	delete datDo;
}

/// TLACIDLO 13
void Firma::vypisNezrealizovanePalety()
{
	sklad_->vypisNezrealizovanePalety();
}


/// TLACIDLO 14
/// posunie aktualny den o 1
/// O(1)
void Firma::pridajDen()
{
	flagPocetStlaceni6_ = 0;
	flagPocetStlaceni7_ = 0;
	flagPocetStlaceni8_ = 0;
	flagPocetStlaceni9_ = 0;

	aktualnyDatum_->pridajDen();
	std::cout << "> Den bol pridany\n";
	vypisDnesnyDatum();
	sklad_->pripravRegionyNaDnesPrveTriedy(*aktualnyDatum_);
}

/// TLACIDLO 15
/// nacita data zo suboru
/// O(n) - nacitanie vozidiel, n pocet vozidiel
/// O(m) - nacitanie dodavatelov, m pocet dodavatelov
/// O(c*m*p) - nacitanie ohlasenych zasielok, c pocet zasielok, p pocet paliet 
void Firma::nacitajZoSuboru()
{
	if (momentalnyPocetPrikazov_ != pocitadloPrikazov_ - 1)
	{
		std::cout << "> Ak chces nacitat zo suboru, uloz si doteraz vykonane zmeny zapisom do suboru\n";
		return;
	}
	/// nacita zaevidovane vozidla
	std::string pocetVozidiel;
	std::string datumZaevidovania;
	std::string SPZ;
	std::string nosnost;
	std::string den;
	std::string mesiac;
	std::string rok;
	std::string opotrebovanie;
	std::ifstream suborVozidla("VozidlaZaevidovane" + priponaZapis_ + ".txt");
	getline(suborVozidla, pocetVozidiel);
	if (suborVozidla.is_open())
	{
		for (int i = 0; i < stoi(pocetVozidiel); i++)
		{
			getline(suborVozidla, datumZaevidovania);
			getline(suborVozidla, SPZ);
			getline(suborVozidla, nosnost);
			getline(suborVozidla, opotrebovanie);
			Datum* datum = strToDatum(datumZaevidovania);
			Vozidlo* vozidlo = new Vozidlo(SPZ, stoi(nosnost), *datum);
			vozidlo->pridajOpotrebenie(stoi(opotrebovanie));
			vozidlaFiremne_->add(vozidlo);
			//sklad_->pridajVozidlo(*vozidlo);
			delete datum;
		}
		std::cout << "Vozidla nacitane zo suboru\n";
		suborVozidla.close();
	}
	else
	{
		std::cout << "Subor VozidlaZaevidovane.txt sa nepodarilo otvorit\n";
	}

	/// nacita vozidla do skladu a zaroven aj do firemnych vozidiel
	sklad_->nacitajVozidlaDoSkladu(vozidlaFiremne_);

	/// nacita VOZIDLA NA CESTE aj z ich PALETAMI
	sklad_->nacitajVozidlaAPaletyNaCeste(vozidlaFiremne_);

	/// nacita VOZIDLA NA ODPIS
	sklad_->nacitajVozidlaNaOdpis(vozidlaFiremne_);

	/// nacita NEZREALIZOVANE palety
	sklad_->nacitajNezrealizovanePalety();

	/// nacita dodavatelov
	std::string pocetDodavatelov;
	std::string obchodnyNazov;
	std::string adresaSidla;
	std::ifstream suborDodavatelia("Dodavatelia" + priponaZapis_ + ".txt");
	getline(suborDodavatelia, pocetDodavatelov);
	if (suborDodavatelia.is_open())
	{
		for (int i = 0; i < stoi(pocetDodavatelov); i++)
		{
			getline(suborDodavatelia, obchodnyNazov);
			getline(suborDodavatelia, adresaSidla);
			Dodavatel* dodavatel = new Dodavatel(obchodnyNazov, adresaSidla);
			dodavatelia_->add(dodavatel);
		}
		std::cout << "Dodavatelia nacitani zo suboru\n";
		suborDodavatelia.close();
	}
	else
	{
		std::cout << "Subor Dodavatelia.txt sa nepodarilo otvorit\n";
	}


	/// nacita NEPREVZATE palety (uz musia byt nacitani dodavatelia)
	nacitajNeprezvatePalety();


	/// nacita ohlasene zasielky
	bool existujeDodavatel = false;
	std::string pocetZasielok;
	std::string pocetPalietPrvejTriedy;
	std::string pocetPalietDruhejTriedy;
	std::string strDodavatel;
	std::string strDatumPrichodu;
	std::string region;
	std::string hmotnost;
	std::string strDatumDorucenia;
	std::ifstream suborOhlaseneZasielky("OhlaseneZasielky" + priponaZapis_ + ".txt");
	if (suborOhlaseneZasielky.is_open())
	{
		getline(suborOhlaseneZasielky, pocetZasielok);
		for (int i = 0; i < stoi(pocetZasielok); i++)
		{
			existujeDodavatel = false;
			OhlasenaZasielka* ohlZas = new OhlasenaZasielka();
			getline(suborOhlaseneZasielky, strDodavatel);
			getline(suborOhlaseneZasielky, strDatumPrichodu);
			getline(suborOhlaseneZasielky, pocetPalietPrvejTriedy);
			for each (Dodavatel* aktDod in *dodavatelia_)
			{
 				if (strDodavatel == aktDod->getObchodnyNazov())
				{
					existujeDodavatel = true;
					ohlZas->setDodavatel(*aktDod);
					Datum* datumPrichodu = strToDatum(strDatumPrichodu);
					ohlZas->setDatumPrichodu(*datumPrichodu);
					for (int j = 0; j < stoi(pocetPalietPrvejTriedy); j++)
					{
						getline(suborOhlaseneZasielky, region);
						getline(suborOhlaseneZasielky, hmotnost);
						getline(suborOhlaseneZasielky, strDatumDorucenia);
						Datum* datumDorucenia = strToDatum(strDatumDorucenia);
						Paleta* paleta = new Paleta(stoi(region), stoi(hmotnost), *datumDorucenia, *datumPrichodu, 1, aktDod->getObchodnyNazov());
						ohlZas->pridajPaletu(*paleta);
						delete datumDorucenia;
					}
					getline(suborOhlaseneZasielky, pocetPalietDruhejTriedy);
					for (int l = 0; l < stoi(pocetPalietDruhejTriedy); l++)
					{
						getline(suborOhlaseneZasielky, region);
						getline(suborOhlaseneZasielky, hmotnost);
						Datum* lubovolnyDatumDorucenia = new Datum(1, 1, 1960);
						Paleta* paleta = new Paleta(stoi(region), stoi(hmotnost), *lubovolnyDatumDorucenia, *datumPrichodu, 0, aktDod->getObchodnyNazov());
						ohlZas->pridajPaletu(*paleta);
						delete lubovolnyDatumDorucenia;
					}
					delete datumPrichodu;
					ohlaseneZasielky_->add(ohlZas);
					break;
				}
			}
			if (!existujeDodavatel)
			{
				std::cout << "Problem pri nacitani ohlasenych zasielok, dodavatel " << strDodavatel << " neexistuje v zozname dodavatelov firmy\n";
			}

		}
		std::cout << "Ohlasene zasielky nacitane zo suboru\n";
		suborOhlaseneZasielky.close();
	}
	else 
	{
		std::cout << "Subor OhlaseneZasielky.txt sa nepodarilo otvorit";
	}

	/// nacita REGIONY
	sklad_->nacitajRegiony();

	/// nacita PALETY DO SKLADU
	sklad_->nacitajPaletyDoSkladu();

	/// nacita REGIONY PRVYCH TRIED na dnes
	sklad_->nacitajRegionyPrveTriedyNaDnes();

	/// nacita REGIONY DRUHYCH TRIED 
	sklad_->nacitajRegionyDruheTriedy();
}

void Firma::nacitajNeprezvatePalety()
{
	std::string dodavatel;
	std::string pocetNeprPaliet;
	std::string strDatNeprevzatia;
	Datum* datumPrichodu = new Datum(1, 1, 2010);
	Datum* datumDorucenia = new Datum(2, 1, 2010);
	std::ifstream suborNeprevzatePalety("NeprevzatePalety" + priponaZapis_ + ".txt");
	if (suborNeprevzatePalety.is_open())
	{
		while (getline(suborNeprevzatePalety, dodavatel))
		{
			getline(suborNeprevzatePalety, pocetNeprPaliet);
			for each (Dodavatel* aktDod in *dodavatelia_)
			{
				if (aktDod->getObchodnyNazov() == dodavatel)
				{
					for (int i = 0; i < stoi(pocetNeprPaliet); i++)
					{
						getline(suborNeprevzatePalety, strDatNeprevzatia);
						Datum* datNeprevzatia = strToDatum(strDatNeprevzatia);
						Paleta* novaPaleta = new Paleta(0, 10, *datumDorucenia, *datumPrichodu, 0, dodavatel);
						novaPaleta->setPrevzata(false);
						novaPaleta->setDatumNeprevzatia(*datNeprevzatia);
						aktDod->pridajNeprevzatuPaletu(*novaPaleta);
						delete datNeprevzatia;
					}
				}
			}
		}
		std::cout << "Neprevzate palety nacitane zo suboru\n";
		suborNeprevzatePalety.close();
	}
	else
	{
		std::cout << "Subor NeprevzatePalety.txt sa nepodarilo otvorit\n";
	}
	delete datumPrichodu;
	delete datumDorucenia;

}

/// TLACIDLO 16
/// zapise aktualny stav firmy do suboru

void Firma::zapisDoSuboru()
{
	momentalnyPocetPrikazov_ = pocitadloPrikazov_;
	/// zapise zaevidovane vozidla
	std::ofstream vozidlaZaevidovaneZapis;
	vozidlaZaevidovaneZapis.open("VozidlaZaevidovaneZapis.txt");
	vozidlaZaevidovaneZapis << vozidlaFiremne_->size() << std::endl;
	for each (Vozidlo* aktVozidlo in *vozidlaFiremne_)
	{
		vozidlaZaevidovaneZapis << aktVozidlo->getDatumZaevidovania()->to_string() << std::endl;
		vozidlaZaevidovaneZapis << aktVozidlo->getSPZ() << std::endl;
		vozidlaZaevidovaneZapis << aktVozidlo->getNosnost() << std::endl;
		vozidlaZaevidovaneZapis << aktVozidlo->getOpotrebenie() << std::endl;
	}
	std::cout << "Vozidla sa zapisali do suboru\n";
	vozidlaZaevidovaneZapis.close();

	/// zapise dodavatelov firmy
	std::ofstream dodavateliaZapis;
	dodavateliaZapis.open("DodavateliaZapis.txt");
	dodavateliaZapis << dodavatelia_->size() << std::endl;
	for each (Dodavatel* aktDod in *dodavatelia_)
	{
		dodavateliaZapis << aktDod->getObchodnyNazov() << std::endl;
		dodavateliaZapis << aktDod->getAdresaSidla() << std::endl;
	}
	std::cout << "Dodavatelia sa zapisali do suboru\n";
	dodavateliaZapis.close();

	/// zapise ohlasene zasielky
	std::ofstream ohlaseneZasielkyZapis;
	ohlaseneZasielkyZapis.open("OhlaseneZasielkyZapis.txt");
	ohlaseneZasielkyZapis << ohlaseneZasielky_->size() << std::endl;
	for each (OhlasenaZasielka* aktZas in *ohlaseneZasielky_)
	{
		ohlaseneZasielkyZapis << (aktZas->getDodavatel())->getObchodnyNazov() << std::endl;
		ohlaseneZasielkyZapis << (aktZas->getDatumPrichodu())->to_string() << std::endl;
		ohlaseneZasielkyZapis << aktZas->getSizePaletyPrvejTriedy() << std::endl;
		for (int i = 0; i < aktZas->getSizePaletyPrvejTriedy(); i++)
		{
			ohlaseneZasielkyZapis << (aktZas->getPaletaPrvejTriedy(i))->getRegion() << std::endl;
			ohlaseneZasielkyZapis << (aktZas->getPaletaPrvejTriedy(i))->getHmotnost() << std::endl;
			ohlaseneZasielkyZapis << ((aktZas->getPaletaPrvejTriedy(i))->getDatumDorucenia())->to_string() << std::endl;
		}
		ohlaseneZasielkyZapis << aktZas->getSizePaletyDruhejTriedy() << std::endl;
		for (int j = 0; j < aktZas->getSizePaletyDruhejTriedy(); j++)
		{
			ohlaseneZasielkyZapis << (aktZas->getPaletaDruhejTriedy(j))->getRegion() << std::endl;
			ohlaseneZasielkyZapis << (aktZas->getPaletaDruhejTriedy(j))->getHmotnost() << std::endl;
		}
	}
	std::cout << "Ohlasene zasielky sa zapisali do suboru\n";
	ohlaseneZasielkyZapis.close();

	/// zapise existujuce regiony
	sklad_->zapisRegionyDoSuboru();

	/// zapise ZAEVIDOVANE palety
	sklad_->zapisZaevidovanePaletyDoSuboru();

	/// zapise PALETY A VOZIDLA na ceste
	sklad_->zapisPaletyAVozidlaNaCeste(*aktualnyDatum_);

	/// zapise VOZIDLA V SKLADE
	sklad_->zapisVozidlaVSklade();

	/// zapise VOZIDLA NA ODPIS
	sklad_->zapisVozidlaNaOdpis();

	/// zapise NEZREALIZOVANE palety
	sklad_->zapisNezrealizovanePalety();

	/// zapise NEPREVZATE PALETY dodavatela
	std::ofstream neprevzatePaletyZapis;
	neprevzatePaletyZapis.open("NeprevzatePaletyZapis.txt");

	Datum* datumOd = new Datum(1, 1, 1960);
	Datum* datumDo = new Datum(1, 1, 2099);

	for each (Dodavatel* aktDod in *dodavatelia_)
	{
		
		int pocetNeprevziatychPalietDodavatela = aktDod->getPocetNeprevzatychPalietVDanomCasovomObdobi(*datumOd, *datumDo);
		if (pocetNeprevziatychPalietDodavatela > 0)
		{
			neprevzatePaletyZapis << aktDod->getObchodnyNazov() << std::endl;
			neprevzatePaletyZapis << pocetNeprevziatychPalietDodavatela << std::endl;
			LinkedList<Paleta*>* neprevziatePaletyPointer = aktDod->getNeprevziatePaletyPointer();
			for each (Paleta* aktPal in *neprevziatePaletyPointer)
			{
				neprevzatePaletyZapis << aktPal->getDatumNeprevzatia()->to_string() << std::endl;
			}
		}
	}
	neprevzatePaletyZapis.close();
	std::cout << "Neprevzate palety zapisane do suboru\n";

	delete datumOd;
	delete datumDo;
}
