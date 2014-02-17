#include <errno.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <netcdfcpp.h>
#include "GridList.h"
#include "GridNMC.h"
#include "GridLaLo.h"
#include "optlist.h"

using namespace std;

static const float NO_LEVEL = -32000;


int ncampo0 = 0, ncampos = 12;

int year0 = 1948, month0 = 1;  // OJO no estamos considerando el día, day0 = 1;


GridList *readNC(string filename, string variable, float level)
{
	int di, dj;
	float la1, la2, lo1, lo2;

	NcFile dataFile(filename.c_str(), NcFile::ReadOnly);

	if (!dataFile.is_valid())
		return NULL;

	if (dataFile.num_dims() < 3) {
		fprintf(stderr, "ERROR: Too few dims\n");
		return NULL;
	}
	
	NcDim *latDim, *lonDim;
	if (!(latDim = dataFile.get_dim("lat")))
    	return NULL;
	if (!(lonDim = dataFile.get_dim("lon")))
		return NULL;
	int NLAT = latDim->size();
	int NLON = lonDim->size();
	printf("Dims %d %d\n", NLAT, NLON);
	
	NcVar *latVar, *lonVar, *levelVar, *dataVar;
	NcAtt *latAtt, *lonAtt, *varAtt;
	 
	if (!(latVar = dataFile.get_var("lat")))
		return NULL;

	if (!(lonVar = dataFile.get_var("lon")))
		return NULL;

	int ilevel;
	if (level > NO_LEVEL) {
		if (!(levelVar = dataFile.get_var("level")))
			return NULL;
		ilevel = levelVar->get_index(&level);
		printf("Level %d\n", ilevel);
	}
	
	if (!(dataVar = dataFile.get_var( variable.c_str() )))
		return NULL;

	
	di = lonVar->num_vals();
	dj = latVar->num_vals();
	latAtt = latVar->get_att("actual_range");
	lonAtt = lonVar->get_att("actual_range");
	lo1 = lonAtt->as_float(0);
	lo2 = lonAtt->as_float(1);
	la1 = latAtt->as_float(0);
	la2 = latAtt->as_float(1);
	
	GridLaLo *lalo = new GridLaLo(di, dj);
	lalo->setLaLo(la1, lo1, la2, lo2);

	GridList *list;
	list = new GridList(lalo);
	
	printf("Dims %d %d %g %g %g %g : %d - %g;    %d\n", 
	   di, dj, lo1, lo2, la1, la2, lalo->getN(), lalo->la(0, 0), lalo->idx(146, 35) );
	
	// Read data in grid
	
	float *fdata = new float[NLAT*NLON];

	float factor_escala = 1.0;
	float add_offset = 0.0;
	
	if ((varAtt = dataVar->get_att("scale_factor")))
		factor_escala = varAtt->as_float(0);
		
	if ((varAtt = dataVar->get_att("add_offset")))
		add_offset = varAtt->as_float(0);
	
	printf("ESCALA %g  OFFSET %g :: %d %d\n", factor_escala, add_offset, ncampo0, ncampos);
	
	for (int k = 0; k < ncampos; k++) {
		double t = (double)(ncampo0 + k);

		if (level > NO_LEVEL)  {
			if (!dataVar->set_cur(t, ilevel, 0, 0))
				return NULL;
		} else 
			if (!dataVar->set_cur(t, 0, 0))
				return NULL;
				
//		printf("Getting data\n"); fflush(stdout);
		if (level > NO_LEVEL) {
			if (!dataVar->get(fdata, 1, 1, NLAT, NLON))
				return NULL;
		} else {
			if (!dataVar->get(fdata, 1, NLAT, NLON))
				return NULL;
		}
		
		GridData *d = new GridData(lalo);
		
//		printf("Copying data\n"); fflush(stdout);
		for (int i=0; i < lalo->getN(); i++) {
			d->setData(i, add_offset + fdata[i]*factor_escala);
		}
		float average, deviation;
		if (la1 < 90.0) 
			d->setDefault(d->getMinRow(0));
//		d->getAverage(average, deviation);
//		printf ("avenc %g %g - %g %g\n", average, deviation, d->getDefault(), la1);
		list->addData(d);
	}

	return list;
}


float level = NO_LEVEL;
string inname, outname, variable;
bool print_binary=false, print_text=false, print_average=false;


