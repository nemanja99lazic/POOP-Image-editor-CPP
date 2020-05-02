#ifndef _MENU_H_
#define _MENU_H_
#include "exceptions.h"
#include "Slika.h"
#include <iostream>
class Menu
{
public:
	virtual void ispisiMeni() const = 0;
	virtual void izaberiOpciju();
	int dohvatiOpciju() const;
	void unesiPutanju();
	virtual Slika* obradiOpciju(Slika*) { return nullptr; }
	string dohvatiPutanju() const;
protected:
	int opcija;
	string putanja;
};

class GlavniMeni : public Menu
{
public:
	void ispisiMeni() const;
	void izaberiOpciju();
	Slika* obradiOpciju(Slika*);
};

class Opcija1Meni : public Menu
{
public:
	void ispisiMeni() const;
};

class Opcija3Meni : public Menu
{
public:
	void ispisiMeni() const;
	void unesiImeSlikeZaCuvanje();
	string dohvatiImeSlikeZaCuvanje() const;
private:
	string ime_slike_za_cuvanje;
};

#endif
