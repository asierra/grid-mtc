/*
 * Code generated by ObjectDomain using CPP Code Generator 2.0
 */
#include <stdio.h>
#include <math.h>
#include "GridNMC4.h"


static float NMCR    =  31.204359052;     /*  Radio al Ecuador */
static float deg2rad  = 0.01745329252;
static float deg10   =  0.1745329252;    /*  Ajuste de 10 Long. */
#define sqr(x)    ((x)*(x))


static short ks[] = {
	28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 
	18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 
	8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 
	8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 
	18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 
	28, 
};


static short fs[] = {
	0, 36, 74, 114, 156, 200, 246, 294, 344, 396, 
	450, 506, 564, 624, 686, 750, 816, 884, 954, 1026, 
	1100, 1176, 1254, 1334, 1416, 1500, 1586, 1674, 1764, 1856, 
	1948, 2040, 2132, 2224, 2316, 2408, 2500, 2592, 2684, 2776, 
	2868, 2960, 3052, 3144, 3236, 3328, 3420, 3512, 3604, 3696, 
	3788, 3880, 3972, 4064, 4156, 4248, 4340, 4432, 4524, 4616, 
	4708, 4800, 4892, 4984, 5076, 5168, 5260, 5352, 5444, 5536, 
	5628, 5720, 5812, 5904, 5994, 6082, 6168, 6252, 6334, 6414, 
	6492, 6568, 6642, 6714, 6784, 6852, 6918, 6982, 7044, 7104, 
	7162, 7218, 7272, 7324, 7374, 7422, 7468, 7512, 7554, 7594, 
	7632, 
};


GridNMC4::GridNMC4() {
   N = 7769;
   dI = 90;
   dJ = 100;
}

float GridNMC4::la(int i, int j) {
   return (90. - 2.*atan2(sqrt(sqr(i-45) + sqr(j-50)),NMCR)/deg2rad);
}

float GridNMC4::lo(int i, int j) {
   return (atan2((double)j-50.,(double)(i - 45))/deg2rad + 10.);
}

float GridNMC4::lof(float i, float j) {
   return (atan2((double)j-50.,(double)(i - 45))/deg2rad + 10.);
}

float GridNMC4::laf(float i, float j) {
   return (90. - 2.*atan2(sqrt(sqr(i-45) + sqr(j-50)),NMCR)/deg2rad);
}

float GridNMC4::gi(float lo, float la) {
   return (45 + NMCR*cos(lo*deg2rad - deg10)*tan(M_PI_4 - la*deg2rad/2.));
}

float GridNMC4::gj(float lo, float la) {
   return (50 + NMCR*sin(lo*deg2rad - deg10)*tan(M_PI_4 - la*deg2rad/2.));
}

int GridNMC4::idx(int i, int j)
{
   if (i < ks[j] || i>(dI - ks[j]) || j<0 || j > dJ) 
     return -1;
   
   return fs[j] + i - ks[j];
}


int GridNMC4::getIJ(int idx, int &i, int &j)
{
	i=-1, j = -1;

	if (0 <= idx && idx < N) {
		j = 0;
		while (fs[j+1] <= idx && j < 50)
			j++;

		i = idx - fs[j] + ks[j];
		
		return 0;
	}

	return -1;
}


int GridNMC4::ii(int idx)
{
	int i, j;

	getIJ(idx, i, j);

	return i;
}


int GridNMC4::ij(int idx)
{
	int i, j;

	getIJ(idx, i, j);

	return j;
}


float GridNMC4::la(int idx)
{
	int i, j;
	
	getIJ(idx, i, j);
	
	return la(i, j);
}


float GridNMC4::lo(int idx)
{
	int i, j;
	
	getIJ(idx, i, j);
	
	return lo(i, j);
}

