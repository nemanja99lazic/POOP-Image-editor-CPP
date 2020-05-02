#include "Slika.h"
#include <utility>
using namespace std;

Slika* Slika::instance = nullptr;

int Slika::getWidth() const
{
	return this->width;
}

int Slika::getHeight() const
{
	return this->height;
}

void Slika::deleteInstance()
{
	for (Layer* l : Slika::instance->layers)   
		delete l;
}

string Slika::findImageFormat(const string& path)
{
	regex rx("(.*)\\.(...)");
	smatch result;
	regex_match(path, result, rx); //dobro odredjuje format dokumenta samo ako se koristi getline!!!!!
	string format = result.str(2);
	return format;
}

void Slika::addLayer(const string& layer_path, int _opacity)
{
	Formatter* f = nullptr;
	string format = findImageFormat(layer_path);
	if (format == "bmp")
		f = new BMPFormatter(layer_path);
	else
		if (format == "pam")
			f = new PAMFormatter(layer_path);
		else
		/*	if (format == "xml" || format == "XML")
				f = XMLFormatter(layer_path);
			else*/
				throw GNeodgovarajuciTipFajla();
	
	//Ubaci dobijeni layer na pocetak niza layera
	layers.resize(layers.size() + 1);
	for (int i = layers.size() - 1; i > 0; i--)
		layers[i] = layers[i - 1];
	layers[0] = f->getLayer();
	layers[0]->setPath(layer_path);
	layers[0]->setOpacity(_opacity);
	this->height = (*max_element(layers.begin(), layers.end(), [](Layer* a, Layer* b)->bool {return a->getHeight() < b->getHeight(); }))->getHeight();
	this->width = (*max_element(layers.begin(), layers.end(), [](Layer* a, Layer* b)->bool {return a->getWidth() < b->getWidth(); }))->getWidth();
	
	//Popuni providnim pikselima
	for(Layer* l : layers)
	{
		l->fillWithTransparentPixels(this->getHeight(), this->getWidth());
		l->setHeight(this->getHeight());
		l->setWidth(this->getWidth());
	}
}

void Slika::setSelectionToActive(const string& name)
{
	auto p = this->selection_map.find(name);
	if (p == selection_map.end())
		throw GNePostojiSelekcija();
	else
	{
		p->second->setToActive();
		this->hasActiveSelection = true;
	}
}

void Slika::setSelectionToInactive(const string& name)
{
	auto p = this->selection_map.find(name);
	if (p == selection_map.end())
		throw GNePostojiSelekcija();
	else
	{
		p->second->setToInactive();
		bool check_activity = false;
		for(pair<const string, Selection*> &tempp : selection_map)
			if (tempp.second->isActive())
			{
				check_activity = true;
				break;
			}
		if (check_activity == false)
			this->hasActiveSelection = false;
	}
}

void Slika::deleteLayer(int ind)
{
	if (ind >= this->layers.size())
		throw GPrekoracenjeMaxIndeksaVektora();
	else
	{
		Layer* layer_to_delete = this->layers[ind];
		for (int i = ind; i + 1 < layers.size(); i++)
			this->layers[i] = this->layers[i + 1];
		this->layers.resize(this->layers.size() - 1);
		delete layer_to_delete;
	}
}

Slika::~Slika()
{
	Slika::deleteInstance();
}

void Slika::addSelection(const string& name)
{
	int rup, rleft, rheight, rwidth;
	vector<Rectangle*> rec_vector;
	auto test = this->selection_map.find(name);
	if (test != selection_map.end())
		throw GPostojiSelekcija();
	cout << "Unesi pravougaonike selekcija u formatu: start_pozicija_gore start_pozicija_levo visina sirina" << endl;
	cout << "Za kraj unesi 0 za visinu ili sirinu" << endl;
	cin >> rup >> rleft >> rheight >> rwidth;
	while (rheight != 0 || rwidth != 0)
	{
		if (rup >= getHeight() || rleft >= getWidth())
			continue;
		else
		{
			if (rup + rheight > height)
				rheight = height - rup;
			if (rleft + rwidth > width)
				rwidth = width - rleft;
		}
		rec_vector.push_back(new Rectangle(rup, rleft, rheight, rwidth));
		cin >> rup >> rleft >> rheight >> rwidth;
	}
	this->selection_map[name] = new Selection(rec_vector);
	this->hasActiveSelection = true;
}

void Slika::save(const string& path)
{
	Formatter* f = nullptr;
	string format = Slika::findImageFormat(path);
	if (format == "bmp")
		f = new BMPFormatter();
	else
		if (format == "pam")
			f = new PAMFormatter();
		else
		/*	if (format == "xml")
				f = new XMLFormatter();
			else*/
				throw GNeodgovarajuciTipFajla();
	f->save(path, *this);
	exported = true;
}

