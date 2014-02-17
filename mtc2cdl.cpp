#include <errno.h>
#include <string.h>
#include "GridList.h"
#include "GridNMC.h"


int main(int argc, char *argv[]) 
{
	char *f;
	int dpr = 10;
	GridNMC *gridbase;
	GridList *list;
	gridbase = new GridNMC();
	list = new GridList(gridbase);

	f = argv[1];

// First load the data
	fprintf (stderr, "Archivo %s  Datos %p %d\n", f, gridbase, gridbase->getN()); fflush(stderr);
	list->setList(f);

	if (list->size()<=0)
		return 1;

	fprintf (stderr, "Lista %d\n", list->size());
	

	printf ("latitude = ");
	for (int i=0; i < gridbase->getN(); i++) {
		printf("%g", gridbase->la(i));
		if (i < gridbase->getN()-1)
			printf(", ");
		else 
			printf(" ;\n");
		if (i > 0 && i % dpr==0)
			printf("\n");
	}

	printf ("longitude = ");
	for (int i=0; i < gridbase->getN(); i++) {
		printf("%g", gridbase->lo(i));
		if (i < gridbase->getN()-1)
			printf(", ");
		else 
			printf(" ;\n");
		if (i > 0 && i % dpr==0)
			printf("\n");
	}

	for (int j=0; j < list->size(); j++) 
	{
		GridData *d = list->getData(j);
		fprintf(stderr, "Data %d %g %g %p\n", j, d->getMin(), d->getMax(), d->getData());  
	  
		printf ("temperature =\n");
		for (int i=0; i < gridbase->getN(); i++) {
			printf("%g", d->getData(i));
			if (i < gridbase->getN()-1)
				printf(", ");
			else 
				printf(" ;\n");
			if (i > 0 && i % dpr==0)
				printf("\n");
		}
	}	

	return 0;
}

