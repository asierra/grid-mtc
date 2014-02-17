#include <math.h>
#include "GridData.h"

static float NULO = 100000;

static int idiota = 0;


GridData::GridData(GridBase* gb) 
{
   gbase = gb;
   data = new float[gbase->getN()];
   min = 1e20;
   max = -1e20;
   default_data = NULO;
}


GridData::~GridData() {
	delete data;
}


float GridData::getData(int idx) {
   if (0 <= idx  &&  idx < gbase->getN()) 
     return data[idx];
   
//   printf("Errorx %d\n", idx);

//   return (default_data!=NULO) ? default_data: NULO;
   return default_data;
}


float *GridData::getData() {
	return data;
}


float GridData::getData(int i, int j) {
   int idx = gbase->idx(i, j);

//   printf("idx %d ", idx);

//	if (idx==-1)
//		printf("Error %d %d\n", i, j);
		
   return getData(idx);
}


float GridData::getDataf(float fi, float fj) {
	int i, j;
	
	i = (int)round(fi);
	j = (int)round(fj);
	
	return getData(i, j);
}


float GridData::getDataLaLo(float la, float lo) {
	float fi, fj;
	
	fi = gbase->gi(lo, la);
	fj = gbase->gj(lo, la);
	
	return getDataf(fi, fj);
}


float GridData::getDataLaLoBilinear(float la, float lo) {
	float fi, fj;
	
	fi = gbase->gi(lo, la);
	fj = gbase->gj(lo, la);
	
	return getDataBilinearf(fi, fj);
}


float GridData::getDataBilinearf(float fi, float fj) {
	int i1, j1, i2, j2;
	float fdi, fdj, f;
	float d11, d12, d22, d21;
	
	i1 = (int)floor(fi);
	j1 = (int)floor(fj);
	
	i2 = (i1 < gbase->getDI()-1) ? i1 + 1: 0;
	j2 = j1 + 1;
	
	fdi = fi - i1;
	fdj = fj - j1;
	
	d11 = getData(i1, j1);
	d12 = getData(i1, j2);
	d21 = getData(i2, j1);
	d22 = getData(i2, j2);
	
	f = d11*(1. - fdi)*(1. - fdj) + d21*fdi*(1. - fdj) + d12*(1. - fdi)*fdj + d22*fdi*fdj;
	
//	if (f > 0.0 && f < NULO)
//		printf("Mierda %g %g %g\n", fi, fj, f);
//	if (idiota)
//		printf("Idiota %g %g; %g %g %g %g = %g\n%d %d %d %d\n", 
//		   fi, fj,  d11, d12, d22, d21, f, i1, j1, i2, j2);
		
	return f;
}


void GridData::setData(int i, int j, float f) {
	int idx = gbase->idx(i, j);
	setData(idx, f);
}


void GridData::setData(int idx, float f) {
	if (idx >=0 && idx < gbase->getN()) {	
		data[idx] = f;
		if (f < min)
			min = f;
		if (f > max)
			max = f;
	}   
}


void GridData::setData(GridData* d)
{	
	for (int i=0;  i < getN(); i++) {
		float la, lo;
		
		la = gbase->la(i);
		lo = gbase->lo(i);	
		if (lo < 0.)
			lo += 360.;	  //  Ojo, posible problema!
			
		idiota = (i==99) ? 1: 0;
 		float f = d->getDataLaLoBilinear(la, lo);
		setData(i, f);
	}
}


int GridData::getN() {
   return gbase->getN();
}


void GridData::writeData(FILE *out)
{
   int di = gbase->getDI();
   int dj = gbase->getDJ();
   int k=0;
   for (int j=0; j <= dj; j++) {	
      for (int i=0; i <= di; i++) {	     
	 float f = getData(i, j);
	 fwrite(&f, sizeof(float), 1, out);
	 k++;
      }
   }
}


void GridData::writeDataText(FILE *out)
{
	int k=0;	
	for (int i=0; i < getN(); i++) {	     
		float f = getData(i);
		if (max-min < 0.01)
			fprintf(out, "%11.5g ", f);
		else
			fprintf(out, "%8.4f ", f);
		k++;
		if (k % 10 == 0)
			fprintf(out, "\n");
	}
	fprintf(out, "\n");
}


float GridData::getMinRow(int j)
{
	double a=1e10;
	for (int i=0; i < gbase->getDI(); i++) {
		float x = getData(i, j);
		if (x < a)
			a = x;
	}  
	return a;
}


float GridData::getAverageRow(int j, float &deviation)
{
	double a=0., d=0.;
	for (int i=0; i < gbase->getDI(); i++) {
		float x = getData(i, j);
		a += x;
		d += x*x;
	}  
	float average = (a/gbase->getN());
	deviation = sqrt(d/gbase->getN() - average*average);
	
	return average;
}

void GridData::getAverage(float &average, float &deviation)
{
	double a=0., d=0.;
	for (int i=0; i < gbase->getN(); i++) {
		float x = data[i];
		a += x;
		d += x*x;
	}  
	average = (a/gbase->getN());
	deviation = sqrt(d/gbase->getN() - average*average);
}

void GridData::setNulo(float nulo)
{
	NULO = nulo;
}
