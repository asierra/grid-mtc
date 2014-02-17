
#include "GridBase.h"


class GridLaLo : public GridBase {

public:
	GridLaLo(int i, int j);

	float lo(int i, int j);

	float la(int i, int j);
	
	float lof(float i, float j);

	float laf(float i, float j);
	
	float la(int idx);

	float lo(int idx);

	float gi(float lo, float la);

	float gj(float lo, float la);

	int idx(int i, int j);	

	int ii(int idx)  { return idx % dI; }

	int ij(int idx)  { return idx/dI; }

	void setLaLo(float ila1, float ilo1, float ila2, float ilo2) {
		la1 = ila1;
		lo1 = ilo1;
		la2 = ila2;
		lo2 = ilo2;
	}

private:
	float lo1, lo2, la1, la2;
};
