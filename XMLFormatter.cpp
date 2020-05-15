#include "XMLFormatter.h"
#include "pugixml.hpp"
#include <string>
#include "Slika.h"

using namespace std;

const string XMLFormatter::ext = "_layer";
const string XMLFormatter::format = ".svdlayer";

// load 
XMLFormatter::XMLFormatter(const string& path, Slika& img)
{
    pugi::xml_document doc;

    pugi::xml_parse_result res = doc.load_file(path.c_str());
    if (res == false)
        throw GNePostojiFajl();
    
    Slika::deleteInstance();
    img = Slika::getInstance();

    pugi::xml_node rootSlika = doc.first_child();
    
    pugi::xml_node nodeWidth = rootSlika.child("Width");
    img.width = nodeWidth.first_child().text().as_int();

    pugi::xml_node nodeHeight = rootSlika.child("Height");
    img.height =  nodeHeight.first_child().text().as_int();

    pugi::xml_node nodeHasActiveSelection = rootSlika.child("HasActiveSelection");
    img.hasActiveSelection = nodeHasActiveSelection.first_child().text().as_bool();

    img.operation = nullptr;  //ne mora da se cita iz datoteke, jer ce uvek biti nullptr

    // iteriraj kroz Selection Map
    for (auto iter : rootSlika.child("SelectionMap").children())
    {
        bool isActive;
        string selection_name = string(iter.attribute("name").value());
        
        pugi::xml_node nodeIsActiveSelection = iter.child("IsActiveSelection");
        isActive = nodeIsActiveSelection.first_child().text().as_bool();
        
        vector<Rectangle*> rec_vector;
        for (auto iter_rectangle = nodeIsActiveSelection.next_sibling(); iter_rectangle; iter_rectangle = iter_rectangle.next_sibling()) // iteriraj kroz cvorove pravouganika
        {
            int up = iter_rectangle.child("Up").first_child().text().as_int();
            int left = iter_rectangle.child("Left").first_child().text().as_int();
            int height = iter_rectangle.child("Height").first_child().text().as_int();
            int width = iter_rectangle.child("Width").first_child().text().as_int();
            
            rec_vector.push_back(new Rectangle(up, left, height, width));
        }
        Selection* sel = new Selection(rec_vector, isActive);
        img.selection_map.insert(pair<string, Selection*>(selection_name, sel));
    }
    
    //iteriraj kroz niz Layera
    img.layers.clear();
    for (auto iter_layers : rootSlika.child("Layers").children())
    {
        int width = iter_layers.child("Width").first_child().text().as_int();
        int height = iter_layers.child("Height").first_child().text().as_int();
        bool active = iter_layers.child("IsActive").first_child().text().as_bool();
        int opacity = iter_layers.child("Opacity").first_child().text().as_int();
        string path = iter_layers.child("Path").first_child().text().as_string();
        string path_to_saved_layer = iter_layers.child("PixelList").child("PathToSavedLayer").first_child().text().as_string();

        Layer* lay = new Layer(height, width, active, opacity);
        lay->setPath(path);

        ifstream f(path_to_saved_layer, ios::binary);
        if (!f.is_open())
            throw GNePostojiSacuvanaDatotekaSloja();
        
        byte r, g, b, a;
        for(int i = 0; i < lay->height; i++)
            for (int j = 0; j < lay->width; j++)
            {
                f.read((char*)&r, sizeof(byte));
                f.read((char*)&g, sizeof(byte));
                f.read((char*)&b, sizeof(byte));
                f.read((char*)&a, sizeof(byte));
                
                lay->pxvec[i].push_back(new Pixel((int)r, (int)g, (int)b, (int)a));
            }
        img.layers.push_back(lay);
    } 
}

