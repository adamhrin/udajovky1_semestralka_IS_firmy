#include "Sklad.h"
#include <iostream>
#include <fstream>
#include "structures\heap_monitor.h"


Sklad::Sklad()
{

	/////////////////////////////////////
	priponaZapis_ = ""; //////"Zapis"
	/////////////////////////////////////

	vozidlaVSklade_ = new Heap<Vozidlo*>();
	vozidlaNaCestu_ = new LinkedList<Vozidlo*>();
	paletyNaSklade_ = new ArrayList<Paleta*>();
	regionyPodlaNajlahsichPalietPrvejTriedy_ = new Heap<Region*>();
	regionyPodlaNajlahsichPalietDruhejTriedy_ = new Heap<Region*>();
	regiony_ = new ArrayList<Region*>();
	nezrealizovanePalety_ = new LinkedList<Paleta*>();
	vozidlaNaOdpis_ = new ExplicitQueue<Vozidlo*>();
	MAXNosnost_ = 0;
}

/// O(c*n), n pocet prvkov v jednotlivych zoznamoch, c pocet zoznamov
Sklad::~Sklad()
{
	/// nezrealizovane palety uz nema region, uz su iba na sklade, preto si ich maze sklad
	for each (Paleta* aktPaleta in *nezrealizovanePalety_)
	{
		delete aktPaleta;
	}
	delete nezrealizovanePalety_;

	// realne vozidla deletujem vo firme (vozidlaFiremne)
	delete vozidlaNaCestu_;

	/// vozidla vymazavam vo firme
	delete vozidlaVSklade_;

	/// palety vymazava region
	delete paletyNaSklade_;

	/// ak tam nejake vozidla ostanu, vymaze ich pri destrukcii firma, ja vymazem len pointer na zoznam
	delete vozidlaNaOdpis_;

	for each (Region* region in *regiony_)
	{
		delete region;
	}
	delete regiony_;
	delete regionyPodlaNajlahsichPalietPrvejTriedy_;
	delete regionyPodlaNajlahsichPalietDruhejTriedy_;
	regionyPodlaNajlahsichPalietPrvejTriedy_ = nullptr;
	regionyPodlaNajlahsichPalietDruhejTriedy_ = nullptr;
	vozidlaVSklade_ = nullptr;
	vozidlaNaOdpis_ = nullptr;
	vozidlaNaCestu_ = nullptr;
	regiony_ = nullptr;
	paletyNaSklade_ = nullptr;
	nezrealizovanePalety_ = nullptr;
}


void Sklad::pridajVozidlo(Vozidlo & pridavaneVozidlo)
{
	if (pridavaneVozidlo.getNosnost() > MAXNosnost_)
	{
		MAXNosnost_ = pridavaneVozidlo.getNosnost();
	}
	vozidlaVSklade_->push(pridavaneVozidlo.getOpotrebenie(), &pridavaneVozidlo);
}


