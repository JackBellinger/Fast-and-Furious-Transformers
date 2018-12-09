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

enum filter_t {NONE= 0, REV = 1, BOX = 2, COS = 3};
struct Plot
{
	public:
	Plot(unsigned long size);
	Plot(const Plot&);
	~Plot();
	void setData();
	
	unsigned long size;
	float* timeX;
	float* realData;
	float* imagData;
	float* magData;
	float* filterData;
	Gnuplot gp;

	void graph(std::string);
	void setData(float**);//must have 3 float arrays with size = this.size
};
class FFT
{
	public:
	void calcOmega(float time[], unsigned long size, float omega[]); 
	void swap(float& , float&); 
	void four1(float*, unsigned long, int, Plot&);
	void cosFilter(float[], unsigned long size);
	void revFilter(float[], unsigned long size, float freqStep);
	void boxFilter(float[], unsigned long size, float freqStep);
};
