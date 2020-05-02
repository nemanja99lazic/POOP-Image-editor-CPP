#include "Formatter.h"
#include <fstream>
#include "exceptions.h"
#include "Slika.h"
#include <string>
using namespace std;

Formatter::~Formatter() {}


BMPFormatter::BMPFormatter(const string& putanja)
{
	ifstream file;
	file.open(putanja, ios::binary);
	if (!file.is_open())
		throw GNePostojiFajl();
	file.read((char*)&this->header, sizeof(header));
	/*if (header.number_of_bits_per_pixel == 32)
		file.read((char*)&this->addition, sizeof(addition));*/

	this->layer = new Layer(header.height, header.width);  //dodata sirina i visina
	file.seekg(header.start_of_pixels_array, ios::beg);  // postavi bafer na odgovarajucu poziciju

	BMPPixel px;
	int padding = header.number_of_bits_per_pixel == 32 ? 0 : header.width % 4;
	byte tempv_for_padding;
	// prethodna verzija : for (int i = header.height - 1; i >= 0; i--)
	for (int i = header.height - 1; i >= 0; i--)
	{
		for (int j = 0; j < header.width; j++)
		{
			file.read((char*)&px.blue, sizeof(byte));
			file.read((char*)&px.green, sizeof(byte));
			file.read((char*)&px.red, sizeof(byte));
			if (header.number_of_bits_per_pixel == 32)
				file.read((char*)&px.alpha, sizeof(byte));
			else
				px.alpha = 255;
			// transfer piksela posle ucitavanja
			this->layer->pxvec[i].push_back(new Pixel(px.red, px.green, px.blue, px.alpha));
		}
		for (int i = 0; i < padding; i++)
			file.read((char*)&tempv_for_padding, sizeof(byte));
	}

	file.close();
}

void BMPFormatter::save(const string& path, const Slika& img)
{
	double udeo;
	ofstream f;
	f.open(path, ios::binary);
	// priprema headera
	this->header.width = img.getWidth();
	this->header.height = img.getHeight();
	this->header.size_of_file = 122 + 4 * this->header.width * this->header.height;
	this->header.size_of_bitmap_in_bytes = 4 * this->header.width * this->header.height;
	f.write((char*)&this->header, sizeof(this->header));
	f.write((char*)&this->addition, sizeof(this->addition));
	//priprema piksela
	double r, g, b, a, total_transparency;
	for (int i = img.getHeight() - 1; i >= 0; i--)
		for (int j = 0; j < img.getWidth(); j++)
		{
			r = g = b = a = 0;
			udeo = 1;
			for (Layer* l : img.layers)
				if (l->active)
				{
					Pixel& sp = *l->pxvec[i][j];
					total_transparency = ((double)sp.getAlpha() / 255) * ((double)l->opacity / 100);
					r += sp.getRed() * total_transparency * udeo;
					g += sp.getGreen() * total_transparency * udeo;
					b += sp.getBlue() * total_transparency * udeo;
					udeo = udeo - udeo * total_transparency;
				}
			udeo = 1 - udeo;
			a = udeo * 255;
			if (udeo != 0)
			{
				r = r / udeo;
				g = g / udeo;
				b = b / udeo;
			}
			else
				r = b = g = 0;
			BMPPixel* p = new BMPPixel;
			p->red = (byte)r;
			p->green = (byte)g;
			p->blue = (byte)b;
			p->alpha = (byte)a;
			f.write((char*)&(*p), sizeof(BMPPixel));
			delete p;
		}
	f.close();
}

PAMFormatter::PAMFormatter(const string& path)
{
	ifstream f(path, ios::binary);
	unsigned char r, g, b, a;
	string num;
	regex rx("([A-Z]* )(.*)");
	smatch result;
	if (!f.is_open())
		throw GNePostojiFajl();
	string line;
	getline(f, line); // ucitaj 1. liniju - P7
	
	getline(f, line); // ucitaj 2. liniju - 
	regex_match(line, result, rx);
	this->header.width = (int)atoi(result.str(2).c_str()); // c_str konvertuje string u char[]
	
	getline(f, line); // ucitaj 3. liniju
	regex_match(line, result, rx);
	this->header.height = (int)atoi(result.str(2).c_str());

	getline(f, line); // ucitaj 4. liniju
	regex_match(line, result, rx);
	this->header.depth = (int)atoi(result.str(2).c_str());

	getline(f, line); // ucitaj 5. liniju
	regex_match(line, result, rx);
	this->header.maxval = (int)atoi(result.str(2).c_str());

	getline(f, line); // ucitaj 6. liniju - tip torke
	getline(f, line); // ucitaj 7. liniju - kraj hedera

	this->layer = new Layer(this->header.height, this->header.width);
	
	// Popuni matricu piksela
	for (int i = 0; i < this->header.height; i++)
	{
		for (int j = 0; j < this->header.width; j++)
		{
			f.read((char*)&r, sizeof(r));
			f.read((char*)&g, sizeof(g));
			f.read((char*)&b, sizeof(b));
			if (this->header.depth == 4)
			{
				f.read((char*)&a, sizeof(a));
			}
			else
				a = 255;
			this->layer->pxvec[i].push_back(new Pixel(r, g, b, a));
		}

	}

	f.close();
}

void PAMFormatter::save(const string& path, const Slika& img)
{
	double udeo;
	ofstream f;
	f.open(path, ios::binary);
	// priprema headera
	this->header.width = img.getWidth();
	this->header.height = img.getHeight();
	this->header.depth = 4;
	this->header.maxval = 255;
	this->header.tupltype = "RGB_ALPHA";
	
	string line[7];
	line[0] = "P7";
	line[1] = "WIDTH " + to_string(this->header.width);
	line[2] = "HEIGHT " + to_string(this->header.height);
	line[3] = "DEPTH " + to_string(this->header.depth);
	line[4] = "MAXVAL " + to_string(this->header.maxval);
	line[5] = "TUPLTYPE " + this->header.tupltype;
	line[6] = "ENDHDR";

	byte newlinechar = 0x0a;
	for (int i = 0; i < 7; i++)
	{
		f.write((char*)line[i].c_str(), sizeof(char) * line[i].size());
		f.write((char*)&newline, sizeof(newlinechar));
	}

	double r, g, b, a, total_transparency;
	for (int i = 0; i < img.getHeight(); i++)
		for (int j = 0; j < img.getWidth(); j++)
		{
			r = g = b = a = 0;
			udeo = 1;
			for (Layer* l : img.layers)
				if (l->active)
				{
					Pixel& sp = *l->pxvec[i][j];
					total_transparency = ((double)sp.getAlpha() / 255) * ((double)l->opacity / 100);
					r += sp.getRed() * total_transparency * udeo;
					g += sp.getGreen() * total_transparency * udeo;
					b += sp.getBlue() * total_transparency * udeo;
					udeo = udeo - udeo * total_transparency;
				}
			udeo = 1 - udeo;
			a = udeo * 255;
			if (udeo != 0)
			{
				r = r / udeo;
				g = g / udeo;
				b = b / udeo;
			}
			else
				r = b = g = 0;
			PAMPixel* p = new PAMPixel;
			p->r = (byte)r;
			p->g = (byte)g;
			p->b = (byte)b;
			p->a = (byte)a;
			f.write((char*)&(*p), sizeof(PAMPixel));
			delete p;
		}
	f.close();
}