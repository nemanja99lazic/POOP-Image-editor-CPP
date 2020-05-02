#ifndef _LAYER_H_
#define _LAYER_H_
#include <vector>
#include "Pixel.h"
#include <fstream>
#include "Formatter.h"
using namespace std;

class Layer
{
public:
	Layer(int, int, bool _active = true, int _opacity = 100);
	int getHeight() const
	{
		return height;
	}
	int getWidth() const
	{
		return width;
	}
	vector<vector<Pixel*>>& getPixelMatrix()
	{
		return this->pxvec;
	}
	void setHeight(int _height)
	{
		this->height = _height;
	}
	void setWidth(int _width)
	{
		this->width = _width;
	}
	void setPath(const string& _path)
	{
		path = _path;
	}
	void setToActive()
	{
		active = true;
	}
	void setToInactive()
	{
		active = false;
	}
	void setOpacity(int opac)
	{
		opac = opac < 0 ? 0 : opac;
		opac = opac > 100 ? 100 : opac;
		opacity = opac;
	}
	~Layer();
private:
	friend class Slika;
	void fillWithTransparentPixels(int, int);
private:
	int height;
	int width;
	bool active;
	int opacity;
	string path;
	vector<vector<Pixel*>> pxvec;
	friend class BMPFormatter;
	friend class PAMFormatter;
	//string putanja;
};

#endif