void Slika::applyOperation(int cnst)
{
	vector<Pixel*> blur_vector;
	if (this->hasActiveSelection)
	{	
		for (pair<const string, Selection*>& selection_pair : this->selection_map)
		{
			if (selection_pair.second->isActive())
				for (Rectangle* rec : selection_pair.second->getRectangleVector())
					for (Layer* l : this->layers)
					{
						int starth = rec->getUp();
						int stoph = rec->getUp() + rec->getHeight();
						int startw = rec->getLeft();
						int stopw = rec->getLeft() + rec->getWidth();
						for(int i = starth; i < stoph; i++) // nije mogla for_each zbog medijane, jer treba da se izdvoje iteratori
							for (int j = startw; j < stopw; j++)
							{
								insertInVector(blur_vector, l->pxvec, starth, stoph, startw, stopw, i, j);
								Pixel* p = l->pxvec[i][j];
								this->operation->apply(p, cnst, &blur_vector);
								blur_vector.clear();
							}
					}
		}
	}
	else
	{
		for (Layer* l : this->layers)
		{
			int starth = 0;
			int stoph = l->pxvec.size();
			int startw = 0;
			int stopw = l->pxvec[0].size();
			for(int i = starth; i < stoph; i++)
				for (int j = startw; j < stopw; j++)
				{
					insertInVector(blur_vector, l->pxvec, starth, stoph, startw, stopw, i, j);
					Pixel* p = l->pxvec[i][j];
					this->operation->apply(p, cnst, &blur_vector);
					blur_vector.clear();
				}
		}
	}
}

void Slika::fixOverflow()
{
	if (this->hasActiveSelection)
	{
		for (pair<const string, Selection*>& selection_pair : this->selection_map)
		{
			if (selection_pair.second->isActive())
				for (Rectangle* rec : selection_pair.second->getRectangleVector())
					for (Layer* l : this->layers)
					{
						auto starth = l->pxvec.begin() + rec->getUp();
						auto endh = starth + rec->getHeight();

						for_each(starth, endh, [rec, this](vector<Pixel*>& v) {
							auto startw = v.begin() + rec->getLeft();
							auto endw = startw + rec->getWidth();
							for_each(startw, endw, [this](Pixel* p) {
								p->setParameters(p->getRed() % 256, p->getGreen() % 256, p->getBlue() % 256, p->getAlpha());
								});
							});
					}
		}
	}
	else
	{
		for (Layer* l : this->layers)
		{
			for_each(l->pxvec.begin(), l->pxvec.end(), [this](vector<Pixel*>& v) {
				for_each(v.begin(), v.end(), [this](Pixel* p) {
					p->setParameters(p->getRed() % 256, p->getGreen() % 256, p->getBlue() % 256, p->getAlpha());
					});
				});
		}
	}
}

void Slika::insertInVector(vector<Pixel*>&blur_vector, const vector<vector<Pixel*>> &pxvec, int starth, int stoph, int startw, int stopw, int i, int j)
{
	i - 1 >= starth && j - 1 >= startw ? blur_vector.push_back(pxvec[i - 1][j - 1]) : blur_vector.push_back(nullptr);
	i - 1 >= starth ? blur_vector.push_back(pxvec[i - 1][j]) : blur_vector.push_back(nullptr);
	i - 1 >= starth && j + 1 < stopw ? blur_vector.push_back(pxvec[i - 1][j + 1]) : blur_vector.push_back(nullptr);
	j + 1 < stopw ? blur_vector.push_back(pxvec[i][j + 1]) : blur_vector.push_back(nullptr);
	i + 1 < stoph && j + 1 < stopw ? blur_vector.push_back(pxvec[i + 1][j + 1]) : blur_vector.push_back(nullptr);
	i + 1 < stoph ? blur_vector.push_back(pxvec[i + 1][j]) : blur_vector.push_back(nullptr);
	i + 1 < stoph && j - 1 >= startw ? blur_vector.push_back(pxvec[i + 1][j - 1]) : blur_vector.push_back(nullptr);
	j - 1 >= startw ? blur_vector.push_back(pxvec[i][j - 1]) : blur_vector.push_back(nullptr);
}

Selection& Slika::getSelectionByName(const string& name)
{
	auto iter = std::find_if(this->selection_map.begin(), this->selection_map.end(), [&name](pair<const string, Selection*>& sel_pair)->bool {
		if (sel_pair.first == name)
			return true;
		else
			return false; 
		});
	if (iter == this->selection_map.end())
		throw GNePostojiSelekcija();
	else
		return *iter->second;
}