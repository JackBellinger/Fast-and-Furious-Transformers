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
#define BUFFER_LEN 512
#define MAX_CHANNELS 6


void proccessData(double *data, float*magData, int count, int channels, FFT fft, filter_t filter, Plot& plot, bool isSin);

int main(int argc, char** argv)
{

	//two data arrays used for windowing
	static double data [2*BUFFER_LEN]; 
	static double data1 [BUFFER_LEN]; 
	static double data2 [BUFFER_LEN]; 
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

	bool isSin = (argv[1][0] == 's' && argv[1][1] == 'i' && argv[1][3] == 'n');
	std::string graphType =  argv[2];

	filter_t filter = NONE;
	std::string filterType = argv[3];
	if(filterType == "REV")
	{
		filter =  REV;
	}
	else if(filterType == "BOX")
	{
		filter = BOX;
	}
	else if(filterType == "COS")
	{
		filter = COS;
	}
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
	if(!(outfile = sf_open(filteroutfilename, SFM_WRITE, &sfinfo)))
	{
		printf("Not able to open output file %s. \n", outfilename); 
		puts (sf_strerror(NULL)); 
		return 1; 
	};

	//need this loop to maintain formatting at beginning 
	//of a wav file
//	for(int i = 0; i<3; i++)
//	{
//		if(readcount = sf_read_double(infile,data,BUFFER_LEN))
//			sf_write_double(outfile,data,readcount); 
//	}
	//while
	FFT fft;
	Plot snippitPlot(2*BUFFER_LEN);
	std::vector<double*> frequencyData;
	std::vector<float*> outputData;
	std::vector<float*> imagData;
	float* filtData = new float[2*BUFFER_LEN];
	sf_read_double(infile, data1, BUFFER_LEN); 
	int ind=0; 
	while((readcount = sf_read_double (infile, data2, BUFFER_LEN)))
	{
		int dataIndex = 0; 
		//WINDOWING and copy over
		//ramp function = 2*i/length
		//ramp down =  2*(length-i)/length
		for(int i=0; i<BUFFER_LEN/2; i++)
		{
			data[dataIndex]=data1[i] *2*i/BUFFER_LEN; 
			dataIndex++; 
		}
		for(int i = BUFFER_LEN/2; i<BUFFER_LEN; i++)
		{
			data[dataIndex]=data1[i]; 
			dataIndex++; 
		}
		for(int i = 0; i<BUFFER_LEN/2; i++)
		{
			data[dataIndex]=data2[i]; 
			dataIndex++; 
		}	
		for(int i = BUFFER_LEN/2; i<BUFFER_LEN; i++)
		{
			data[dataIndex]=data2[i] *2*(BUFFER_LEN - i)/BUFFER_LEN; 
			dataIndex++; 
		}

		proccessData(data, filtData, 2*BUFFER_LEN, sfinfo.channels, fft, filter, snippitPlot, isSin);

		//frequencyData.push_back(data);
		
		
		
		snippitPlot.gp.clear();
		snippitPlot.graph(graphType);
		//write out the un windowed part of the file	
		sf_write_double(outfile, &data[BUFFER_LEN/2], BUFFER_LEN);
		//sf_write_float(outfile, snippitPlot.realData, readcount);
		//sf_write_float(filteroutfile, snippitPlot.filterData, readcount);
		//
		//copy data 2 into data 1
		for(int i = 0; i<BUFFER_LEN; i++)
			data1[i]=data2[i]; 

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
	Plot gpFull(size);
	fft.four1(magnitudes, size, 1, gpFull);
	gpFull.graph("mag");
*/
	delete []filtData;
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
			fft.cosFilter(g_real, size);
			fft.cosFilter(g_imag, size); 
		}
		//	fft.graph(omega, g_mag, size);
		j = 0; 
		for(int i = 0; i<2*size; i+=2)
		{
			a[i] = g_real[j]; 
			a[i+1]=g_imag[j];
			data[j]=a[i]/size;
			j++;	
		}
	}


	for(int i = 0; i<size; i++)
	{
		plot.imagData[i] = data[i];
		f_imag[i]=a[2*i+1];
	}

	fft.four1(a, size, -1, plot);
	j = 0; 
	//select just the real component
	for(int i = 0; i<size*2; i+=2)
	{
		data[j]=a[i]/size; 
		j++;
	}
	//data = (double*)plot.realData; 
	//	g.graph(time, g_real, size);	
	//	g.graph(time, g_imag, size);	

	//	g.graph(time, f_real, size);	
	//	g.graph(time, f_imag, size);	
}
