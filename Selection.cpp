#include "Selection.h"
#include "Slika.h"

void Selection::fillWithColor(int r, int g, int b, vector<Layer*> layers)
{
	for (Rectangle* rec : this->rec_vector)
	{
		for (Layer* l : layers)
		{
			vector<vector<Pixel*>>& pxvec = l->getPixelMatrix();
			auto starth = pxvec.begin() + rec->getUp();
			auto endh = starth + rec->getHeight();
			for_each(starth, endh, [&rec, r, g, b](vector<Pixel*>& v) {
				auto startw = v.begin() + rec->getLeft();
				auto stopw = startw + rec->getWidth();
				for_each(startw, stopw, [r, g, b](Pixel* p) {
					if (p->getRed() != r && p->getGreen() != g && p->getBlue() != b)
						p->setParameters(r, g, b, p->getAlpha());
				});
			});
		}
	}
}
