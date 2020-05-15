#ifndef _MENU_H_
#define _MENU_H_
#include "exceptions.h"
#include <iostream>
using namespace std;
class Menu
{
public:
	void writeBeforeMain();
	void writeMain();
	void writeLayerMenu();
	void writeSelectionMenu();
	void writeOperationMenu();
	void writeExportMenu();
};

#endif