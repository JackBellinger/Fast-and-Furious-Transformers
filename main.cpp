#include "FFT.h"
#include "soundfile-2.2/include/soundfile.h"
#include<stdlib.h>
#include <iostream>
#include<cmath>
int main(int argc, char** argv)
{
//sound proccessing stuff
	Options options; 
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





	FFT g; 
	unsigned long size=64;
	//first row y 
	//second row x
	
	float timeStep = .125; 
	float frequency = 10;
//        float f_real[size]; 
	float f_imag[size];
	float time[size]; 
	float g_real[size];
	float g_imag[size];
	float omega[size]; 
	
	float a[size*2]; 
	for(int i=0; i<size;  i++)
	{
		time[i]=i*timeStep; 
//		f_real[i]=sin(i*timeStep*frequency);
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
