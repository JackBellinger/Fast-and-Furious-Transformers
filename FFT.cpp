#include <math.h>
#include "FFT.h"
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
