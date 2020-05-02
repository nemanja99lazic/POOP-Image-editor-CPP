/*
#include "Menu.h"
#include <iostream>

using std::cout;
using std::endl;
using std::cin;

void GlavniMeni::ispisiMeni() const
{
	cout << "GLAVNI MENI:" << endl;
	cout << "1. Ucitavanje slike" << endl;
	cout << "2. Rad sa slikom" << endl;
	cout << "3. Eksportuj sliku" << endl;
	cout << "4. Snimi projekat kao XML fajl" << endl;
	cout << "0. Izlaz iz aplikacije" << endl;
}

void Menu::izaberiOpciju()
{
	cout << "Izaberi opciju:" << endl;
	cin >> opcija;
}

int Menu::dohvatiOpciju() const
{
	return opcija;
}

void GlavniMeni::izaberiOpciju()
{
	Menu::izaberiOpciju();
	if (opcija < 0 || opcija>4)
		throw GNePostojiOpcija();
}

void Opcija1Meni::ispisiMeni() const
{
	cout << "Unesi apsolutnu putanju do slike:"<<endl;
}

void Menu::unesiPutanju()
{
	string putanja;
	cin.ignore();
	std::getline(cin, putanja);
	this->putanja = putanja;
}

string Menu::dohvatiPutanju() const
{
	return this->putanja;
}

void Opcija3Meni::ispisiMeni() const
{
	cout << "Cuvanje slike" << endl;
	cout << "Unesi ime slike u formatu ime_slike.format_slike (slika ce biti sacuvana u folderu Resources): " << endl;
}

void Opcija3Meni::unesiImeSlikeZaCuvanje()
{
	cin.ignore();
	cin >> ime_slike_za_cuvanje;
	ime_slike_za_cuvanje = "\\Resources\\" + ime_slike_za_cuvanje;
}

string Opcija3Meni::dohvatiImeSlikeZaCuvanje() const
{
	return ime_slike_za_cuvanje;
}

Slika* GlavniMeni::obradiOpciju(Slika* img_old)
{
	Slika* img = nullptr;
	switch (opcija)
	{
	case 1:
	{
		Menu* m1 = new Opcija1Meni();
		m1->ispisiMeni();
		m1->unesiPutanju();
		img = new Slika(m1->dohvatiPutanju());
		delete m1;
	}
	break;
	case 2:
	{

	}
	break;
	case 3:
	{
		Opcija3Meni& m3 = *(new Opcija3Meni);
		m3.ispisiMeni();
		m3.unesiImeSlikeZaCuvanje();
		img_old->sacuvajUOdgovarajucemFormatu(m3.dohvatiImeSlikeZaCuvanje());
		delete &m3;
		return img_old;
	}
	break;
	case 4:
	{

	}
	break;
	}
	return img;
}
*/
