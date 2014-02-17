/*
 * Real data in the data set described by GridBase
 */

#include <stdio.h>

#include "GridBase.h"

class GridData {

protected:
	float* data;
	float min, max;
	float default_data;
	GridBase* gbase;

public:

	GridData(GridBase* gbase);
	~GridData();

	virtual float getData(int i, int j);

	virtual float getDataf(float fi, float fj);
	
	float getDataBilinearf(float fi, float fj);
	
	virtual float getDataLaLo(float la, float lo);
	
	virtual float getDataLaLoBilinear(float la, float lo);
	
	virtual float *getData();

	virtual float getData(int idx);
	
	virtual void setData(int i, int j, float f);

	virtual void setData(int idx, float f);

	virtual void setData(GridData*);

	float getMin() { return min; }

	float getMax() { return max; }

	void writeData(FILE *);
	
	void writeDataText(FILE *);
	
	float getMinRow(int row);
	float getAverageRow(int row, float &deviation);
	void getAverage(float &average, float &deviation);
	

	void setNulo(float);
	
	void setDefault(float d)   { default_data = d; }
	
	float getDefault()   { return default_data; }
	
	
	
// Number of data elements
	virtual int getN();
};
