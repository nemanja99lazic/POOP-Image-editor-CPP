#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_
class Rectangle
{
public:
	Rectangle(int, int, int, int);
	int getUp() const
	{
		return up;
	}
	int getLeft() const
	{
		return left;
	}
	int getHeight() const
	{
		return height;
	}
	int getWidth() const
	{
		return width;
	}
private:
	int up, left, height, width;
};

#endif
