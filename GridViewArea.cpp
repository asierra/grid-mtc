#include "GridViewArea.h"


void GridViewArea::GridViewArea(GridData* d): GridView(d) {
   min = 0;
   max = 1;
   umbral = 0.5;
   scale = 1;
}

void GridViewArea::draw(Graphics* g) 
{
   
}
   

int GridViewArea::getColor(float x) 
{
   int c;
   
   if (scale>0)
     c = (int)(scale*(x-min)/(max-min));
   else
     c = (x > umbral) ? 1: 0;
   
   return c;
}


void GridViewArea::drawSquares(Graphics* g) 
{
   for (int j=0; j < data->getDJ(); j++) {
      for (int i=0; i < data->getDI(); i++) {
	 g->setColor();
	 g->rect();
      }
   }   	
}
