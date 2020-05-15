#ifndef _SLIKA_H_
#define _SLIKA_H_
#include <vector>
#include <iostream>
#include "Layer.h"
#include "Pixel.h"
#include "Formatter.h"
#include <fstream>
#include <regex>
#include <algorithm>
#include "exceptions.h"
#include <map>
#include "Selection.h"
#include "Operation.h"
using namespace std;

class Slika
{
public:
//	void sacuvajUOdgovarajucemFormatu(const string&) const;
	int getWidth() const;
	int getHeight() const;
	vector<Layer*>& getLayersVector()
	{
		return layers;
	}
	Selection& getSelectionByName(const string& name);
	void addLayer(const string&, int = 100);
	void addEmptyLayer(int _height, int _width);
	void setLayerToActive(int b)
	{
		if ((unsigned)b >= layers.size())
			throw GPrekoracenjeMaxIndeksaVektora();
		else
			this->layers[b]->setToActive();
	}
	void setLayerToInactive(int b)
	{
		if ((unsigned)b >= layers.size())
			throw GPrekoracenjeMaxIndeksaVektora();
		else
			this->layers[b]->setToInactive();
	}
	static Slika& getInstance()
	{
		if (instance == nullptr)
		{
			instance = new Slika();
		}
		return *instance;
	}
	void deleteLayer(int);
	void addSelection(const string& name);
	void deleteSelection(const string& name)
	{
		setSelectionToInactive(name); // u ovo funkciji proverava da li selekcija postoji i da li ce posle njenog deaktiviranja ostati aktiviranih layera
		selection_map.erase(name);
	}
	void setSelectionToActive(const string& name);
	void setSelectionToInactive(const string& name);
	void setOperationThenApply(Operation* _operation)
	{
		operation = _operation;
		this->applyOperation(_operation->getConstant());
		this->fixOverflow();
		delete _operation;
	}
	void loadFromXML(const string&);
	void save(const string&);
	bool isExported()
	{
		return exported;
	}
	~Slika();
private:
	Slika() { width = 0; height = 0;}
	static Slika* instance;
	static void deleteInstance();
	friend class BMPFormatter;
	friend class PAMFormatter;
	friend class XMLFormatter;
	static string findImageFormat(const string&);
	void insertInVector(vector<Pixel*>&, const vector<vector<Pixel*>>&, int, int, int, int, int, int);
	void applyOperation(int);
	void fixOverflow();
private:
	vector<Layer*> layers;
	Operation* operation = nullptr;
	bool exported = false;
	map<string, Selection*> selection_map; // map<name,ptr_on_selection> 
	bool hasActiveSelection = false;
	int width;
	int height;
};

#endif
