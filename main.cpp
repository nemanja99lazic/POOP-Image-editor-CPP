#include <iostream>
#include "Menu.h"
#include "exceptions.h"
#include "Slika.h"
using namespace std;
//TREBA DA SE URADI:
// - Nastavi funkciju PAMFormatter::save !!!
// -  Napisi konstruktore za klasu Layer
// - Posle testiranja cuvanja slike, ispisi konstruktore i destruktor za klasu Selection



int main()
{
	Slika& img = Slika::getInstance();
	string img_path;
	/*getline(cin, img_path);
	img.addLayer(img_path);*/
	/*getline(cin, img_path);
	img.addLayer(img_path);*/

	/*img.addLayer("E:\\FAKS\\4. semestar\\POOP\\Projekat C++\\Projekat1\\Projekat1\\Resources\\Examples\\zeleni.bmp", 50);
	img.addLayer("E:\\FAKS\\4. semestar\\POOP\\Projekat C++\\Projekat1\\Projekat1\\Resources\\Examples\\crveni.bmp", 50);
	img.addLayer("E:\\FAKS\\4. semestar\\POOP\\Projekat C++\\Projekat1\\Projekat1\\Resources\\Examples\\Shapes.bmp", 70);
	img.addSelection("Selection1");*/

	/*img.addLayer("E:\\FAKS\\4. semestar\\POOP\\Projekat C++\\Projekat1\\Projekat1\\sredjena3.bmp");
	img.setOperationThenApply(new Median);
	img.save("sredjena4.bmp");*/

	img.addLayer("E:\\FAKS\\4. semestar\\POOP\\Projekat C++\\Projekat1\\Projekat1\\Resources\\Examples\\nela_example.bmp");
	img.save("exported.pam");
	img.deleteLayer(0);
	img.addLayer("E:\\FAKS\\4. semestar\\POOP\\Projekat C++\\Projekat1\\Projekat1\\exported.pam");
	img.addSelection("Selection1");
	Selection &s = img.getSelectionByName("Selection1");
	s.fillWithColor(243, 120, 42, img.getLayersVector());
	img.save("nela_pamtobmp.bmp");
	
	delete &img;
}