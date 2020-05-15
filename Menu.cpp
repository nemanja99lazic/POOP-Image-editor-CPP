#include <iostream>
#include "Menu.h"
using namespace std;

void Menu::writeBeforeMain()
{
	cout << "Unesi opciju: " << endl;
	cout << "1. Ucitavanje prazne slike sa zadavanjem dimenzija: " << endl;
	cout << "2. Zadaj putanju do slike:" << endl;
	cout << "3. Ucitaj sacuvano stanje slike iz XML datoteke" << endl;
	cout << "Opcija:";
}

void Menu::writeMain()
{
	cout << endl;
	cout << "GLAVNI MENI:" << endl;
	cout << "Unesi opciju: " << endl;
	cout << "1. Rad sa slojevima" << endl;
	cout << "2. Rad sa selekcijama" << endl;
	cout << "3. Operacije" << endl;
	cout << "4. Eksportuj" << endl;
	cout << "0. Kraj rada" << endl;
	cout << "Opcija:";
}

void Menu::writeLayerMenu()
{
	cout << endl;
	cout << "MENI ZA RAD SA SLOJEVIMA" << endl;
	cout << "Izaberi opciju" << endl;
	cout << "1. Dodaj sloj" << endl;
	cout << "2. Promeni neprozirnost sloja" << endl;
	cout << "3. Aktiviraj sloj" << endl;
	cout << "4. Deaktiviraj sloj" << endl;
	cout << "5. Izbrisi sloj" << endl;
	cout << "0. Vrati se nazad" << endl;
	cout << "Opcija: ";
}

void Menu::writeSelectionMenu()
{
	cout << endl;
	cout << "MENI ZA RAD SA SELEKCIJAMA" << endl;
	cout << "1. Dodaj selekciju" << endl;
	cout << "2. Aktiviraj selekciju" << endl;
	cout << "3. Deaktiviraj selekciju" << endl;
	cout << "4. Izbrisi selekciju" << endl;
	cout << "5. Popuni selekciju zadatom bojom" << endl;
	cout << "0. Vrati se nazad" << endl;
	cout << "Opcija: ";
}

void Menu::writeOperationMenu()
{
	cout << endl;
	cout << "MENI ZA RAD SA OPERACIJAMA" << endl;
	cout << "1. Ucitaj operacije pomocu standardnog ulaza i primeni na slici" << endl;
	cout << "2. Ucitaj operacije iz datoteke i primeni na slici" << endl;
	cout << "3. Ucitaj operacije pomocu standardnog ulaza i sacuvaj ih u datoteku ne primenjujuci ih na sliku" << endl;
	cout << "4. Vrati se nazad" << endl;
}

void Menu::writeExportMenu()
{
	cout << endl;
	cout << "EKSPORTOVANJE" << endl;
	cout << "Unesi putanju do datoteke u kojoj ce se cuvati slika" << endl;
}