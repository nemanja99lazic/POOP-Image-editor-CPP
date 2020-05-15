#ifndef _COMPOSITEOPERATION_H_
#define _COMPOSITEOPERATION_H_
#include "Operation.h"
#include <iostream>
#include <list>
class CompositeOperation : public Operation
{
public:
	CompositeOperation(std::string& _name, list<Operation*>& _operation_list, const std::string& _path) { name = _name; operation_list = _operation_list; path = _path; }
	friend static void Operation::helperSaveOperation(Operation*, const std::string&);
	
	// ucitaj kompozitnu iz fajla
	static Operation* loadCompositeOperationFromFile(const string path);
private:
	void apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels);
	static Operation* getPtrOnFunctionType(const string type, int cnst);
};

#endif