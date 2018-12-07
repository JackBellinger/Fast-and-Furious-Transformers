#include "FFT.h"
#include<stdlib.h>
#include <iostream>
#include <cstring>
#include<stdio.h>
#include<string.h>
#include<cmath>
#include<vector>

#include "gnuplot-iostream/gnuplot-iostream.h"

#include<sndfile.h>
#define BUFFER_LEN 1024
#define MAX_CHANNELS 6


void proccessData(double *data, float*magData, int count, int channels, FFT fft, filter_t filter, Plot& plot, bool isSin);

int main(int argc, char** argv)
{

	static double data [BUFFER_LEN]; 
	SNDFILE *infile, *outfile, *filteroutfile; 
	SF_INFO sfinfo; 
	int readcount; 
	if(argc<=1)
	{
		std::cout << "error no input file" << std::endl;
		return 1; 
	}
	if(argc<= 2)
	{
		std::cout << "error no graph type" << std::endl;
		return 1;
	}
	if(argc<= 3)
	{
		std::cout << "error no filter type" << std::endl;
		return 1;
	}

	bool isSin = argv[1][0] == 's';
	std::string graphType =  argv[2];

	filter_t filter = NONE;
	std::string filterType = argv[3];
	if(filterType == "REV")
		filter =  REV;
	else if(filterType == "BOX")
		filter = BOX;
	else if(filterType == "COS")
		filter = COS;
	else
		filter = NONE;
	std::string iFname = argv[1];
	std::string oFname = iFname + "output" + filterType + ".wav";
	std::string filteredOutFname = iFname + "output" + filterType + ".wav";
	const char *infilename = iFname.c_str(); 
	const char *outfilename = oFname.c_str(); 
	const char *filteroutfilename = filteredOutFname.c_str();

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
	if(!(outfile = sf_open(filteroutfilename, SFM_WRITE, &sfinfo)))
	{
		printf("Not able to open output file %s. \n", outfilename); 
		puts (sf_strerror(NULL)); 
		return 1; 
	};
//	for(int i = 0; i<3; i++)
//	{
//		if(readcount = sf_read_double(infile,data,BUFFER_LEN))
//			sf_write_double(outfile,data,readcount); 
//	}
	//while
	FFT fft;
	Plot snippitPlot(BUFFER_LEN);
	std::vector<double*> frequencyData;
	std::vector<float*> outputData;
	std::vector<float*> imagData;
	float* filtData = new float[BUFFER_LEN];

	while((readcount = sf_read_double (infile, data, BUFFER_LEN)))
	{
		proccessData(data, filtData, BUFFER_LEN, sfinfo.channels, fft, REV, snippitPlot, isSin);
		//frequencyData.push_back(data);
		
		snippitPlot.gp.clear();
		snippitPlot.graph(graphType);
		
		sf_write_double(outfile, data, readcount);
		//sf_write_float(outfile, snippitPlot.realData, readcount);
		//sf_write_float(filteroutfile, snippitPlot.filterData, readcount);

	}
	sf_close(infile); 
	sf_close(outfile); 
	/*
	   int size = frequencyData.size() * BUFFER_LEN;
	   std::cout << "size: " << size << std::endl;
	   float magnitudes [size];
	   float time[size];
	   int i = 0;	
	   for(int j = 0; j < frequencyData.size(); j++)
	   for(int f = 0; f < BUFFER_LEN && i < size; f++)
	   {
	//std::cout << "magnitude " << i << std::endl;
	//std::cout << "is " << frequencyData[j][f] << std::endl;
	magnitudes[i]=frequencyData[j][f];
	time[i] = i;
	i++;
	}
	float** graphData = {&time, &magnitudes, }
	Plot gpFull(size);
	gpFull.graph("mag");
	*/

	return 0; 
}

void proccessData(double* data, float* filterData, int size, int channels, FFT fft, filter_t filter, Plot& plot, bool isSin)
{



	float timeStep = .125; //the size of the time step 
	float frequency = 10; //the frequency of the original function
	float f_real[size]; 
	for(int i = 0; i<size; i++)
		f_real[i]=data[i]; 
	float f_imag[size]; //imaginary part of original funcion
	float time[size];  //time array hold the "x"(t) component of original function
	float g_real[size]; // g holds function after FFT 
	float g_imag[size]; // imaginary part of g
	float omega[size];  // omega is the frequency (step size of g)
	float a[size*2];  //a holds f stored in interleaf format
	//initalize f to sin function
	for(int i=0; i<size;  i++)
	{
		/*		time[i]=(i-size/2)*timeStep; 
				if((i-size/2) >-100 && (i-size/2) <100)
				f_real[i]=1;
				else
				f_real[i]=0;
				*///	
		if(isSin)
			f_real[i]=sin(i*timeStep*frequency);
		f_imag[i]=0;
		time[i]=i*timeStep; 
		//		f_real[i]=sin(i*timeStep*frequency)+rand()%5;
	}

	//store f in a using interleaf format
	int j = 0; 
	for(int i = 0; i<size*2; i+=2)	
	{
		a[i]=f_real[j]; 
		a[i+1]=f_imag[j];
		j++;
		//	std::cout<<a[i]<<"  "<<a[i+1]<<std::endl;	       
	}
	//graph f with respect to time
	// g.graph(time, f_real, size);
	//do the FFT
	//	fft.graph(time, f_real, size);
	//	fft.graph(time, f_imag, size);
	fft.four1(a, size, 1, plot); 

	//copy a into g 
	j=0; 
	for(int i = 0; i<size; i++)
	{
		g_real[i]=a[i*2];
		g_imag[i]=a[2*i+1];
	}				
	fft.calcOmega(time, size, omega);
	//	fft.graph(omega, g_real, size);	
	//	fft.graph(omega, g_imag, size);	
	//	fft.graph(omega, g_mag, size);
	//	pass stride 2 to proccess real and imag
	if(filter)
	{
		if(filter == REV)
		{
			fft.revFilter(g_real, size, omega[1]); 
			//		g.graph(omega, g_real, size);	
			fft.revFilter(g_imag, size, omega[1]); 
		}
		if(filter == BOX)
		{
			fft.boxFilter(g_real, size, omega[1]);
			fft.boxFilter(g_imag, size, omega[1]); 
		}
		if(filter == COS)
		{
			fft.cosFilter(g_real, size, omega[1]);
			fft.cosFilter(g_imag, size, omega[1]); 
		}
		//	fft.graph(omega, g_mag, size);
		j = 0; 
		for(int i = 0; i<2*size; i+=2)
		{
			a[i] = g_real[j]; 
			a[i+1]=g_imag[j];
			j++;	
		}
	}


	for(int i = 0; i<size; i++)
	{
		data[i]=a[i*2]/size;
		f_imag[i]=a[2*i+1];
	}

	fft.four1(a, size, -1, plot);
	*(plot.imagData) = *f_imag;
	//data = (double*)plot.realData; 
	//	g.graph(time, g_real, size);	
	//	g.graph(time, g_imag, size);	

	//	g.graph(time, f_real, size);	
	//	g.graph(time, f_imag, size);	
}