void XMLFormatter::save(const string& path, const Slika& img)
{
	pugi::xml_document doc;

    pugi::xml_node root = doc.document_element(); // dodaj heder

    pugi::xml_node nodeSlika = doc.append_child("Slika");  // u DOM stablo dodaje cvor sa imenom "Slika"

    pugi::xml_node nodeWidth = nodeSlika.append_child("Width");
    nodeWidth.append_child(pugi::node_pcdata).set_value(to_string(img.getWidth()).c_str());  // Cvor sa plain character data je takodje CVOR

    pugi::xml_node nodeHeight = nodeSlika.append_child("Height");
    nodeHeight.append_child(pugi::node_pcdata).set_value(to_string(img.getHeight()).c_str());

    pugi::xml_node nodeHasActiveSelection = nodeSlika.append_child("HasActiveSelection");
    nodeHasActiveSelection.append_child(pugi::node_pcdata).set_value(to_string(img.hasActiveSelection).c_str());

    pugi::xml_node nodeExported = nodeSlika.append_child("Exported");
    nodeExported.append_child(pugi::node_pcdata).set_value(to_string(img.exported).c_str());
   
    pugi::xml_node nodeOperation = nodeSlika.append_child("Operation");
    nodeOperation.append_child(pugi::node_pcdata).set_value("nullptr");

    pugi::xml_node nodeSelectionMap = nodeSlika.append_child("SelectionMap");  // dodavanje slozenijeg cvora - Selection Map

   
    for (const pair<const string, Selection*>& sel_pair : img.selection_map)
    {
        /*pugi::xml_node nodeNameOfTheSelection = nodeSelectionMap.append_child("NameOfTheSelection");
        nodeNameOfTheSelection.append_child(pugi::node_pcdata).set_value(sel_pair.first.c_str());*/

        pugi::xml_node nodeSelection = nodeSelectionMap.append_child("Selection");
        nodeSelection.append_attribute("name") = sel_pair.first.c_str();

        pugi::xml_node nodeIsActiveSelection = nodeSelection.append_child("IsActiveSelection");
        nodeIsActiveSelection.append_child(pugi::node_pcdata).set_value(to_string(sel_pair.second->isActive()).c_str());

        for (Rectangle* rec : sel_pair.second->getRectangleVector())
        {
            pugi::xml_node nodeRectangle = nodeSelection.append_child("Rectangle");

            pugi::xml_node nodeUp = nodeRectangle.append_child("Up");
            nodeUp.append_child(pugi::node_pcdata).set_value(to_string(rec->getUp()).c_str());

            pugi::xml_node nodeLeft = nodeRectangle.append_child("Left");
            nodeLeft.append_child(pugi::node_pcdata).set_value(to_string(rec->getLeft()).c_str());

            pugi::xml_node nodeHeight = nodeRectangle.append_child("Height");
            nodeHeight.append_child(pugi::node_pcdata).set_value(to_string(rec->getHeight()).c_str());

            pugi::xml_node nodeWidth = nodeRectangle.append_child("Width");
            nodeWidth.append_child(pugi::node_pcdata).set_value(to_string(rec->getWidth()).c_str());
        }
    }
    

    pugi::xml_node nodeLayers = nodeSlika.append_child("Layers");
    int i = 0;
    for (Layer* layer : img.layers)
    {
        pugi::xml_node nodeLayer = nodeLayers.append_child("Layer");

        pugi::xml_node nodeWidth = nodeLayer.append_child("Width");
        nodeWidth.append_child(pugi::node_pcdata).set_value(to_string(layer->getWidth()).c_str());

        pugi::xml_node nodeHeight = nodeLayer.append_child("Height");
        nodeHeight.append_child(pugi::node_pcdata).set_value(to_string(layer->getHeight()).c_str());

        pugi::xml_node nodeIsActive = nodeLayer.append_child("IsActive");
        nodeIsActive.append_child(pugi::node_pcdata).set_value(to_string(layer->active).c_str());

        pugi::xml_node nodeOpacity = nodeLayer.append_child("Opacity");
        nodeOpacity.append_child(pugi::node_pcdata).set_value(to_string(layer->opacity).c_str());

        pugi::xml_node nodePath = nodeLayer.append_child("Path");
        nodePath.append_child(pugi::node_pcdata).set_value(layer->path.c_str());

        pugi::xml_node nodePixelList = nodeLayer.append_child("PixelList");

        regex rx("(.*)\\.xml");
        smatch result;
        if (regex_match(path, result, rx) == 0)
            throw GNeodgovarajuciTipFajla();
        string template_path_to_saved_layers = result.str(1);
        template_path_to_saved_layers += XMLFormatter::ext;

        string temp_layer_path = template_path_to_saved_layers += to_string(i++) + XMLFormatter::format;  // cela putanja do sacuvane binarne datoteke piksela sloja
        ofstream f(temp_layer_path, ios::binary);
        for(vector<Pixel*> &v : layer->pxvec)
            for (Pixel* p : v)
            {
                XMLLayerPixel* px = new XMLLayerPixel((byte)p->getRed(), (byte)p->getGreen(), (byte)p->getBlue(), (byte)p->getAlpha());
                f.write((char*)px, sizeof(XMLLayerPixel));
                delete px;
            }
        f.close();
        pugi::xml_node nodePathToSavedLayer = nodePixelList.append_child("PathToSavedLayer");
        nodePathToSavedLayer.append_child(pugi::node_pcdata).set_value(temp_layer_path.c_str());
    }
    bool saveSucceeded = doc.save_file(path.c_str(), PUGIXML_TEXT("  "));
}