/// O(n*m), n pocet paliet, m pocet regionov
void Sklad::zaevidujZasielku(OhlasenaZasielka & zasielka)
{
	std::cout << "Evidujem zasielky ohlasene na dnes...\n";
	std::cout << "Dodavatel:   \n" << zasielka.getDodavatel()->getObchodnyNazov() << std::endl;
	std::cout << "Palety 1. triedy:\n";
	for (int i = 0; i < zasielka.getPocetPalietPrvejTriedy(); i++)
	{
		bool existujeRegion = false;
		Paleta* paletaCopy = new Paleta(*zasielka.getPaletaPrvejTriedy(i));
		std::cout << std::to_string(i + 1) << "."
				  << "   region: " << paletaCopy->getRegion()
				  << "   hmotnost: " << paletaCopy->getHmotnost()
				  << "   datum dorucenia : " << paletaCopy->getDatumDorucenia()->to_string() << std::endl;

		for each (Region* aktRegion in *regiony_)
		{
			if (zasielka.getPaletaPrvejTriedy(i)->getRegion() == aktRegion->getCislo()) /// ak existuje region
			{
				existujeRegion = true; 
				if (paletaCopy->getHmotnost() > MAXNosnost_)	/// ak ma paleta hmotnost vasciu max nosnosti vozidla firmy alebo - NEZAEVIDUJ
				{
					std::cout << "Paleta " << std::to_string(i + 1) << " prekracuje maximalnu nosnost vozidla [" << MAXNosnost_ << "] - NEZAEVIDOVANA\n";
					delete paletaCopy;
					break;
				}
				else if(*(paletaCopy->getDatumDorucenia()) < *(paletaCopy->getDatumPrichoduDoSkladu()) ||	/// ak je datum dorucenia palety mensi alebo rovny
						*(paletaCopy->getDatumDorucenia()) == *(paletaCopy->getDatumPrichoduDoSkladu()))	/// ako dnesny datum (datum prichodu do skladu) - NEZAEVIDUJ
				{		
					std::cout << "Paleta " << std::to_string(i + 1) << " ma datum dorucenia skor ako dnes - NEZAEVIDOVANA\n";
					delete paletaCopy;
					break;
				}
				else
				{
					/// vlozi paletu prvej triedy do regionu podla hmotnosti
					aktRegion->pridajPaletuPrvejTriedy(paletaCopy->getHmotnost(), *paletaCopy);
					/// prida ju aj do arrayListu paliet na sklade
					paletyNaSklade_->add(paletaCopy);
					break;
					//aktPriorFront->push(paletaCopy->getHmotnost(), paletaCopy);
				}
			}

		}
		if (!existujeRegion) /// neexistuje este taky front, vytvori ho a vlozi tam paletu prvej triedy
		{
			if (paletaCopy->getHmotnost() > MAXNosnost_)	/// ak ma paleta hmotnost vasciu max nosnosti vozidla firmy alebo - NEZAEVIDUJ
			{
				std::cout << "Paleta " << std::to_string(i + 1) << " prekracuje maximalnu nosnost vozidla [" << MAXNosnost_ << "] - NEZAEVIDOVANA\n";
				delete paletaCopy;
			}
			else if (*(paletaCopy->getDatumDorucenia()) < *(paletaCopy->getDatumPrichoduDoSkladu()) ||	/// ak je datum dorucenia palety mensi alebo rovny
				*(paletaCopy->getDatumDorucenia()) == *(paletaCopy->getDatumPrichoduDoSkladu()))	/// ako dnesny datum (datum prichodu do skladu) - NEZAEVIDUJ
			{
				std::cout << "Paleta " << std::to_string(i + 1) << " ma datum dorucenia skor ako dnes - NEZAEVIDOVANA\n";
				delete paletaCopy;
			}
			else
			{
				/// vlozi paletu prvej triedy do zoznamu zaevidovanych paliet prvej triedy
				/// na den dorucenia palety a podla hmotnosti
				Region* novyRegion = new Region(paletaCopy->getRegion());
				regiony_->add(novyRegion);
				
				novyRegion->pridajPaletuPrvejTriedy(paletaCopy->getHmotnost(), *paletaCopy);
				paletyNaSklade_->add(paletaCopy);
			}
		}
	}

	std::cout << "Palety 2. triedy:\n";
	for (int i = 0; i < zasielka.getPocetPalietDruhejTriedy(); i++)
	{
		Paleta* paletaCopy = new Paleta(*zasielka.getPaletaDruhejTriedy(i));
		std::cout << std::to_string(i + 1) << "."
				  << "   region: " << paletaCopy->getRegion()
				  << "   hmotnost: " << paletaCopy->getHmotnost() << std::endl;
		bool existujeRegion = false;
		for each (Region* aktRegion in *regiony_)
		{
			if (zasielka.getPaletaDruhejTriedy(i)->getRegion() == aktRegion->getCislo()) /// ak existuje prior front na den prichodu palety do skladu
			{
				existujeRegion = true;
				
				if (paletaCopy->getHmotnost() > MAXNosnost_)	/// ak ma paleta hmotnost vasciu max nosnosti vozidla firmy alebo - NEZAEVIDUJ
				{
					std::cout << "Paleta " << std::to_string(i + 1) << " prekracuje maximalnu nosnost vozidla [" << MAXNosnost_ << "]  - NEZAEVIDOVANA\n";
					delete paletaCopy;
					break;
				}
				else
				{
					/// vlozi paletu druhej triedy do zoznamu zaevidovanych paliet druhej triedy
					/// na den prichodu palety a podla hmotnosti
					////////////////////////////////////////////TODO
					aktRegion->pridajPaletuDruhejTriedy(*paletaCopy);
					paletyNaSklade_->add(paletaCopy);
					break;
				}
			}

		}
		if (!existujeRegion) /// neexistuje este taky front, vytvori ho a vlozi tam paletu druhej triedy
		{

			if (paletaCopy->getHmotnost() > MAXNosnost_)	/// ak ma paleta hmotnost vasciu max nosnosti vozidla firmy alebo - NEZAEVIDUJ
			{
				std::cout << "Paleta " << std::to_string(i + 1) << " prekracuje maximalnu nosnost vozidla [" << MAXNosnost_ << "] - NEZAEVIDOVANA\n";
				delete paletaCopy;
			}
			else
			{
				Region* novyRegion = new Region(paletaCopy->getRegion());
				regiony_->add(novyRegion);
				
				novyRegion->pridajPaletuDruhejTriedy(*paletaCopy);
				paletyNaSklade_->add(paletaCopy);

			}
		}
	}


	std::cout << "> Zasielka od dodavatela zaevidovana - palety vylozene v sklade\n";
	//delete &zasielka;
}

/// metoda vyvolana firmou, ked sa zaeviduju vsetky zasielky na dnes, vykona sa aktualizacia regionov v prioritnom fronte paliet druhej triedy
void Sklad::pripravRegionyDruheTriedy()
{
	////////////////////////TODO napln regionyDruhychTried = prior front regionov podla priority najlahsej a najskorsej palety 2 triedy v regione
	/// popnu sa vsetky regiony v regionochDruhejTriedy a aktualizovane sa tam daju naspat
	if (!regionyPodlaNajlahsichPalietDruhejTriedy_->isEmpty())
	{
		while (!regionyPodlaNajlahsichPalietDruhejTriedy_->isEmpty()) /// vyprazdni ten prior front
		{
			regionyPodlaNajlahsichPalietDruhejTriedy_->pop();
		}
	}
	for each (Region* aktRegion in *regiony_)
	{
		// ponakladaj do pf regionov podla najprioritnejsej palety druhej triedy
		if (aktRegion->getPocetPalietDruhejTriedySNajvacsouPrioritou() != 0)
		{
			regionyPodlaNajlahsichPalietDruhejTriedy_->push(aktRegion->peekPaletyDruhejTriedySNajvacsouPrioritou()->getPrioritaDruheTriedy(), aktRegion);
		}
	}
}

LinkedList<Vozidlo*>* Sklad::getVozidlaNaCestePointer()
{
	return vozidlaNaCestu_;
}


