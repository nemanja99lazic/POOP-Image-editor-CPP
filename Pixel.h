#ifndef _PIXEL_H_
#define _PIXEL_H_

class Pixel
{
public:
	Pixel(int, int, int, int);
	int getRed()
	{
		return r;
	}
	int getGreen()
	{
		return g;
	}
	int getBlue()
	{
		return b;
	}
	int getAlpha()
	{
		return a;
	}
	void setRed(int red)
	{
		r = red;
	}
	void setGreen(int green)
	{
		g = green;
	}
	void setBlue(int blue)
	{
		b = blue;
	}
	void setAlpha(int alpha)
	{
		a = alpha;
	}
	void setParameters(int _r, int _g, int _b, int _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
private:
	int r, g, b, a;
};

#endif

