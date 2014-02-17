#include "GridBase.h"


class GridNMC : public GridBase {

public:
	GridNMC();

	float lo(int i, int j);

	float la(int i, int j);
	
	float lof(float i, float j);

	float laf(float i, float j);

	float la(int idx);
	
	float lo(int idx);

	float gi(float lo, float la);

	float gj(float lo, float la);

	int idx(int i, int j);
	
	int ii(int idx);
	
	int ij(int idx);
	
	int getIJ(int idx, int &i, int &j);
	
};