/// naplni vozidla paletami prvej triedy, ktore maju byt dorucene dnes
void Sklad::naplnVozidlaPaletami()
{
	std::cout << "Nakladam...\n";
	while (!regionyPodlaNajlahsichPalietPrvejTriedy_->isEmpty())
	{
		Region* vybranyRegion = regionyPodlaNajlahsichPalietPrvejTriedy_->pop(); // popne region, potom ho pripadne pushne naspat
		Paleta* vybranaPaleta = vybranyRegion->peekPaletyPrvejTriedyNaNajmensiDatum(); // peekne paletu, ked sa ju podari vlozit tak ju popne
		if (pokusSaNalozitPaletu(*vybranaPaleta))
		{
			vybranyRegion->popPaletuPrvejTriedyNaNajmensiDatum();
			/// ked paletu nalozim do auta, vymazem ju z paliet na sklade
			paletyNaSklade_->tryRemove(vybranaPaleta);
			if (vybranyRegion->getPocetPalietPrvejTriedyNaNajmensiDatum() != 0)
			{
				regionyPodlaNajlahsichPalietPrvejTriedy_->push(vybranyRegion->peekPaletyPrvejTriedyNaNajmensiDatum()->getHmotnost(), vybranyRegion);
			}
			else
			{
				vybranyRegion->vymazPriorFrontNaNajmensiDatumPrveTriedy();
			}
		}
		// ked uz nebudem schopny nalozit palety prvej triedy z dovodu nedostatocnej kapacity
		// tak vsetky ostatne palety prvej triedy zo vsetkych ostatnych regionov s datumom dorucenia dnes
		// presuniem do zoznamu nezrealizovanych paliet
		else
		{
			regionyPodlaNajlahsichPalietPrvejTriedy_->push(vybranyRegion->peekPaletyPrvejTriedyNaNajmensiDatum()->getHmotnost(), vybranyRegion);
			std::cout << "Ostatne palety prvej triedy nie je mozne nalozit - NEZREALIZOVANE:\n";
			while (!regionyPodlaNajlahsichPalietPrvejTriedy_->isEmpty())
			{
				int pocetPalietPrvejTriedyRegionuNaNajmensiDatum = regionyPodlaNajlahsichPalietPrvejTriedy_->peek()->getPocetPalietPrvejTriedyNaNajmensiDatum();
				for (int i = 0; i < pocetPalietPrvejTriedyRegionuNaNajmensiDatum; i++)
				{
					Paleta* paletaDoNezrealizovanych = regionyPodlaNajlahsichPalietPrvejTriedy_->peek()->popPaletuPrvejTriedyNaNajmensiDatum();
					std::cout << "Dodavatel: " << paletaDoNezrealizovanych->getDodavatel() << "   region: " << paletaDoNezrealizovanych->getRegion() << "   hmotnost: " << paletaDoNezrealizovanych->getHmotnost() << std::endl;
					/// vymaze sa aj z paliet na sklade
					paletyNaSklade_->tryRemove(paletaDoNezrealizovanych);
					nezrealizovanePalety_->add(paletaDoNezrealizovanych); // popne paletu v prvom prior fronte a da ju do nezrealizovanych
				}
				regionyPodlaNajlahsichPalietPrvejTriedy_->peek()->vymazPriorFrontNaNajmensiDatumPrveTriedy();
				regionyPodlaNajlahsichPalietPrvejTriedy_->pop(); // popne prvy region z prior frontu
			}
			break;
		}
	}
	std::cout << "Koniec nakladania paliet prvej triedy\n";
	std::cout << "Pokracujem paletami druhej triedy...\n";
	nalozPaletyDruhejTriedy();

}

/// metoda pripravi prioritny front regionov, v ktorych su palety prvej triedy na dorucenie DNES
/// priorita jednotlivych regionov je hmotnost najlahsej palety prvej triedy na dorucenie DNES
/// O(n), n pocet regionov
void Sklad::pripravRegionyNaDnesPrveTriedy(Datum & aktualnyDatum)
{
	if (!regionyPodlaNajlahsichPalietPrvejTriedy_->isEmpty()) // ak nie je prazdny, mam tam este palety ktore som mal rozviest vcera, cize stavaju sa nezrealizovanymi
	{
		std::cout << "Na sklade mas palety 1. triedy, ktore mali byt dorucene VCERA - vraciam dodavatelovi ako nezrealizovane!\n";
		while (!regionyPodlaNajlahsichPalietPrvejTriedy_->isEmpty())
		{
			int pocetPalietPrvejTriedyRegionuNaNajmensiDatum = regionyPodlaNajlahsichPalietPrvejTriedy_->peek()->getPocetPalietPrvejTriedyNaNajmensiDatum();
			for (int i = 0; i < pocetPalietPrvejTriedyRegionuNaNajmensiDatum; i++)
			{
				Paleta* paletaDoNezrealizovanych = regionyPodlaNajlahsichPalietPrvejTriedy_->peek()->popPaletuPrvejTriedyNaNajmensiDatum();
				std::cout << "Dodavatel: " << paletaDoNezrealizovanych->getDodavatel() << "   region: " << paletaDoNezrealizovanych->getRegion() << "   hmotnost: " << paletaDoNezrealizovanych->getHmotnost() << std::endl;
				nezrealizovanePalety_->add(paletaDoNezrealizovanych); // popne paletu v prvom prior fronte a da ju do nezrealizovanych
			}
			regionyPodlaNajlahsichPalietPrvejTriedy_->peek()->vymazPriorFrontNaNajmensiDatumPrveTriedy();
			regionyPodlaNajlahsichPalietPrvejTriedy_->pop(); // popne prvy region z prior frontu
		}
	}
	for each (Region* aktRegion in *regiony_)
	{
		if (aktRegion->peekPaletyPrvejTriedyNaNajmensiDatum() != nullptr) // moze sa stat, ze ked v tomto momente prechadzam regiony, v niektorom z nich su iba palety druhej triedy, a teda by mi to robilo problemy
		{
			if (*(aktRegion->peekPaletyPrvejTriedyNaNajmensiDatum()->getDatumDorucenia()) == aktualnyDatum)
			{
				regionyPodlaNajlahsichPalietPrvejTriedy_->push(aktRegion->peekPaletyPrvejTriedyNaNajmensiDatum()->getHmotnost(), aktRegion);
			}
		}
	}

}

int Sklad::getPocetRegionov()
{
	return regiony_->size();
}

