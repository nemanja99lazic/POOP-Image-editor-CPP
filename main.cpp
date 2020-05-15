#include <iostream>
#include "Menu.h"
#include "exceptions.h"
#include "Slika.h"
#include "CompositeOperation.h"
using namespace std;

int main(int argc, char *argv[])
{
	Slika& img = Slika::getInstance();
	string img_path;

	if (argc > 1)
	{
		try
		{
			string image = string(argv[1]);
			string composite_fun = string(argv[2]);
			img.addLayer(image);
			img.setOperationThenApply(CompositeOperation::loadCompositeOperationFromFile(composite_fun));
			img.save(image);
		}
		catch (...)
		{
			return -1;
		}
	}
	else
	{
		int opcija = 0;
		Menu menu;
		while (opcija != 1 && opcija != 2 && opcija != 3)
		{
			try
			{
				cout << endl;
				menu.writeBeforeMain();
				cin >> opcija;
				switch (opcija)
				{
				case 1:
				{
					int height, width;
					cout << endl << "Unesi visinu pa sirinu slike" << endl;
					cout << "Visina:";
					cin >> height;
					cout << "Sirina: " << endl;
					cin >> width;
					img.addEmptyLayer(height, width);
				}break;
				case 2:
				{
					string path;
					cout << endl << "Unesi putanju do 1. sloja slike" << endl;
					cin.ignore();
					getline(cin, path);
					img.addLayer(path);
				}
				break;
				case 3:
				{
					string path;
					cout << endl << "Unesi putanju do sacuvanog stanja slike u XML formatu" << endl;
					cin.ignore();
					getline(cin, path);
					img.loadFromXML(path);
				}
				default:
					continue;
				}
			}
			catch (exception& e)
			{
				cout << "Uneta putanja nije u redu, pokusaj ponovo";
				opcija = 0;
			}
		}
		while (true)
		{
			menu.writeMain();

			cin >> opcija;
			cout << endl;
				
			switch (opcija)
			{
				case 1: // Slojevi
				{
					while (opcija != 0)
					{

						try {
							menu.writeLayerMenu();
							cin >> opcija;
							switch (opcija)
							{
							case 1:
							{
								cout << "Dodavanje sloja" << endl;
								string path;
								cout << endl << "Unesi putanju do sloja slike" << endl;
								cin.ignore();
								getline(cin, path);
								img.addLayer(path);
							} break;
							case 2:
							{
								int redni_broj, vrednost;
								cout << "Unesi redni broj sloja: " << endl;
								cout << "Redni broj: ";
								cin >> redni_broj;
								cout << endl;
								cout << "Unesi vrednost neprozirnosti sloja u granicama[0 - 100]" << endl;
								cout << "Vrednost: ";
								cin >> vrednost;
								cout << endl;
								if (redni_broj > img.getLayersVector().size())
									throw GRedniBrojVanGranica();
								img.getLayersVector()[redni_broj - 1]->setOpacity(vrednost);
							} break;
							case 3:
							{
								int redni_broj;
								cout << "Unesi redni broj sloja: " << endl;
								cout << "Redni broj: ";
								cin >> redni_broj;
								cout << endl;
								if (redni_broj > img.getLayersVector().size())
									throw GRedniBrojVanGranica();
								img.getLayersVector()[redni_broj - 1]->setToActive();
							}break;
							case 4:
							{
								int redni_broj;
								cout << "Unesi redni broj sloja: " << endl;
								cout << "Redni broj: ";
								cin >> redni_broj;
								cout << endl;
								if (redni_broj > img.getLayersVector().size())
									throw GRedniBrojVanGranica();
								img.getLayersVector()[redni_broj - 1]->setToInactive();
							}break;
							case 5:
							{
								int redni_broj;
								cout << "Unesi redni broj sloja za brisanje: " << endl;
								cout << "Redni broj: ";
								cin >> redni_broj;
								cout << endl;
								if (redni_broj > img.getLayersVector().size())
									throw GRedniBrojVanGranica();
								img.deleteLayer(redni_broj);
							}break;
							}
						}
						catch (exception& e) { cout << e.what() << endl; }
					}
					
				} break;
				case 2: // Selekcije
				{
					while (opcija != 0)
					{
						try {
							menu.writeSelectionMenu();
							cin >> opcija;
							switch (opcija)
							{
							case 1:
							{
								string ime_selekcije;
								cout << endl;
								cout << "Unesi ime selekcije" << endl;
								cout << "Ime selekcije: ";
								cin.ignore();
								getline(cin, ime_selekcije);
								cout << endl;
								img.addSelection(ime_selekcije);
							}break;
							case 2:
							{
								string ime_selekcije;
								cout << endl;
								cout << "Unesi ime selekcije" << endl;
								cout << "Ime selekcije: ";
								cin.ignore();
								getline(cin, ime_selekcije);
								cout << endl;
								img.getSelectionByName(ime_selekcije).setToActive();
							} break;
							case 3:
							{
								string ime_selekcije;
								cout << endl;
								cout << "Unesi ime selekcije" << endl;
								cout << "Ime selekcije: ";
								cin.ignore();
								getline(cin, ime_selekcije);
								cout << endl;
								img.getSelectionByName(ime_selekcije).setToInactive();
							} break;
							case 4:
							{
								string ime_selekcije;
								cout << endl;
								cout << "Unesi ime selekcije" << endl;
								cout << "Ime selekcije: ";
								cin.ignore();
								getline(cin, ime_selekcije);
								cout << endl;
								
								img.getSelectionByName(ime_selekcije).deleteSelection();
							} break;
							case 5:
							{
								string ime_selekcije;
								cout << endl;
								cout << "Unesi ime selekcije" << endl;
								cout << "Ime selekcije: ";
								cin.ignore();
								getline(cin, ime_selekcije);
								cout << endl;

								int r, g, b;
								cout << "Unesi boju" << endl;
								cout << "R:";
								cin >> r;
								cout << endl;
								cout << "G:";
								cin >> g;
								cout << endl;
								cout << "B:";
								cin >> b;
								cout << endl;

								img.getSelectionByName(ime_selekcije).fillWithColor(r, g, b, img.getLayersVector());
							}break;
							}
						}
						catch (exception& e) { cout << e.what() << endl; }
					}
				} break; 
				case 3: //Operacije
				{
					while (opcija != 0)
					{
						try {
							menu.writeOperationMenu();
							cin >> opcija;
							switch (opcija)
							{
							case 1:
							{
								img.setOperationThenApply(Operation::loadOperation());
							}break;
							case 2:
							{
								string path;
								cout << endl;
								cout << "Upisi putanju do datoteke sa kompozitnom operacijom:" << endl;
								cin.ignore();
								getline(cin, path);
								cout << endl << "Obrada..." << endl;
								img.setOperationThenApply(CompositeOperation::loadCompositeOperationFromFile(path));
							}break;
							case 3:
							{
								string path;
								cout << endl;
								cout << "Upisi putanju do datoteke u kojoj ce se cuvati funkcija" << endl;
								cin.ignore();
								getline(cin, path);
								cout << endl;

								Operation::saveOperationInFile(path);
							}break;
							}
						}
						catch (exception& e) { cout << e.what() << endl; }
					}
				} break;
				case 4: // Eksportovanje
				{
					try {
						string path;
						menu.writeExportMenu();
						cin.ignore();
						getline(cin, path);
						cout << endl << "Obrada...";
						img.save(path);
					}
					catch (exception& e) { cout << e.what() << endl; }
				} break;
				case 0: // Izlaz
				{
					if (img.isExported())
					{
						cout << "Kraj programa.." << endl;
						return 0;
					}
					else
					{
						char odgovor;
						cout << "Slika nije sacuvana. Vrati se u glavni meni? [Y/N]" << endl;
						cin.ignore();
						cin >> odgovor;
						if (odgovor == 'N')
						{
							cout << "Kraj programa.." << endl;
							return 0;
						}
						else
						{
							cout << "Povratak u glavni meni" << endl;
						}
					}
				} break;
			}
		}
	}
	delete &img;
}