#include "FFT.h"

#include "gnuplot-iostream/gnuplot-iostream.h"

// http://stackoverflow.com/a/1658429
#ifdef _WIN32
        #include <windows.h>
        inline void mysleep(unsigned millis) {
                ::Sleep(millis);
        }
#else
        #include <unistd.h>
        inline void mysleep(unsigned millis) {
                ::usleep(millis * 1000);
        }
#endif

//Utility function for graphing
void pause_if_needed() {
#ifdef _WIN32
        // For Windows, prompt for a keystroke before the Gnuplot object goes out of sco
        // the gnuplot window doesn't get closed.
        std::cout << "Press enter to exit." << std::endl;
        std::cin.get();
#endif
}
Plot::Plot(unsigned long s)
{
	size = s;
	realData = new float[size];
	imagData = new float[size];
	magData = new float[size];
	filterData = new float[size];

	timeX = new float[size];
	for(unsigned long i = 0; i < size; i++)
		timeX[i] = i;

}
Plot::Plot(const Plot& p)
{
	size = p.size;
	timeX = new float[size];
	realData = new float[size];
	magData = new float[size];
	filterData = new float[size];

	for(unsigned long s = 0; s < size; s++)
	{
		timeX[s] = p.timeX[s];
		realData[s] = p.realData[s];
		magData[s] = p.magData[s];
		filterData[s] = p.filterData[s];
	}
}

Plot::~Plot()
{
	delete[] timeX;
	delete[] realData;
	delete[] magData;
	delete[] filterData;
}

void Plot::graph(std::string type)
{//why are these floats?
	if(type !="none")
	{
	float* x;
	float* y;
	if(type == "real")
	{
		x = timeX;
		y = realData;
	}
	if(type == "imag")
	{
		x = timeX;
		y = imagData;
	}
	if(type == "mag")
	{
		x = timeX;
		y = magData;
	}
	if(type == "filter")
	{
		x = timeX;
		y = filterData;
	}

	double xmin = x[0];
	double xmax = x[0];
	double ymin = y[0]; 
	double ymax = y[0];
	for(int i = 0; i<size; i++)
	{
		if(ymin>y[i])
			ymin=y[i]; 
		if(ymax<y[i])
			ymax=y[i]; 
		if(xmin>x[i])
			xmin = x[i]; 
		if(xmax<x[i])
			xmax= x[i];
	}
	
	double plotx; 
	double ploty; 
	std::vector<std::pair<double,double>> xy_pts; 
	for(int i=0; i<size; i++)
	{
		plotx=x[i]; 
		ploty=y[i];
//		std::cout<<"("<<x<<", "<<y<<")"<<std::endl;	
		xy_pts.push_back(std::make_pair(plotx, ploty));
	}
 	
//	std::cout<<xmin<<std::endl;
	gp<<"set xrange ["<<xmin - xmax*.1<<":"<<xmax+xmax*.1<<"]\nset yrange ["<<ymin-ymin*.1<<":"<<ymax+ymax*.1<<"]\n"; 
	gp<<"plot '-' with lines title 'test'\n";
	gp.send1d(xy_pts); 
	pause_if_needed(); 

	}
}

void Plot::setData(float** t_r_idatas)
{
	realData = t_r_idatas[0];	
	imagData = t_r_idatas[0];
	magData = t_r_idatas[0];	
}
//End of graphing


void FFT:: boxFilter(float data[], unsigned long size, float freqStep)
{
	for(int i = size/4; i<size-size/4; i++)
		data[i]=0;

}

	


//takes array of magnitudes of frequency
//orignal: Takes array in frequency domain and multiplies it by cos zeroed at size / 4 and 3/4 size
//now: can be scaled by variable scale, changes zeros to size/scale and size - size/scale
void FFT:: cosFilter(float data[], unsigned long size)
{
	double pi = atan(1) *4.0; 
	float cos1[size/2];
	float cos2[size/2];
	//when index is at size/scale thats where the zero is
	//scale must be at least 2
	int scale = 4; //should be power of 2 //note 
	for(int i = 0; i<size/scale; i++)
		cos1[i]=cos(i * ((scale/2)*pi/size));
       for (int i = 0; i<size/scale; i++)
		cos2[i]=cos(pi/2 - i *((scale/2)*pi/size));
	for(int i = 0; i<size/scale; i++)
	{
		data[i]*=cos1[i]; 
		data[i+(size-(size/scale))]*=cos2[i]; 
	}
	for(int i = size/scale; i<(size-(size/scale)); i++)
		data[i]=0; 


}

