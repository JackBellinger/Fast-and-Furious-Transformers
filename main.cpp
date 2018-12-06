#include "FFT.h"
#include<stdlib.h>
#include <iostream>
#include <cstring>
#include<stdio.h>
#include<string.h>
#include<cmath>


#include<sndfile.h>
#define BUFFER_LEN 1024
#define MAX_CHANNELS 6

void proccessData(double *data, int count, int channels);
int main(int argc, char** argv)
{

	static double data [BUFFER_LEN]; 
	SNDFILE *infile, *outfile; 
	SF_INFO sfinfo; 
	int readcount; 
	if(argc<2)
		return 1; 
	const char *infilename = argv[1]; 
	const char *outfilename = "output3.wav"; 
	memset(&sfinfo, 0, sizeof(sfinfo)); 
	if(!(infile = sf_open(infilename, SFM_READ, &sfinfo)))
	{
		printf("Not able to open input file %s. \n", infilename); 
		puts (sf_strerror(NULL));
		return 1; 
	};
	if(sfinfo.channels >MAX_CHANNELS)
	{
		printf ("Not able to process more than %d channels\n", MAX_CHANNELS);
		return 1; 
	};
	if(!(outfile = sf_open(outfilename, SFM_WRITE, &sfinfo)))
	{
		printf("Not able to open output file %s. \n", outfilename); 
		puts (sf_strerror(NULL)); 
		return 1; 
	};
	for(int i = 0; i<3; i++)
	{
		if(readcount = sf_read_double(infile,data,BUFFER_LEN))
			sf_write_double(outfile,data,readcount); 
	}
	//while
	if((readcount = sf_read_double (infile, data, BUFFER_LEN)))
	{
		proccessData(data,BUFFER_LEN, sfinfo.channels);	
		sf_write_double(outfile, data, readcount);
	}
	sf_close(infile); 
	sf_close(outfile); 
	
//sound proccessing stuff
//	Options options; 
/*	options.define("c|channel=i:0", "channel to extract (offset from zero)");
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

*/


return 0; 
}

void proccessData(double *data, int size, int channels)
{


	FFT g; //used to accsess FFT functions 

//	unsigned long size=64;//number of samples
	
	float timeStep = .125; //the size of the time step 
	//float timeStep = .125; //the size of the time step 
	float frequency = 1; //the frequency of the original function
        float f_real[size]; 
//	for(int i = 0; i<size; i++)
//		f_real[i]=data[i]; 
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
		time[i]=(i-size/2)*timeStep; 
		if((i-size/2) >-100 && (i-size/2) <100)
			f_real[i]=1;
		else
			f_real[i]=0;
	//	f_real[i]=sin(i*timeStep*frequency);
		f_imag[i]=0;
//		time[i]=i*timeStep; 
//		f_real[i]=sin(i*timeStep*frequency)+rand()%5;
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
       // g.graph(time, f_real, size);
	//do the FFT
	g.graph(time, f_real, size);
//	g.graph(time, f_imag, size);
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
//	std::cout<<a[2*size -1]<<std::endl;
//	std::cout<<std::endl;
//	g.graph(omega, g_mag, size);
//	pass stride 2 to proccess real and imag

//	g.boxFilter(g_real, size, omega[1]); 
//	g.graph(omega, g_real, size);	
//	g.boxFilter(g_imag, size, omega[1]); 

//	g.graph(omega, g_mag, size);
	j = 0; 
	for(int i = 0; i<2*size; i+=2)
	{
		a[i] = g_real[j]; 
		a[i+1]=g_imag[j];
	j++;	
	}
	g.four1(a, size, -1);
	for(int i = 0; i<size; i++)
	{
		f_real[i]=a[i*2];
		f_imag[i]=a[2*i+1];

	}
	float magnitudes[size];
	for(int i = 0; i < size; i++)
	{
		magnitudes[i] = sqrt(pow(g_real[i], 2) + pow(g_imag[i], 2));
	}
//	g.graph(time, g_real, size);	
//	g.graph(time, g_imag, size);	
//	g.graph(omega, magnitudes, size);

	g.graph(time, f_real, size);	
//	g.graph(time, f_imag, size);	
}
