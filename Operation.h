#ifndef _OPERATION_H_
#define _OPERATION_H_
#include "Pixel.h"
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

class Operation
{
public:
	virtual void apply(Pixel* pixel, int cnst = 0, vector<Pixel*> *adjacent_pixels = nullptr) = 0;
};

class Addition : public Operation
{
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(p->getRed() + cnst, p->getGreen() + cnst, p->getBlue() + cnst, p->getAlpha());
	}
};

class Subtraction : public Operation
{
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(p->getRed() - cnst, p->getGreen() - cnst, p->getBlue() - cnst, p->getAlpha());
	}
};

class InverseSubtraction : public Operation
{
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(cnst - p->getRed(), cnst - p->getGreen(), cnst - p->getBlue(), p->getAlpha());
	}
};

class Multiplication : public Operation
{
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(cnst * p->getRed(),cnst * p->getGreen(),cnst * p->getBlue(), p->getAlpha());
	}
};

class Division : public Operation
{
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		if (cnst == 0)
			return;
		p->setParameters(p->getRed() / cnst, p->getGreen() / cnst, p->getBlue() / cnst, p->getAlpha());
	}
};

class InverseDivision : public Operation
{
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
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters((int)pow(p->getRed(), cnst), (int)pow(p->getGreen(), cnst), (int)pow(p->getBlue(), cnst), p->getAlpha());
	}
};

class Logarithm : public Operation
{
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters((int)log(p->getRed()), (int)log(p->getGreen()), (int)log(p->getBlue()), p->getAlpha());
	}
};

class Abs : public Operation
{
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(abs(p->getRed()), abs(p->getGreen()), abs(p->getBlue()), p->getAlpha());
	}
};

class Min : public Operation
{
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(std::min(p->getRed(), cnst), std::min(p->getGreen(), cnst), std::min(p->getBlue(), cnst), p->getAlpha());
	}
};

class Max : public Operation
{
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(std::max(p->getRed(), cnst), std::max(p->getGreen(), cnst), std::max(p->getBlue(), cnst), p->getAlpha());
	}
};

class Invert : public Operation
{
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		p->setParameters(255 - p->getRed(), 255 - p->getGreen(), 255 - p->getBlue(), p->getAlpha());
	}
};

class Grayscale : public Operation
{
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		double arithmetic_mean = (p->getRed() + p->getGreen() + p->getBlue())*1.0 / 3;
		p->setParameters((int)arithmetic_mean, (int)arithmetic_mean, (int)arithmetic_mean, p->getAlpha());
	}
};

class BlackAndWhite : public Operation
{
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
	{
		double arithmetic_mean = (p->getRed() + p->getGreen() + p->getBlue()) * 1.0 / 3;
		int value = arithmetic_mean < 127 ? 0 : 255;
		p->setParameters(value, value, value, p->getAlpha());
	}
};

class Median : public Operation
{
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
		//pixel->setParameters(red_vector[red_vector.size() / 2], green_vector[green_vector.size() / 2], blue_vector[blue_vector.size() / 2], pixel->getAlpha());
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