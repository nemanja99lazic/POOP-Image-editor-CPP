#include "CompositeOperation.h"
#include <regex>
#include "pugixml.hpp"
#include "exceptions.h"
#include <iostream>
#include <map>
using namespace std;

void CompositeOperation::apply(Pixel* p, int cnst, vector<Pixel*>* adjacent_pixels)
{
	for(Operation* op : this->operation_list)
		op->apply(p, op->getConstant(), adjacent_pixels);
}

Operation* CompositeOperation::loadCompositeOperationFromFile(const string path)
{
	const string basic_function_root_name = "BasicFunction";
	const string composite_function_root_name = "CompositeFunction";
	string name;
	int cnst;
	list<Operation*> op_list;

	regex rx("(.*)\\.(...)");
	smatch result;
	regex_match(path, result, rx); //dobro odredjuje format dokumenta samo ako se koristi getline!!!!!
	string extension = result.str(2);

	if (extension != "fun")
		throw GNeodgovarajuciTipFajla();

	pugi::xml_document doc;

	pugi::xml_parse_result res = doc.load_file(path.c_str());
	if (res == false)
		throw GNePostojiFajl();

	pugi::xml_node rootFunction = doc.first_child();
	name = string(rootFunction.attribute("name").value());

	for (auto iter : rootFunction.children())
	{
		string rootName = iter.name();
		if (rootName == basic_function_root_name)
		{
			string functionType = iter.first_child().text().as_string();
			cnst = atoi(iter.attribute("cnst").value());
			Operation* returned_operation = CompositeOperation::getPtrOnFunctionType(functionType, cnst);
			if (returned_operation == nullptr)
				throw GNePostojiOperacijaUHardkodovanojFunkciji();
			op_list.push_back(returned_operation);
		}
		else
			if (rootName == composite_function_root_name)
			{
				pugi::xml_node rootPath = iter.first_child();
				string path_to_internal_composite = rootPath.first_child().text().as_string();
				Operation* compositeFromXML = CompositeOperation::loadCompositeOperationFromFile(path_to_internal_composite);
				op_list.push_back(compositeFromXML);
			}
	}
	return new CompositeOperation(name, op_list, path);
}

Operation* CompositeOperation::getPtrOnFunctionType(const string type, int cnst)
{
	/*map<string, Operation*> temp_fmap;
	temp_fmap["Addition"] = new Addition();
	temp_fmap["Subtraction"] = new Subtraction();
	temp_fmap["InverseSubtraction"] = new InverseSubtraction();
	temp_fmap["Multiplication"] = new Multiplication();
	temp_fmap[""]*/

	if (type == "Addition")
		return new Addition(cnst);
	if (type == "Subtraction")
		return new Subtraction(cnst);
	if (type == "InverseSubtraction")
		return new InverseSubtraction(cnst);
	if (type == "Multiplication")
		return new Multiplication(cnst);
	if (type == "Division")
		return new Division(cnst);
	if (type == "InverseDivision")
		return new InverseDivision(cnst);
	if (type == "Power")
		return new Power(cnst);
	if (type == "Logarithm")
		return new Logarithm(cnst);
	if (type == "Abs")
		return new Abs();
	if (type == "Min")
		return new Min(cnst);
	if (type == "Max")
		return new Max(cnst);
	if (type == "Invert")
		return new Invert();
	if (type == "Grayscale")
		return new Grayscale();
	if (type == "BlackAndWhite")
		return new BlackAndWhite();
	if (type == "Median")
		return new Median();
	return nullptr;
}