const char *helps =
"options:\n"
"	-i : Archivo NetCDF de entrada.\n"
"	-v : Variable a extraer.\n"
"	-l : Nivel a extraer.\n"
"	-f : Fecha del primer campo a extraer (dd/mm/aaaa).\n"
"	-p : Número del primer campo a extraer.\n"
"	-n : Número de campos a extraer.\n"
"	-b : Guardar salida binaria.\n"
"	-a : Imprime promedio y desviación estándar.\n"
"	-t : Guardar salida en texto.\n"
"	-? : Imprime ayuda.\n";

void parse_options(int argc, char *argv[])
{
	option_t *optList=NULL, *thisOpt;

	optList = GetOptList(argc, argv, "i:v:l:n:p:f:bat?");

	if (argc < 3) {
		printf("Usanza: %s <options>\n%s", argv[0], helps);
		exit(1);
	}
		
	while (optList != NULL)
	{
		thisOpt = optList;
		optList = optList->next;
		
		switch (thisOpt->option) {
			case '?':
			printf("Usanza: %s <options>\n\n%s", argv[0], helps);
			FreeOptList(thisOpt);   /* free the rest of the list */
			exit(1);
			break;
			
			case 'i':
			if (thisOpt->argument != NULL) {		
			int len = strlen(thisOpt->argument) - 3;
			inname = thisOpt->argument;
			outname = string(thisOpt->argument, len);
			} else
				fprintf(stderr, "ERROR: Falta argumento en opción %dn", thisOpt->argIndex);
			break;	
			case 'v':
			if (thisOpt->argument != NULL)
				variable = thisOpt->argument;
			else
				fprintf(stderr, "ERROR: Falta argumento en opción %dn", thisOpt->argIndex);
			break;
			case 'l':
			if (thisOpt->argument != NULL)
				level = atof(thisOpt->argument);
			else
				fprintf(stderr, "ERROR: Falta argumento en opción %dn", thisOpt->argIndex);
			break;
			case 'n':
			if (thisOpt->argument != NULL)
				ncampos = atoi(thisOpt->argument);
			else
				fprintf(stderr, "ERROR: Falta argumento en opción %dn", thisOpt->argIndex);
			break;
			case 'b':
			print_binary = true;
			break;
			case 't':
			print_text = true;
			break;
			case 'a':
			print_average = true;
			break;
			case 'p':
			if (thisOpt->argument != NULL)
				ncampo0 = atoi(thisOpt->argument);
			else
				fprintf(stderr, "ERROR: Falta argumento en opción %dn", thisOpt->argIndex);
			break;
			case 'f':
			if (thisOpt->argument != NULL) {
				int d, m, a;
				sscanf (thisOpt->argument,"%d/%d/%d", &d, &m, &a);
				ncampo0 = (a - 1948)*12 + m - 1;
				fprintf(stderr, "Fecha %d %d %d = %d\n", d, m, a, ncampo0);
			}
			else
				fprintf(stderr, "ERROR: Falta argumento en opción %dn\n", thisOpt->argIndex);
			break;	
		}		
		free(thisOpt);    /* done with this item, free it */
	}
	if (level > NO_LEVEL) {
		stringstream slevel;
		slevel << level;
		outname += "-" + slevel.str();
	}
	outname += ".dat";
}


int main(int argc, char *argv[]) 
{
	parse_options( argc, argv);
	
	GridNMC *gridbase;
	GridList *list;

	gridbase = new GridNMC();
	list = new GridList(gridbase);

	fprintf (stderr, "Archivo %s => %s y %s al %g\n", 
		inname.c_str(), outname.c_str(), variable.c_str(), level);

	GridList *listNC = readNC(inname, variable, level);

	if (listNC->size()<=0)
		return 1;

	fprintf (stderr, "Lista %d\n", listNC->size());

	for (int i=0; i < listNC->size(); i++) 
	{
		GridData *dNC = listNC->getData(i);
		GridData *d = new GridData(gridbase);
		d->setData(dNC);
		list->addData(d);		
		if (print_average) { 
			float average, deviation;
			dNC->getAverage(average, deviation);
			printf ("Average %g %g ---- %g\n", average, deviation, dNC->getDefault());
		}
	}   

	if (print_binary)
		list->writeData(outname.c_str());

	if (print_text)	{
		outname += ".txt";
		list->writeDataText(outname.c_str());
	}

	return 0;
}
