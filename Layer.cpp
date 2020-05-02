#include "Layer.h"
#include <algorithm>

Layer::Layer(int _height, int _width, bool _active, int _opacity) : height(_height), width(_width), active(_active)
{
	_opacity = _opacity > 100 ? 100 : _opacity;
	_opacity = _opacity < 0 ? 0 : _opacity;
	opacity = _opacity;
	this->pxvec.resize(height);
}

Layer::~Layer()
{
	for (vector<Pixel*>& v : pxvec)
	{
		v.clear();
	}
	pxvec.clear();
}

void Layer::fillWithTransparentPixels(int maxHeight, int maxWidth)
{
	pxvec.resize(maxHeight);
	for (vector<Pixel*>& v : pxvec)
	{
		v.resize(maxWidth, new Pixel(255, 255, 255, 0));
	}
	auto height_begin = pxvec.begin() + this->height;
	for_each(height_begin, pxvec.end(), [this](vector<Pixel*> &v) {
		for_each(v.begin(), v.end(), [](Pixel* p) {
			p = new Pixel(255, 255, 255, 0);
		});
	});
}