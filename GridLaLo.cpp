
#include <math.h>
#include "GridLaLo.h"


GridLaLo::GridLaLo(int i, int j) {
   dI = i;
   dJ = j;
   N = i*j;
}


float GridLaLo::lo(int i, int j) {
	return lof((float)i, (float)j);
}


float GridLaLo::la(int i, int j) {
	return laf((float)i, (float)j);
}


float GridLaLo::lof(float i, float j) {
	float t = i/(float)(dI-1);
	return lo1 + t*(lo2 - lo1);
}


float GridLaLo::laf(float i, float j) {
	float t = j/(float)(dJ-1);
	return la1 + t*(la2 - la1);
}


float GridLaLo::lo(int idx) {
	float t = ii(idx)/(float)(dI-1);
	return la1 + t*(lo2 - lo1);
}


float GridLaLo::la(int idx) {
	float t = ij(idx)/(float)(dJ-1);
	return la1 + t*(la2 - la1);
}


float GridLaLo::gi(float lo, float la) {
	float i = (dI - 1)*(lo - lo1)/(lo2 - lo1);
	return i;
}


float GridLaLo::gj(float lo, float la) {
	float j = (dJ - 1)*(la - la1)/(la2 - la1);
	return j;
}


int GridLaLo::idx(int i, int j)
{
	if (0 <= i && i < dI  && 0 <= j && j < dJ) 
		return j*dI + i;

	return -1;
}