void FFT::revFilter(float data[], unsigned long size, float freqStep)
{
//	for(int i = 0; i<size; i++)
//		data[i]=0; 
	float pi = atan(1)*4.0;
        //scale decides how narrow the sin wave is
	//then it is shifted to be centered 	
	//scale should be at least 2
	//midpoint is size/scale
	int scale = 4; 
	float sin1[size]; 
	for(int i = 0; i<2*size/scale; i++)
	{
		sin1[i]= sin(i*scale*pi/(2*size));
	}
	int j = 0; 
	for(int i = size/2 - size/scale; i<size/2 + size/scale; i++)
	{
		data[i]*=sin1[j];
	       j++;	
	}
	for(int i = 0; i<size/2 - size/scale; i++)
		data[i]=0; 
	for (int i = size/2 + size/scale; i<size; i++)
		data[i]=0;
}

void FFT::calcOmega(float time[], unsigned long size, float omega[])
{
	//get delta from time array 
	//delta = time step
	
	float delta = time[1]; 
	//time = sec
	//freq = Hertz
	
	//frequency step
	float freqStep= 1/(size*delta); 

	int i = 0;
	//loop over first half
	for(i = 0; i < size/2; i++)
	{
		omega[i] = freqStep * i;
	}
	//loop over second half
	for(i = size/2; i<size; i++)
	{
		omega[i] = freqStep * (i - size);

	}


}


void FFT::swap(float& a, float& b)
{	
	float temp = a; 
	a=b; 
	b=temp; 
}

/*Replaces data [1...2*nn] by its discrete Fourier transform if isign is input as 1
 * or replaces by its inverse Fourier transform if isign is input -1 
 *
 *ORIGINAL METHOD EXPECTED 1...2nn adjust for offset
 *
 *data complex array of size nn, stored as real array size 2*nn

 This function assumes nn is an integer power of 2 

 //this function is in C copied from numerical recipes in C pg 507
 */
void FFT::four1(float data0[], unsigned long nn, int isign, Plot& p)
{
	//save prefiltered data
	for(int i = 0; i < nn; i+=2)
		p.filterData[i] = data0[i];

	//compute pi
	double pi = atan(1) * 4.0;
	//data 0 has 0 based indexing
	float *data;
	data=data0-1; //pointer arithmetic
//	for(int i = 1; i<=2*nn; i++)
//		std::cout<<*(data+i)<<"  ";
	unsigned long n, mmax, m, j, istep, i; 
	double wtemp, wr, wpr, wpi, wi, theta; 
	float tempr, tempi; 

	n=nn <<1;//n = nn*2 
	j=1; 
	//bit reverse 
	for(i=1; i<n; i+=2)//i declared at top 
	{
		if(j>i)
		{
			swap(data[j], data[i]); 
			swap(data[j+1], data[i+1]); 

		}
		m = nn;
		while(m>=2 && j > m)
		{	
			j-=m; 
			m >>= 1; 
		}
		j+=m; 	
	}

	//Danielson-Lanczos 
	mmax = 2; 
	while(n > mmax)
	{
		istep = mmax <<1; 
		theta = isign * ((pi*2)/mmax);
		wtemp = sin(0.5 * theta); 
		wpr = -2.0 * wtemp * wtemp; 
		wpi = sin(theta); 
		wr = 1.0; 
		wi = 0.0; 
		for(m = 1; m < mmax; m += 2)
		{	
			for(i = m; i <= n; i+= istep)
			{
				j = i + mmax; 
				tempr = wr * data[j] - wi * data[j+1]; 
				tempi = wr * data[j+1] + wi * data[j]; 
				data[j] = data[i] - tempr; 
				data[j+1] = data[i+1] - tempi; 
				data[i] += tempr; 
				data[i+1] += tempi; 
			}
			wr = (wtemp=wr) * wpr - wi * wpi + wr; 
		        wi = wi * wpr + wtemp * wpi + wi; 	
		}
		mmax = istep; 
		
	}

	for(int i = 0; i<nn; i++)
	{
		p.realData[i]=data[i*2]/nn;
		p.imagData[i]=data[2*i+1];
	//	std::cout<<data[2*i]/nn<<"  "<<data[2*i +1]/nn<<std::endl;; 

	}
								//calculate the frequency spectrum graph
  								/*    __________________________		*/
	for(int i = 0; i < p.size; i++)/*  /		  2				  2			*/
	{							/* \/ data[i]  +  imaginary[i]          */
		p.magData[i] =  (float)sqrt( pow(p.realData[i], 2) + pow(p.imagData[i], 2) );
	}
}
