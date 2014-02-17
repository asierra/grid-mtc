#include <errno.h>
#include <string.h>
#include <netcdfcpp.h>
using namespace std;
#include "GridList.h"
#include "GridNMC.h"

static const int NLAT=1977;
static const int NLON=1977;
static const int NLVL=2;

static const int NC_ERR = 2;

int main(int argc, char *argv[]) 
{
	char *f;
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


	float lats[NLAT],lons[NLON];
	for (int i=0; i < gridbase->getN(); i++) {
		lats[i] = gridbase->la(i);
		lons[i] = gridbase->lo(i);
	}

	NcError err(NcError::verbose_nonfatal);
	NcFile dataFile("data.nc", NcFile::Replace);
	if (!dataFile.is_valid())
		return NC_ERR;

	NcDim *lvlDim, *latDim, *lonDim, *recDim;
	if (!(lvlDim = dataFile.add_dim("level", NLVL)))
		return NC_ERR;
	if (!(latDim = dataFile.add_dim("latitude", NLAT)))
		return NC_ERR;
	if (!(lonDim = dataFile.add_dim("longitude", NLON)))
		return NC_ERR;
// Add an unlimited dimension...
	if (!(recDim = dataFile.add_dim("time")))
		return NC_ERR;

	NcVar *latVar, *lonVar;
	if (!(latVar = dataFile.add_var("latitude", ncFloat, latDim)))
		return NC_ERR;
	if (!(lonVar = dataFile.add_var("longitude", ncFloat, lonDim)))
		return NC_ERR;
	if (!latVar->add_att("units", "degrees_north"))
		return NC_ERR;
	if (!lonVar->add_att("units", "degrees_east"))
		return NC_ERR;

	NcVar *tempVar;
	if (!(tempVar = dataFile.add_var("temperature", ncFloat, recDim, 
		lvlDim, latDim, lonDim)))
		return NC_ERR;

	if (!tempVar->add_att("units", "celsius"))
		return NC_ERR;

	if (!latVar->put(lats, NLAT))
		return NC_ERR;       
	if (!lonVar->put(lons, NLON))
		return NC_ERR;

	for (int i=0; i < list->size(); i++) 
	{
		GridData *d = list->getData(i);
		printf("NC Data %g %g %d %p\n", d->getMin(), d->getMax(), i, d->getData());  fflush(stdout);
		if (!tempVar->put_rec(d->getData(), i))
			return NC_ERR;       
	}	

	return 0;
}

