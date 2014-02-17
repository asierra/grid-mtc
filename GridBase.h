#ifndef _GRIDBASE_H
#define _GRIDBASE_H



/*
*  Definition of a grided data set
*/
class GridBase {

protected:

// Grid horizontal dimension
	int dI;

// Grid vertical dimension
	int dJ;

// Number of data
	int N;

public:
	virtual float lo(int i, int j) = 0;

	virtual float la(int i, int j) = 0;
	
	virtual float lof(float i, float j) = 0;

	virtual float laf(float i, float j) = 0;

	virtual float lo(int idx) = 0;

	virtual float la(int idx) = 0;

// Horizontal index 0 <= i <= dI-1
	virtual float gi(float lo, float la) = 0;

	// Vertical index 0 <= j <= dJ-1
	virtual float gj(float lo, float la) = 0;

	// Data index given array positions
	virtual int idx(int i, int j) = 0;

	virtual int ii(int idx) = 0;
	
	virtual int ij(int idx) = 0;
	
	int getDI() {
		return dI;
	}

	int getDJ() {
		return dJ;
	}

	int getN() {
		return N;
	}
};

#endif
