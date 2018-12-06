#include "FFT.h"
#include "soundfile-2.2/include/soundfile.h"
#include<stdlib.h>
#include <iostream>
#include<cmath>
int main(int argc, char** argv)
{

//sound proccessing stuff
/*	Options options; 
	options.define("c|channel=i:0", "channel to extract (offset from zero)");
	options.process(argc, argv); 
	int channel = options.getInteger("channel"); 
	const char* inputname = options.getArg(1); 

	SoundFileRead insound(inputname); 
	SoundHeader header = insound; 
	header.setChannels(1); 
	float f_real[insound.getSamples()];

	for(int i = 0; i<insound.getSamples(); i++)
	{
		f_real[i] = insound.getCurrentSampleDouble(channel); 
		insound.incrementSample(); 
	}


*/	FFT g; //used to accsess FFT functions 

	unsigned long size=64;//number of samples
	
	float timeStep = .125; //the size of the time step 
	float frequency = 30; //the frequency of the original function
        float f_real[size]; 
	float f_imag[size]; //imaginary part of original funcion
	float time[size];  //time array hold the "x"(t) component of original function
	float g_real[size]; // g holds function after FFT 
	float g_imag[size]; // imaginary part of g
	float omega[size];  // omega is the frequency (step size of g)
	float a[size*2];  //a holds f stored in interleaf format
	float g_mag[size];//holds maginitues of comples numbers
	//initalize f to sin function
	for(int i=0; i<size;  i++)
	{
		time[i]=i*timeStep; 
		f_real[i]=sin(i*timeStep*frequency)+rand()%5;
		f_imag[i]=0;
	}

	//store f in a using interleaf format
	int j = 0; 
	for(int i = 0; i<size*2; i+=2)	
	{
		a[i]=f_real[j]; 
		a[i+1]=f_imag[j];
	       j++;	
	}
	//graph f with respect to time
        g.graph(time, f_real, size);
	//do the FFT
	g.four1(a, size, 1); 

	//copy a into g 
	j=0; 
	for(int i = 0; i<size; i++)
	{
		g_real[i]=a[i*2];
		g_imag[i]=a[2*i+1];
		g_mag[i]=pow(g_real[i], 2) + pow(g_imag[i],2);
	}				
	g.calcOmega(time, size, omega);
//	g.graph(omega, g_real, size);	
//	g.graph(omega, g_imag, size);	
//	g.graph(omega, g_mag, size);
//	pass stride 2 to proccess real and imag
	g.cosFilter(g_real, size, omega[1]); 
	g.cosFilter(g_imag, size, omega[1]); 
//	g.graph(omega, g_mag, size);
	for(int i = 0; i<2*size; i+=2)
	{
		a[i] = g_real[i]; 
		a[i+1]=g_imag[i]; 	
	}
	g.four1(a, size, -1);
	for(int i = 0; i<size; i++)
	{
		f_real[i]=a[i*2];
		f_imag[i]=a[2*i+1];
	}
	g.graph(time, f_real, size);	
	g.graph(time, f_imag, size);	



	return 0; 
}