/// pokusi sa nalozit paletu do vozidla
/// return true, ak bude vozidlo k dispozicii
bool Sklad::pokusSaNalozitPaletu(Paleta & vybranaPaleta)
{
	if (!vozidlaNaCestu_->isEmpty()) // zoznam vozidlaNaCestu NIE JE prazdny
	{
		for each (Vozidlo* aktVozidlo in *vozidlaNaCestu_)
		{
			if (vybranaPaleta.getRegion() == aktVozidlo->getRegion()) // naslo sa vozidlo, ktore ide do toho isteho regionu ako paleta
			{
				if ((aktVozidlo->getMomentalnaHmotnost() + vybranaPaleta.getHmotnost()) <= aktVozidlo->getNosnost())
				{
					aktVozidlo->pridajDalsiuPaletu(vybranaPaleta);
					std::cout << "Vozidlo: " << aktVozidlo->getSPZ() << "  |  ";
					std::cout << "Paleta:  dodavatel: "<< vybranaPaleta.getDodavatel() << " region: " << vybranaPaleta.getRegion() << " hmotnost: " << vybranaPaleta.getHmotnost() << std::endl;
					return true;
				}
			}
		}
	}					
	// zoznam vozidlaNaCestu JE prazdny alebo paletu som nedal ani do jedneho vozidla (bud lebo sa nezmestila alebo este do takeho regionu vozidlo nemam
	if (!vozidlaVSklade_->isEmpty()) // ak mam este nejake vozidlo na sklade
	{
		Vozidlo* vybraneVozidloZoSkladu = vozidlaVSklade_->pop(); // vyberie z vozidiel na sklade to z najmensim opotrebenim
		vybraneVozidloZoSkladu->pridajPrvuPaletu(vybranaPaleta); // prida mu paletu
		vozidlaNaCestu_->add(vybraneVozidloZoSkladu); // da ho do zoznamu vozidlaNaCeste
		std::cout << "Vozidlo: " << vybraneVozidloZoSkladu->getSPZ() << "  |  ";
		std::cout << "Paleta:  dodavatel: " << vybranaPaleta.getDodavatel() << " region: " << vybranaPaleta.getRegion() << " hmotnost: " << vybranaPaleta.getHmotnost() << std::endl;
		return true;
	}
	return false;
	
}

// ak je miesto, nalozi palety druhej triedy podla hmotnosti aj podla najskorsieho datumu prichodu
void Sklad::nalozPaletyDruhejTriedy()
{
	while (!regionyPodlaNajlahsichPalietDruhejTriedy_->isEmpty())
	{
		Region* vybranyRegion = regionyPodlaNajlahsichPalietDruhejTriedy_->pop(); // popne region, potom ho pripadne pushne naspat
		Paleta* vybranaPaleta = vybranyRegion->popPaletyDruhejTriedySNajvacsouPrioritou(); // popne paletu, tu uz urcite naspat vracat nebude
		if (pokusSaNalozitPaletu(*vybranaPaleta))
		{
			/// ak sa paletu podari nalozit, vymazem ju z paliet na sklade
			paletyNaSklade_->tryRemove(vybranaPaleta);
			if (vybranyRegion->getPocetPalietDruhejTriedySNajvacsouPrioritou() != 0)
			{

				regionyPodlaNajlahsichPalietDruhejTriedy_->push(vybranyRegion->peekPaletyDruhejTriedySNajvacsouPrioritou()->getPrioritaDruheTriedy(), vybranyRegion);
			}
			else
			{

			}
		}
		// paletu sa uz nepodarilo nalozit, tak sa vrati naspat do regionu
		else
		{
			vybranyRegion->pridajPaletuDruhejTriedy(*vybranaPaleta);
			regionyPodlaNajlahsichPalietDruhejTriedy_->push(vybranyRegion->peekPaletyDruhejTriedySNajvacsouPrioritou()->getPrioritaDruheTriedy(), vybranyRegion);
			break;
		}
	}
	std::cout << "Dokoncilo sa nakladanie paliet druhej triedy\n";
}

//// zlozitost sa zda vysoka, ale vozidiel ktore budu mat neprevzate palety bude malo
//// aj neprevzatych paliet v ramci vozidla bude malo (len 5 percent zo vsetkych)
void Sklad::vratVozidlaDoSkladuAOdovzdajNeprevzatePaley(ArrayList<Dodavatel*>* dodavateliaPointer)
{
	if (vozidlaNaCestu_->isEmpty())
	{
		std::cout << "Ziadne vozidla nie su na ceste";
		return;
	}

	while (!vozidlaNaCestu_->isEmpty())
	{
		Vozidlo* vyberaneVozidlo = vozidlaNaCestu_->removeAt(0);
		while (!vyberaneVozidlo->getPaletyPointer()->isEmpty())
		{
			Paleta* aktPaleta = vyberaneVozidlo->getPaletyPointer()->pop();
			for each (Dodavatel* aktDod in *dodavateliaPointer)
			{
				if (aktPaleta->getDodavatel() == aktDod->getObchodnyNazov())
				{
					std::cout << "Neprevzata paleta region: " << aktPaleta->getRegion() << " | hmotnost : " << aktPaleta->getHmotnost() << " odovzdana dodavatelovi "
						<< aktPaleta->getDodavatel() << std::endl;
					aktDod->pridajNeprevzatuPaletu(*aktPaleta);
					break;
				}
			}
		}
		
		vyberaneVozidlo->pridajOpotrebenie(vyberaneVozidlo->getRegion());
		if (vyberaneVozidlo->getOpotrebenie() > 90) /// skotroluje sa opotrebenie ci je vacsie ako 90, ak ano da vozidlo do fronty na odpis
		{
			std::cout << "> Na odpis: ";
			std::cout << "> SPZ: " << vyberaneVozidlo->getSPZ() << " | opotrebovanie: " << vyberaneVozidlo->getOpotrebenie() << std::endl;
			vozidlaNaOdpis_->push(vyberaneVozidlo);
		}
		else
		{
			std::cout << "> Naspat do vozidiel na sklade: ";
			std::cout << "> SPZ: " << vyberaneVozidlo->getSPZ() << " | opotrebovanie: " << vyberaneVozidlo->getOpotrebenie() << std::endl;
			vozidlaVSklade_->push(vyberaneVozidlo->getOpotrebenie(), vyberaneVozidlo);
		}
	}
}

