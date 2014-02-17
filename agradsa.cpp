#include <errno.h>
#include <string.h>
#include "GridList.h"
#include "GridNMC.h"


int main(int argc, char *argv[]) 
{
   char *f, *fout;
   GridNMC *gridbase;
   GridList *list;
   
   gridbase = new GridNMC();
   list = new GridList(gridbase);

   f = argv[1];
   fout = argv[2];

   // First load the data
   fprintf (stderr, "Archivo %s\n", f);
   list->setList(f);
   
   if (list->size()<=0)
     return 1;
   
   fprintf (stderr, "Lista %d\n", list->size());
   
   // Now feed the clist
   FILE *out = fopen(fout, "wb");
   fprintf(stderr, "File %p %s\n", out, strerror(errno));
   for(int i=0; i < list->size(); i++) 
   {
      GridData *d = list->getData(i);
      d->writeData(out);
   }   
   fclose(out);
   
   return 0;
}
