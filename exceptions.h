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


#endif