/// vyradi vozidla z frontu na odpis
/// O(n*m) n pocet vozidiel vo fronte na odpis, m pocet vozidiel vo firme, pretoze kazde vozidlo odstranovane z frontu musime vymazat aj z firmoveho
void Sklad::vyradVozidla(ArrayList<Vozidlo*>* vozidlaFiremnePointer)
{
	if (vozidlaNaOdpis_->isEmpty())
	{
		std::cout << "Vo fronte na odpis nemas ziadne vozidlo";
		return;
	}

	while (!vozidlaNaOdpis_->isEmpty())
	{
		Vozidlo* vyradovaneVozidlo = vozidlaNaOdpis_->pop();
		for (int i = 0; i < vozidlaFiremnePointer->size(); i++)
		{
			if (vyradovaneVozidlo->getSPZ() == (*vozidlaFiremnePointer)[i]->getSPZ())
			{
				std::cout << "> Vozidlo: " << " SPZ: " << (*vozidlaFiremnePointer)[i]->getSPZ() << " vyradene\n";
				delete vozidlaFiremnePointer->removeAt(i);
				break;
			}
		}
	}
}

void Sklad::vypisPaletyNaSklade()
{
	if (paletyNaSklade_->isEmpty())
	{
		std::cout << "> Na sklade nemas ziadnu paletu\n";
		return;
	}

	std::cout << "Palety aktualne na sklade:\n";
	for each (Paleta* aktPaleta in *paletyNaSklade_)
	{
		if (aktPaleta->jePrvejTriedy())
		{
			std::cout << "Datum prichodu: " << aktPaleta->getDatumPrichoduDoSkladu()->to_string()
				<< " | datum dorucenia: " << aktPaleta->getDatumDorucenia()->to_string()
				<< " | hmotnost: " << aktPaleta->getHmotnost()
				<< " | dodavatel: " << aktPaleta->getDodavatel()
				<< " | region: " << aktPaleta->getRegion() << std::endl;
		}
		else
		{
			std::cout << "Datum prichodu: " << aktPaleta->getDatumPrichoduDoSkladu()->to_string()
				<< " | hmotnost: " << aktPaleta->getHmotnost()
				<< " | dodavatel: " << aktPaleta->getDodavatel()
				<< " | region: " << aktPaleta->getRegion() << std::endl;
		}
		
	}
}


void Sklad::vypisNezrealizovanePalety()
{
	std::cout << "> Nezrealizovane palety:\n";
	for each (Paleta* aktPaleta in *nezrealizovanePalety_)
	{
		std::cout << "Datum prichodu: " << aktPaleta->getDatumPrichoduDoSkladu()->to_string()
			<< " | pozadovany datum dorucenia: " << aktPaleta->getDatumDorucenia()->to_string()
			<< " | hmotnost: " << aktPaleta->getHmotnost()
			<< " | dodavatel: " << aktPaleta->getDodavatel() << std::endl;
	}

}

void Sklad::zapisRegionyDoSuboru()
{
	std::ofstream regionyZapis;
	regionyZapis.open("RegionyZapis.txt");
	regionyZapis << regiony_->size() << std::endl;

	/// zapise celkovo vsetky regiony
	for each (Region* aktRegion in *regiony_)
	{
		regionyZapis << aktRegion->getCislo() << std::endl;
	}
	regionyZapis.close();
	std::cout << "Regiony zapisane do suboru\n";

	/// zapise regiony, v ktorych su palety PRVEJ TRIEDY s datumom dorucenia dnes
	std::ofstream regionyPrveTriedyDnesZapis;
	regionyPrveTriedyDnesZapis.open("RegionyPrveTriedyDnesZapis.txt");
	regionyPrveTriedyDnesZapis << regionyPodlaNajlahsichPalietPrvejTriedy_->size() << std::endl;

	ArrayList<Region*>* regionyPrveTriedyTemp = new ArrayList<Region*>();
	while (!regionyPodlaNajlahsichPalietPrvejTriedy_->isEmpty())
	{
		///poprehadzuje si vsetko z prioritaku do zoznamu
		Region* regionTemp = regionyPodlaNajlahsichPalietPrvejTriedy_->pop();
		regionyPrveTriedyTemp->add(regionTemp);
		regionyPrveTriedyDnesZapis << regionTemp->getCislo() << std::endl;
	}

	while (!regionyPrveTriedyTemp->isEmpty())
	{
		Region* aktReg = regionyPrveTriedyTemp->removeAt(0); // vymaze vzdy prvy prvok v zozname, da ho spat do prioritaku
		regionyPodlaNajlahsichPalietPrvejTriedy_->push(aktReg->peekPaletyPrvejTriedyNaNajmensiDatum()->getHmotnost(), aktReg);
	}
	delete regionyPrveTriedyTemp;
	regionyPrveTriedyDnesZapis.close();
	std::cout << "Regiony prvej triedy na dnes zapisane do suboru\n";



	/// zapise regiony, v ktorych su palety DRUHEJ TRIEDY s najvacsou prioritou (datum+hmotnost)
	std::ofstream regionyNajlahsieDruheTriedyZapis;
	regionyNajlahsieDruheTriedyZapis.open("RegionyNajlahsieDruheTriedyZapis.txt");
	regionyNajlahsieDruheTriedyZapis << regionyPodlaNajlahsichPalietDruhejTriedy_->size() << std::endl;

	ArrayList<Region*>* regionyDruheTriedyTemp = new ArrayList<Region*>();
	while (!regionyPodlaNajlahsichPalietDruhejTriedy_->isEmpty())
	{
		///poprehadzuje si vsetko z prioritaku do zoznamu
		Region* regionTemp = regionyPodlaNajlahsichPalietDruhejTriedy_->pop();
		regionyDruheTriedyTemp->add(regionTemp);
		regionyNajlahsieDruheTriedyZapis << regionTemp->getCislo() << std::endl;
	}

	while (!regionyDruheTriedyTemp->isEmpty())
	{
		Region* aktReg = regionyDruheTriedyTemp->removeAt(0); // vymaze vzdy prvy prvok v zozname, da ho spat do prioritaku
		regionyPodlaNajlahsichPalietDruhejTriedy_->push(aktReg->peekPaletyDruhejTriedySNajvacsouPrioritou()->getPrioritaDruheTriedy(), aktReg);
	}
	delete regionyDruheTriedyTemp;
	regionyNajlahsieDruheTriedyZapis.close();
	std::cout << "Regiony druhej triedy podla priority zapisane do suboru\n";
}

