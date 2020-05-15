#ifndef _SELECTION_H_
#define _SELECTION_H_
#include "Rectangle.h"
#include <vector>
#include "Layer.h"
using namespace std;
class Selection
{
public:
	Selection(vector<Rectangle*>& _rec_vector, bool _active = true) : rec_vector(_rec_vector), active(_active) {}
	void setToActive()
	{
		active = true;
	}
	void setToInactive()
	{
		active = false;
	}
	bool isActive() const
	{
		return active;
	}
	void fillWithColor(int red, int green, int blue, vector<Layer*> layers);
	void deleteSelection()
	{
		rec_vector.clear();
		active = false;
	}
	vector<Rectangle*>& getRectangleVector()
	{
		return rec_vector;
	}
	~Selection()
	{
		deleteSelection();
	}
private:
	vector<Rectangle*> rec_vector;
	bool active;
};

#endif
