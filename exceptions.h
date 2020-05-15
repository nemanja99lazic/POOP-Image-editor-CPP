#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_
#include <exception>

class GNePostojiOpcija : public std::exception
{
public:
	const char* what() const override
	{
		return "Izabrana opcija ne postoji.";
	}
};

class GNePostojiFajl : public std::exception
{
public:
	const char* what() const override
	{
		return "Fajl sa datom putanjom ne postoji";
	}
};

class GNeodgovarajuciTipFajla : public std::exception
{
public:
	const char* what() const override
	{
		return "Ovaj program ne podrzava rad sa datim tipom fajla";
	}
};

class GPrekoracenjeMaxIndeksaVektora : public std::exception
{
public:
	const char* what() const override
	{
		return "Uneti indeks prekoracuje broj najveceg indeksa vektora";
	}
};

class GPostojiSelekcija : public std::exception
{
public:
	const char* what() const override
	{
		return "Selekcija sa unetim imenom vec postoji";
	}
};

class GNePostojiSelekcija : public std::exception
{
public:
	const char* what() const override
	{
		return "Selekcija sa unetim imenom ne postoji";
	}
};

class GNePostojiSacuvanaDatotekaSloja : public std::exception
{
public:
	const char* what() const override
	{
		return "Datoteka sa sacuvanim slojem ne postoji ili je ostecena";
	}
};

class GNePostojiOperacijaUHardkodovanojFunkciji : public std::exception
{
public:
	const char* what() const override
	{
		return "U funkciji Operation* CompositeOperation::getPtrOnFunctionType(const string type) ne postoji odgovarajuca funkcija";
	}
};

class GRedniBrojVanGranica : public std::exception
{
public:
	const char* what() const override
	{
		return "Uneti redni broj je van granica niza slojeva";
	}
};

#endif