void Sklad::zapisZaevidovanePaletyDoSuboru()
{
	std::ofstream zaevidovanePaletyZapis;
	zaevidovanePaletyZapis.open("ZaevidovanePaletyZapis.txt");
	zaevidovanePaletyZapis << paletyNaSklade_->size() << std::endl;
	for each (Paleta* aktPal in *paletyNaSklade_)
	{
		zaevidovanePaletyZapis << aktPal->jePrvejTriedy() << std::endl;
		zaevidovanePaletyZapis << aktPal->getDodavatel() << std::endl;
		zaevidovanePaletyZapis << aktPal->getRegion() << std::endl;
		zaevidovanePaletyZapis << aktPal->getHmotnost() << std::endl;
		zaevidovanePaletyZapis << aktPal->getDatumPrichoduDoSkladu()->to_string() << std::endl;
		if (aktPal->jePrvejTriedy()) /// iba ak je to paleta prvej triedy pozeraj datum dorucenia
		{
			zaevidovanePaletyZapis << aktPal->getDatumDorucenia()->to_string() << std::endl;
		}
	}
	zaevidovanePaletyZapis.close();
	std::cout << "Palety na sklade zapisane do suboru\n";
}

void Sklad::zapisPaletyAVozidlaNaCeste(Datum& dnesnyDatum)
{
	std::ofstream paletyNaCesteZapis;
	paletyNaCesteZapis.open("PaletyNaCesteZapis.txt");
	paletyNaCesteZapis << vozidlaNaCestu_->size() << std::endl;
	for each (Vozidlo* aktVoz in *vozidlaNaCestu_)
	{
		paletyNaCesteZapis << aktVoz->getSPZ() << std::endl;
		paletyNaCesteZapis << aktVoz->getRegion() << std::endl;
		paletyNaCesteZapis << dnesnyDatum.to_string() << std::endl; // datum dorucenia paliet prvej triedy v tom vozidle
		paletyNaCesteZapis << aktVoz->getPaletyPointer()->size() << std::endl;

		ArrayList<Paleta*>* tempPalety = new ArrayList<Paleta*>();

		while (!aktVoz->getPaletyPointer()->isEmpty())
		{
			Paleta* paletaTemp = aktVoz->getPaletyPointer()->pop();
			tempPalety->add(paletaTemp);
			paletyNaCesteZapis << paletaTemp->jePrvejTriedy() << std::endl;
			paletyNaCesteZapis << paletaTemp->getDatumPrichoduDoSkladu()->to_string() << std::endl;
			paletyNaCesteZapis << paletaTemp->getHmotnost() << std::endl; 
			paletyNaCesteZapis << paletaTemp->getDodavatel() << std::endl;
		}

		while (!tempPalety->isEmpty())
		{
			Paleta* paletaSpat = tempPalety->removeAt(0);
			aktVoz->getPaletyPointer()->push(paletaSpat);
		}

		delete tempPalety;
		
	}
	paletyNaCesteZapis.close();
	std::cout << "Palety a vozidla na ceste zapisane do suboru\n";
}

void Sklad::zapisVozidlaVSklade()
{
	std::ofstream vozidlaVSkladeZapis;
	vozidlaVSkladeZapis.open("VozidlaVSkladeZapis.txt");
	vozidlaVSkladeZapis << vozidlaVSklade_->size() << std::endl;
	ArrayList<Vozidlo*>* vozidlaTemp = new ArrayList<Vozidlo*>();
	while (!vozidlaVSklade_->isEmpty())
	{
		Vozidlo* tempVoz = vozidlaVSklade_->pop();
		vozidlaTemp->add(tempVoz);
		vozidlaVSkladeZapis << tempVoz->getSPZ() << std::endl;
	}

	while (!vozidlaTemp->isEmpty())
	{
		Vozidlo* vozidloSpat = vozidlaTemp->removeAt(0);
		vozidlaVSklade_->push(vozidloSpat->getOpotrebenie(), vozidloSpat);
	}

	delete vozidlaTemp;
	vozidlaVSkladeZapis.close();
	std::cout << "Vozidla v sklade zapisane do suboru\n";
}

void Sklad::zapisVozidlaNaOdpis()
{
	std::ofstream vozidlaNaOdpisZapis;
	vozidlaNaOdpisZapis.open("VozidlaNaOdpisZapis.txt");
	vozidlaNaOdpisZapis << vozidlaNaOdpis_->size() << std::endl;

	ArrayList<Vozidlo*>* odpisoveTemp = new ArrayList<Vozidlo*>();
	while (!vozidlaNaOdpis_->isEmpty())
	{
		Vozidlo* vozidloTemp = vozidlaNaOdpis_->pop();
		odpisoveTemp->add(vozidloTemp);
		vozidlaNaOdpisZapis << vozidloTemp->getSPZ() << std::endl;
	}

	while (!odpisoveTemp->isEmpty())
	{
		Vozidlo* vozSpat = odpisoveTemp->removeAt(0);
		vozidlaNaOdpis_->push(vozSpat);
	}

	vozidlaNaOdpisZapis.close();
	delete odpisoveTemp;
	std::cout << "Vozidla na odpis zapisane v subore\n";
}

void Sklad::zapisNezrealizovanePalety()
{
	std::ofstream nezrealizovanePaletyZapis;
	nezrealizovanePaletyZapis.open("NezrealizovanePaletyZapis.txt");
	nezrealizovanePaletyZapis << nezrealizovanePalety_->size() << std::endl;

	for each (Paleta* aktPal in *nezrealizovanePalety_)
	{
		nezrealizovanePaletyZapis << aktPal->getDodavatel() << std::endl;
		nezrealizovanePaletyZapis << aktPal->getHmotnost() << std::endl;
		nezrealizovanePaletyZapis << aktPal->getDatumPrichoduDoSkladu()->to_string() << std::endl;
		nezrealizovanePaletyZapis << aktPal->getDatumDorucenia()->to_string() << std::endl;
	}

	nezrealizovanePaletyZapis.close();
	std::cout << "Nezrealizovane palety zapisane do suboru\n";
}

