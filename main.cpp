#include "FFT.h"
#include <iostream>
#include<cmath>
int main()
{
	FFT g; 
	unsigned long size=64;
	//first row y 
	//second row x
	
	float timeStep = .125; 
	float frequency = 10;
        float f_real[size]; 
	float f_imag[size];
	float time[size]; 
	float g_real[size];
	float g_imag[size];
	float omega[size]; 
	
	float a[size*2]; 
	for(int i=0; i<size;  i++)
	{
		time[i]=i*timeStep; 
		f_real[i]=sin(i*timeStep*frequency);
		f_imag[i]=0;
	}
	int j = 0; 
	for(int i = 0; i<size*2; i+=2)	
	{
		a[i]=f_real[j]; 
		a[i+1]=f_imag[j];
	       j++;	
	}
        g.graph(time, f_real, size);
	g.four1(a, size, 1); 
	j=0; 
	for(int i = 0; i<size; i++)
	{
		g_real[i]=a[i*2];
		g_imag[i]=a[2*i+1];
	}
//	g.graph(real, size);	
//	g.graph(imag, size);	
	return 0; 
}
