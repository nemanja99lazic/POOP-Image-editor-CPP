#ifndef _XMLFORMATTER_H_
#define _XMLFORMATTER_H_
#include "Formatter.h"

class XMLFormatter : public Formatter
{
public:
	XMLFormatter() {}
	XMLFormatter(const string& path, Slika& img); //load
	void save(const string& path, const Slika& img);
private:
#pragma pack(push,1)
	struct XMLLayerPixel
	{
		XMLLayerPixel(byte _r, byte _g, byte _b, byte _a) : r(_r), g(_g), b(_b), a(_a) {}
		byte r;
		byte g;
		byte b;
		byte a;
	};
#pragma pack(pop)
	static const string ext; // dodatak na putanju za cuvanje sloja kod xmla
	static const string format; // format binarne datoteke za upis piksela sloja
};

#endif