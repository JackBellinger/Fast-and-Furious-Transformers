#include "FFT.h"
#include <iostream>
#include<cmath>
int main()
{
	FFT g; 
	unsigned long size=64;
	//first row y 
	//second row x
        float real[size][2]; 
	float imag[size][2];
	float rVi[size][2];//xi is real y is img	
	float a[size*2]; 
	for(int i=0; i<size;  i++)
	{
		real[i][0]=i*.125; 
		imag[i][0]=i*.125; 
		real[i][1]=sin(i*.125);
		imag[i][1]=0;
	}
	for(int i = 0; i<size*2; i+=2)	
	{
		a[i]=real[i][1]; 
		a[i+1]=imag[i][1]; 
	}
        g.graph(real, size);
//	for(int i = 0; i<size; i++) 
//		std::cout<<real[i][0]<<" ";
	g.four1(a, size, 1); 
	for(int i = 0; i<size; i++)
	{
		real[i][0]=i*.125; 
		imag[i][0]=i*.125; 
		real[i][1]=a[2*i];  	
		imag[i][1]=a[2*i+1];
		std::cout<<a[i]<<std::endl;
	}
	g.graph(real, size);	
	g.graph(imag, size);	
	return 0; 
}
