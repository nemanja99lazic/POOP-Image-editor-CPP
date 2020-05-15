#ifndef _OPERATION_H_
#define _OPERATION_H_
#include "Pixel.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <list>
using namespace std;

class Slika;

class Operation
{
public:
	Operation(int _cnst = -1) : cnst(_cnst) { operation_list.clear(); path = ""; }
	virtual void apply(Pixel* pixel, int cnst = 0, vector<Pixel*> *adjacent_pixels = nullptr) = 0;
	
	// ucitava operacije pomocu tekstualnog menija
	static Operation* loadOperation();
	
	/* cuva operacije u fajl sa ekstenzijom path;
	   - ako nije data operacija, ucitace operaciju pomocu tekstualnog menija i unosa korisnika
	   - ako je data operacija, ta operacija ce biti sacuvana
	*/
	static void saveOperationInFile(const string& path, Operation* = nullptr);
	
	// prosledjuje operaciju slici
	static void passOperationToImage(Slika& img);
	string& getName()
	{
		return name;
	}
	int getConstant() const
	{
		return cnst;
	}
protected:
	int cnst = -1;
	string name = "";
	list<Operation*> operation_list;
	string path = "";
	static void helperSaveOperation(Operation*, const string&);
};

class Addition : public Operation
{
public:
	Addition(int _cnst) : Operation(_cnst)  { name = "Addition"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(p->getRed() + cnst, p->getGreen() + cnst, p->getBlue() + cnst, p->getAlpha());
	}
};

class Subtraction : public Operation
{
public:
	Subtraction(int _cnst) : Operation(_cnst) { name = "Subtraction"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(p->getRed() - cnst, p->getGreen() - cnst, p->getBlue() - cnst, p->getAlpha());
	}
};

class InverseSubtraction : public Operation
{
public:
	InverseSubtraction(int _cnst) : Operation(_cnst) { name = "InverseSubtraction"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(cnst - p->getRed(), cnst - p->getGreen(), cnst - p->getBlue(), p->getAlpha());
	}
};

class Multiplication : public Operation
{
public:
	Multiplication(int _cnst) : Operation(_cnst) { name = "Multiplication"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(cnst * p->getRed(),cnst * p->getGreen(),cnst * p->getBlue(), p->getAlpha());
	}
};

class Division : public Operation
{
public:
	Division(int _cnst) : Operation(_cnst) { name = "Division"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		if (cnst == 0)
			return;
		p->setParameters(p->getRed() / cnst, p->getGreen() / cnst, p->getBlue() / cnst, p->getAlpha());
	}
};

class InverseDivision : public Operation
{
public:
	InverseDivision(int _cnst) : Operation(_cnst) { name = "InverseDivision"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		int red = p->getRed() == 0 ? 0 : cnst / p->getRed();
		int green = p->getGreen() == 0 ? 0 : cnst / p->getGreen();
		int blue = p->getBlue() == 0 ? 0 : cnst / p->getBlue();

		p->setParameters(red, green, blue, p->getAlpha());
	}
};

class Power : public Operation
{
public:
	Power(int _cnst) : Operation(_cnst) { name = "Power"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters((int)pow(p->getRed(), cnst), (int)pow(p->getGreen(), cnst), (int)pow(p->getBlue(), cnst), p->getAlpha());
	}
};

class Logarithm : public Operation
{
public:
	Logarithm(int _cnst) : Operation(_cnst) { name = "Logarithm"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters((int)log(p->getRed()), (int)log(p->getGreen()), (int)log(p->getBlue()), p->getAlpha());
	}
};

class Abs : public Operation
{
public:
	Abs() { name = "Abs"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(abs(p->getRed()), abs(p->getGreen()), abs(p->getBlue()), p->getAlpha());
	}
};

class Min : public Operation
{
public:
	Min(int _cnst) : Operation(_cnst) { name = "Min"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(std::min(p->getRed(), cnst), std::min(p->getGreen(), cnst), std::min(p->getBlue(), cnst), p->getAlpha());
	}
};

class Max : public Operation
{
public:
	Max(int _cnst) : Operation(_cnst) { name = "Max"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(std::max(p->getRed(), cnst), std::max(p->getGreen(), cnst), std::max(p->getBlue(), cnst), p->getAlpha());
	}
};

class Invert : public Operation
{
public:
	Invert() { name = "Invert"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(255 - p->getRed(), 255 - p->getGreen(), 255 - p->getBlue(), p->getAlpha());
	}
};

class Grayscale : public Operation
{
public:
	Grayscale() { name = "Grayscale"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		double arithmetic_mean = (p->getRed() + p->getGreen() + p->getBlue())*1.0 / 3;
		p->setParameters((int)arithmetic_mean, (int)arithmetic_mean, (int)arithmetic_mean, p->getAlpha());
	}
};

class BlackAndWhite : public Operation
{
public:
	BlackAndWhite() { name = "BlackAndWhite"; }
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		double arithmetic_mean = (p->getRed() + p->getGreen() + p->getBlue()) * 1.0 / 3;
		int value = arithmetic_mean < 127 ? 0 : 255;
		p->setParameters(value, value, value, p->getAlpha());
	}
};

class Median : public Operation
{
public:
	Median() { name = "Median"; }
private:
	void apply(Pixel* pixel, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		int red, green, blue;
		vector<int> red_vector, green_vector, blue_vector;
		for (Pixel* p : *adjacent_pixels)
		{
			if (p != nullptr)
			{
				red_vector.push_back(p->getRed());
				green_vector.push_back(p->getGreen());
				blue_vector.push_back(p->getBlue());
			}
		}
		sort(red_vector.begin(), red_vector.end());
		sort(green_vector.begin(), green_vector.end());
		sort(blue_vector.begin(), blue_vector.end());
		red = red_vector.size() % 2 == 0 ? (red_vector[red_vector.size() / 2] + red_vector[red_vector.size() / 2 - 1]) / 2 : red_vector[red_vector.size() / 2];
		green = green_vector.size() % 2 == 0 ? (green_vector[green_vector.size() / 2] + green_vector[green_vector.size() / 2 - 1]) / 2 : green_vector[green_vector.size() / 2];
		blue = blue_vector.size() % 2 == 0 ? (blue_vector[blue_vector.size() / 2] + blue_vector[blue_vector.size() / 2 - 1]) / 2 : blue_vector[blue_vector.size() / 2];
		pixel->setParameters(red, green, blue, pixel->getAlpha());
		blue_vector.clear();
		green_vector.clear();
		red_vector.clear();
	}
};

#endif