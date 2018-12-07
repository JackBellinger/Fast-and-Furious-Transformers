//Imports for graphing  
#include <math.h>
#include "gnuplot-iostream/gnuplot-iostream.h"
//
#include<map>
#include<limits>
#include<cmath>
#include<vector>
#include<cstdio>
//2d
#include<boost/tuple/tuple.hpp>
#include<boost/foreach.hpp>
//3d
#include <boost/array.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>
#include <boost/bind.hpp>

class FFT
{
	public:
	void calcOmega(float time[], unsigned long size, float omega[]); 
	void swap(float& , float&); 
	void four1(float*, unsigned long, int);
	void graph(float[],float[], unsigned long);
	void cosFilter(float[], unsigned long size, float freqStep);
	void revFilter(float[], unsigned long size, float freqStep);
	void boxFilter(float[], unsigned long size, float freqStep);
	Gnuplot gp;
};