void Sklad::nacitajVozidlaDoSkladu(ArrayList<Vozidlo*>* vozidlaFiremnePointer)
{
	std::string pocetVozidiel;
	std::string SPZ;
	std::ifstream suborVozidla("VozidlaVSklade" + priponaZapis_ + ".txt");
	if (suborVozidla.is_open())
	{
		getline(suborVozidla, pocetVozidiel);
		for (int i = 0; i < stoi(pocetVozidiel); i++)
		{
			getline(suborVozidla, SPZ);
			for each (Vozidlo* aktVoz in *vozidlaFiremnePointer)
			{
				if (aktVoz->getSPZ() == SPZ)
				{
					pridajVozidlo(*aktVoz); // do skladu
					break;
				}
			}
		}
		std::cout << "Vozidla do skladu nacitane zo suboru\n";
		suborVozidla.close();
	}
	else
	{
		std::cout << "Subor VozidlaDoSkladuZapis.txt sa nepodarilo otvorit\n";
	}
}

void Sklad::nacitajVozidlaAPaletyNaCeste(ArrayList<Vozidlo*>* vozidlaFiremnePointer)
{
	/// nacita VOZIDLA NA CESTE aj s ich PALETAMI
	std::string pocetVozidielNaCeste;
	std::string SPZ;
	std::string region;
	std::string strDatumDorucenia;
	std::string pocetPaliet;
	std::string jePrvejTriedy;
	std::string strDatPrichodu;
	std::string hmotnost;
	std::string dodavatel;

	std::ifstream suborVozidlaAPaletyNaCeste("PaletyNaCeste" + priponaZapis_ + ".txt");
	if (suborVozidlaAPaletyNaCeste.is_open())
	{
		getline(suborVozidlaAPaletyNaCeste, pocetVozidielNaCeste);

		for (int i = 0; i < stoi(pocetVozidielNaCeste); i++)
		{
			getline(suborVozidlaAPaletyNaCeste, SPZ);
			getline(suborVozidlaAPaletyNaCeste, region);
			getline(suborVozidlaAPaletyNaCeste, strDatumDorucenia);
			getline(suborVozidlaAPaletyNaCeste, pocetPaliet);
			Datum* datDoruc = strToDatum(strDatumDorucenia);

			for each (Vozidlo* aktVoz in *vozidlaFiremnePointer)
			{
				if (aktVoz->getSPZ() == SPZ)
				{
					aktVoz->setRegion(stoi(region));
					for (int i = 0; i < stoi(pocetPaliet); i++)
					{
						getline(suborVozidlaAPaletyNaCeste, jePrvejTriedy);
						getline(suborVozidlaAPaletyNaCeste, strDatPrichodu);
						getline(suborVozidlaAPaletyNaCeste, hmotnost);
						getline(suborVozidlaAPaletyNaCeste, dodavatel);

						Datum* datPrich = strToDatum(strDatPrichodu);
						if (stoi(jePrvejTriedy) == 1)
						{
							Paleta* novaPaleta = new Paleta(stoi(region), stoi(hmotnost), *datDoruc, *datPrich, 1, dodavatel);
							aktVoz->pridajDalsiuPaletu(*novaPaleta); // nesetne vozidlu region
						}
						else
						{
							Paleta* novaPaleta = new Paleta(stoi(region), stoi(hmotnost), *datDoruc, *datPrich, 0, dodavatel);
							aktVoz->pridajDalsiuPaletu(*novaPaleta); // nesetne vozidlu region
						}
						delete datPrich;
					}
					vozidlaNaCestu_->add(aktVoz);
					break;
				}
			}
			
			delete datDoruc;
				
		}
		std::cout << "Vozidla na ceste aj so svojimi paletami nacitane zo suboru\n";
		suborVozidlaAPaletyNaCeste.close();
	}
	else
	{
		std::cout << "Subor PaletyNaCesteZapis.txt sa nepodarilo otvorit\n";
	}

}

void Sklad::nacitajVozidlaNaOdpis(ArrayList<Vozidlo*>* vozidlaFiremnePointer)
{
	/// nacita VOZIDLA NA ODPIS
	std::string pocetVoz;
	std::string SPZ;

	std::ifstream suborVozidlaNaOdpis("VozidlaNaOdpis" + priponaZapis_ + ".txt");
	if (suborVozidlaNaOdpis.is_open())
	{
		getline(suborVozidlaNaOdpis, pocetVoz);
		for (int i = 0; i < stoi(pocetVoz); i++)
		{
			getline(suborVozidlaNaOdpis, SPZ);
			for each (Vozidlo* aktVoz in *vozidlaFiremnePointer)
			{
				if (aktVoz->getSPZ() == SPZ)
				{
					vozidlaNaOdpis_->push(aktVoz); // do frontu na odpis
					break;
				}
			}
		}
		std::cout << "Vozidla na odpis nacitane zo suboru\n";
		suborVozidlaNaOdpis.close();
	}
	else
	{
		std::cout << "Subor VozidlaNaOdpis.txt sa nepodarilo otvorit\n";
	}

}

void Sklad::nacitajNezrealizovanePalety()
{
	/// nacita NEZREALIZOVANE palety
	std::string pocetNezPaliet;
	std::string dodavatel;
	std::string hmotnost;
	std::string strDatPrich;
	std::string strDatDoruc;

	std::ifstream suborNezrealizovanePalety("NezrealizovanePalety" + priponaZapis_ + ".txt");
	if (suborNezrealizovanePalety.is_open())
	{
		getline(suborNezrealizovanePalety, pocetNezPaliet);
		for (int i = 0; i < stoi(pocetNezPaliet); i++)
		{
			getline(suborNezrealizovanePalety, dodavatel);
			getline(suborNezrealizovanePalety, hmotnost);
			getline(suborNezrealizovanePalety, strDatPrich);
			getline(suborNezrealizovanePalety, strDatDoruc);
			Datum* datDoruc = strToDatum(strDatDoruc);
			Datum* datPrich = strToDatum(strDatPrich);
			Paleta* novePaleta = new Paleta(0, stoi(hmotnost), *datDoruc, *datPrich, 0, dodavatel);
			nezrealizovanePalety_->add(novePaleta);
			delete datDoruc;
			delete datPrich;
		}
		suborNezrealizovanePalety.close();
		std::cout << "Nezrealizovane palety nacitane zo suboru\n";
	}
	else
	{
		std::cout << "Subor NezrealizovanePalety.txt sa nepodarilo otvorit\n";
	}

}

