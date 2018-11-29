#include <math.h>
#include "FFT.h"


//Imports for graphing  

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

void FFT::graph(float a[][2], unsigned long size)
{
	double min = a[0][0]; 
	double max = a[0][0];
	for(int i = 0; i<size; i++)
	{
		if(min>a[i][1])
			min=a[i][1]; 
		if(max<a[i][1])
			max=a[i][1]; 
	}
		for(int i = 0; i<size; i++)
		{
//			std::cout<<a[i][0]<<std::endl;
//			std::cout<<a[i][1]<<std::endl;
		}

	
	Gnuplot gp;
	float x; 
	float y; 
	std::vector<std::pair<double,double>> xy_pts; 
	for(int i=0; i<size; i++)
	{
		x=a[i][0]; 
		y=a[i][1];
		std::cout<<"("<<x<<", "<<y<<")"<<std::endl;	
		xy_pts.push_back(std::make_pair(x, y));
	}
	gp<<"set xrang [0:"<<size*.125<<"]\nset yrange ["<<min<<":"<<max<<"]\n"; 
	gp<<"plot '-' with lines title 'test'\n";
	gp.send1d(xy_pts); 
	pause_if_needed(); 


}







//End of graphing 

void FFT::swap(float& a, float& b)
{	
	float temp = a; 
	a=b; 
	b=temp; 
}

/*Replaces data [1...2*nn] by its discrete Fourier transform if isign is input as 1
 * or replaces by its inverse Fourier transform if isign is input -1 
 *
 *data complex array of size nn, stored as real array size 2*nn

 This function assumes nn is an integer power of 2 

 //this function is in C copied from numerical recipes in C pg 507
 */
void FFT::four1(float data[], unsigned long nn, int isign)
{
	unsigned long n, mmax, m, j, istep, i; 
	double wtemp, wr, wpr, wpi, wi, theta; 
	float tempr, tempi; 

	n=nn <<1;//n = nn/2 
	j=1; 
	//bit reverse 
	for(i=0; i<n; i+=2)//i declared at top 
	{
		if(j>1)
		{
			swap(data[j], data[i]); 
			swap(data[j+1], data[i+1]); 

		}
		m = n>>1;
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
		theta = isign * (6.28318530717959/mmax);
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
}
