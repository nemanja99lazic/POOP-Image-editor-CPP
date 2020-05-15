#include "Operation.h"
#include "CompositeOperation.h"
#include "Slika.h"
#include "pugixml.hpp"
#include <string>

class CompositeOperation;

Operation* Operation::loadOperation()
{
	string name;
	Operation* op;
	list<Operation* > op_list;
	int cnst = -1;
	cout << "Unesi ime operacije" << endl;
	cin >> name;
	cout << "Izaberi operaciju unosom rednog broja (za izlaz unesi 0)" << endl;
	int opcija = 1;
	while (opcija != 0)
	{
		cout << "1. Sabiranje" << endl;
		cout << "2. Oduzimanje" << endl;
		cout << "3. Inverzno oduzimanje" << endl;
		cout << "4. Mnozenje" << endl;
		cout << "5. Deljenje" << endl;
		cout << "6. Inverzno deljenje" << endl;
		cout << "7. Power" << endl;
		cout << "8. Logaritam" << endl;
		cout << "9. Apsolutna vrednost" << endl;
		cout << "10. Minimum" << endl;
		cout << "11. Maksimum" << endl;
		cout << "12. Invert" << endl;
		cout << "13. Grayscale" << endl;
		cout << "14. Black and white" << endl;
		cout << "15. Median" << endl;
		cout << "16. Ucitaj kompozitnu funkciju iz fajla" << endl;
		cout << "0. Prestani sa ucitavanjem" << endl;

		cin >> opcija;
		switch (opcija)
		{
		case 1:
		{
			cout << "Unesi konstantu operacije" << endl;
			cin >> cnst;
			op_list.push_back(new Addition(cnst));
		}
			break;
		case 2:
		{
			cout << "Unesi konstantu operacije" << endl;
			cin >> cnst;
			op_list.push_back(new Subtraction(cnst));
		}break;
		case 3:
		{
			cout << "Unesi konstantu operacije" << endl;
			cin >> cnst;
			op_list.push_back(new InverseSubtraction(cnst));
		}break;
		case 4:
		{
			cout << "Unesi konstantu operacije" << endl;
			cin >> cnst;
			op_list.push_back(new Multiplication(cnst));
		}break;
		case 5:
		{
			cout << "Unesi konstantu operacije" << endl;
			cin >> cnst;
			op_list.push_back(new Division(cnst));
		}break;
		case 6:
		{
			cout << "Unesi konstantu operacije" << endl;
			cin >> cnst;
			op_list.push_back(new InverseDivision(cnst));
		}break;
		case 7:
		{
			cout << "Unesi konstantu operacije" << endl;
			cin >> cnst;
			op_list.push_back(new Power(cnst));
		}break;
		case 8:
		{
			cout << "Unesi konstantu operacije" << endl;
			cin >> cnst;
			op_list.push_back(new Logarithm(cnst));
		}break;
		case 9:
			op_list.push_back(new Abs);
			break;
		case 10:
		{
			cout << "Unesi konstantu operacije" << endl;
			cin >> cnst;
			op_list.push_back(new Min(cnst));
		}break;
		case 11:
		{
			cout << "Unesi konstantu operacije" << endl;
			cin >> cnst;
			op_list.push_back(new Max(cnst));
		}break;
		case 12:
			op_list.push_back(new Invert);
			break;
		case 13:
			op_list.push_back(new Grayscale);
			break;
		case 14:
			op_list.push_back(new BlackAndWhite);
			break;
		case 15:
			op_list.push_back(new Median);
			break;
		case 16:
			{
				string path;
				cin.ignore();
				getline(cin, path);
				op_list.push_back(CompositeOperation::loadCompositeOperationFromFile(path));
			}
			break;
		default:
			break;
		}
	}
	op = new CompositeOperation(name, op_list, "");
	return op;
}

void Operation::passOperationToImage(Slika& img)
{
	Operation* op = Operation::loadOperation();
	img.setOperationThenApply(op);
}

void Operation::saveOperationInFile(const std::string& path, Operation* op)
{
	regex rx("(.*)\\.(...)");
	smatch result;
	regex_match(path, result, rx); //dobro odredjuje format dokumenta samo ako se koristi getline!!!!!
	string extension = result.str(2);
	
	if (extension != "fun")
		throw GNeodgovarajuciTipFajla();

	if (op == nullptr)
		op = Operation::loadOperation();
	
	Operation::helperSaveOperation(op, path);
}

void Operation::helperSaveOperation(Operation* op, const std::string& path)
{
	pugi::xml_document doc;

	pugi::xml_node root = doc.document_element();

	pugi::xml_node rootFunction = doc.append_child("Function");
	rootFunction.append_attribute("name") = op->name.c_str();

	for (Operation* iter_op : op->operation_list)
	{
		if (iter_op->operation_list.size() == 0)
		{
			pugi::xml_node nodeBasicFunction = rootFunction.append_child("BasicFunction");
			nodeBasicFunction.append_attribute("cnst") = iter_op->getConstant();
			nodeBasicFunction.append_child(pugi::node_pcdata).set_value(iter_op->getName().c_str());
		}
		else
		{
			pugi::xml_node nodeCompositeFunction = rootFunction.append_child("CompositeFunction");
			nodeCompositeFunction.append_attribute("name") = iter_op->getName().c_str();

			pugi::xml_node nodePath = nodeCompositeFunction.append_child("Path");
			nodePath.append_child(pugi::node_pcdata).set_value(iter_op->path.c_str());
		}
	}

	doc.save_file(path.c_str(), PUGIXML_TEXT("  "));
}