void Sklad::nacitajRegiony()
{
	std::string pocetRegionov;
	std::string cislo;
	std::ifstream suborRegiony("Regiony" + priponaZapis_ + ".txt");
	if (suborRegiony.is_open())
	{
		getline(suborRegiony, pocetRegionov);
		for (int i = 0; i < stoi(pocetRegionov); i++)
		{
			getline(suborRegiony, cislo);
			Region* novyRegion = new Region(stoi(cislo));
			regiony_->add(novyRegion);
		}
		std::cout << "Regiony nacitane zo suboru\n";
		suborRegiony.close();
	}
	else
	{
		std::cout << "Subor Regiony.txt sa nepodarilo otvorit\n";
	}

}

void Sklad::nacitajPaletyDoSkladu()
{
	std::string pocetPaliet;
	std::string jePrvaTrieda;
	std::string dodavatel;
	std::string region;
	std::string hmotnost;
	std::string strDatPrich;
	std::string strDatDoruc;

	std::ifstream suborPaletyNaSklade("ZaevidovanePalety" + priponaZapis_ + ".txt");
	if (suborPaletyNaSklade.is_open())
	{
		getline(suborPaletyNaSklade, pocetPaliet);
		for (int i = 0; i < stoi(pocetPaliet); i++)
		{
			getline(suborPaletyNaSklade, jePrvaTrieda);
			getline(suborPaletyNaSklade, dodavatel);
			getline(suborPaletyNaSklade, region);
			getline(suborPaletyNaSklade, hmotnost);
			getline(suborPaletyNaSklade, strDatPrich);
			Datum* datPrich = strToDatum(strDatPrich);

			if (stoi(jePrvaTrieda) == 1)
			{
				getline(suborPaletyNaSklade, strDatDoruc);
				Datum* datDoruc = strToDatum(strDatDoruc);
				Paleta* novaPaleta = new Paleta(stoi(region), stoi(hmotnost), *datDoruc, *datPrich, 1, dodavatel);
				paletyNaSklade_->add(novaPaleta); // prida ju do skladu
				for each (Region* aktReg in *regiony_)
				{
					if (aktReg->getCislo() == stoi(region))
					{
						aktReg->pridajPaletuPrvejTriedy(stoi(hmotnost), *novaPaleta); // prida ju regionu
						break;
					}
				}
				delete datDoruc;
			}
			else
			{
				Datum* datLubov = new Datum(1, 1, 1960);
				Paleta* novaPaleta = new Paleta(stoi(region), stoi(hmotnost), *datLubov, *datPrich, 1, dodavatel);
				paletyNaSklade_->add(novaPaleta);
				for each (Region* aktReg in *regiony_)
				{
					if (aktReg->getCislo() == stoi(region))
					{
						aktReg->pridajPaletuDruhejTriedy(*novaPaleta); // prida ju regionu
						break;
					}
				}
				delete datLubov;
			}
			delete datPrich;
		}
		std::cout << "Zaevidovane palety na sklade nacitane zo suboru\n";
		suborPaletyNaSklade.close();
	}
	else
	{
		std::cout << "Subor ZaevidovanePalety.txt sa nepodarilo otvorit\n";
	}

}

void Sklad::nacitajRegionyPrveTriedyNaDnes()
{
	std::string pocetRegionov;
	std::string cislo;
	std::ifstream suborRegionyPrveTriedyDnes("RegionyPrveTriedyDnes" + priponaZapis_ + ".txt");
	if (suborRegionyPrveTriedyDnes.is_open())
	{
		getline(suborRegionyPrveTriedyDnes, pocetRegionov);
		for (int i = 0; i < stoi(pocetRegionov); i++)
		{
			getline(suborRegionyPrveTriedyDnes, cislo);
			for each (Region* aktReg in *regiony_)
			{
				if (aktReg->getCislo() == stoi(cislo))
				{
					regionyPodlaNajlahsichPalietPrvejTriedy_->push(aktReg->peekPaletyPrvejTriedyNaNajmensiDatum()->getHmotnost(), aktReg);
					break;
				}
			}
		}		
		std::cout << "Regiony prve triedy dnes za nacitali zo suboru\n";
		suborRegionyPrveTriedyDnes.close();
	}
	else
	{
		std::cout << "Subor RegionyPrveTriedyDnes.txt sa nepodarilo otvorit\n";
	}

}

void Sklad::nacitajRegionyDruheTriedy()
{
	std::string pocetRegionov;
	std::string cislo;
	std::ifstream suborRegionyNajlahsieDruheTriedy("RegionyNajlahsieDruheTriedy" + priponaZapis_ + ".txt");
	if (suborRegionyNajlahsieDruheTriedy.is_open())
	{
		getline(suborRegionyNajlahsieDruheTriedy, pocetRegionov);
		for (int i = 0; i < stoi(pocetRegionov); i++)
		{
			getline(suborRegionyNajlahsieDruheTriedy, cislo);
			for each (Region* aktReg in *regiony_)
			{
				if (aktReg->getCislo() == stoi(cislo))
				{
					regionyPodlaNajlahsichPalietDruhejTriedy_->push(aktReg->peekPaletyDruhejTriedySNajvacsouPrioritou()->getPrioritaDruheTriedy(), aktReg);
					break;
				}
			}
		}
		std::cout << "Regiony najlahsie druhe triedy za nacitali zo suboru\n";
		suborRegionyNajlahsieDruheTriedy.close();
	}
	else
	{
		std::cout << "Subor RegionyNajlahsieDruheTriedy.txt sa nepodarilo otvorit\n";
	}
}

Datum * Sklad::strToDatum(std::string strDatum)
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
