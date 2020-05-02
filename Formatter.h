#ifndef _FORMATTER_H_
#define _FORMATTER_H_
#include <iostream>
#include "Layer.h"
#include <cstdint>
using namespace std;

class Layer;
class Slika;
using byte = uint8_t;
using word = uint16_t;
using dword = uint32_t;

class Formatter
{
public:
	Layer* getLayer()
	{
		return layer;
	}
	virtual void save(const string& path, const Slika& img) = 0;
	virtual ~Formatter() = 0;
protected:
	Layer* layer;
};

class PAMFormatter : public Formatter
{
public:
	PAMFormatter() {}
	PAMFormatter(const string&);
	void save(const string& path, const Slika& img);
private:
#pragma pack(push, 1)
	struct PAMHeader
	{
		string magic_word = "P7";
		unsigned width;
		int height;
		int depth;
		int maxval;
		string tupltype;
		string end_of_header_word = "ENDHDR";
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct PAMPixel
	{
		byte r, g, b, a;
	};
#pragma pack(pop)
	PAMHeader header;
	static const int newline = 0x0a;

};

class XMLFormatter : public Formatter
{
public:
	XMLFormatter() {}
};

class BMPFormatter : public Formatter
{
public:
	BMPFormatter() {}
	BMPFormatter(const string&);
	void save(const string&, const Slika&);
	~BMPFormatter() {}
private:
#pragma pack(push, 1)
	struct BMPHeader
	{
	public:
		word id_field = 0x4d42;
		dword size_of_file; // treba da se promeni
		word unused1 = 0x0000; 
		word unused2 = 0x0000; 
		dword start_of_pixels_array = 0x0000007a;  
		dword bytes_in_DIB_header = 0x0000006c;  
		int32_t width;  //treba da se promeni
		int32_t height; // treba da se promeni
		word number_of_color_planes = 0x0001; 
		word number_of_bits_per_pixel = 0x0020; 
		dword BI_RGB = 0x00000003;
		dword size_of_bitmap_in_bytes;  // promenljivo
		int32_t unused3_fixed = 0x00000b13;
		int32_t unused4_fixed = 0x00000b13;
		dword unused5_fixed = 0;
		dword unused6_fixed = 0;
		//pocetak niza piksela za 24-bit
		//dodatak za 32-bit
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct BMP32addition
	{
	public:
		dword red_mask = 0x00ff0000;
		dword green_mask = 0x0000ff00;
		dword blue_mask = 0x000000ff;
		dword alpha_mask = 0xff000000;
		dword unused1_fixed = 0x73524742;
		dword unusedarr[16] = { 0 };
	};

#pragma pack(pop)
#pragma pack(push, 1)
	struct BMPPixel
	{
		byte blue;
		byte green;
		byte red;
		byte alpha = 255;
	};
#pragma pack(pop)
	
	BMPHeader header;
	BMP32addition addition;
};

#endif