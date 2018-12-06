

class FFT
{
	public:
	void calcOmega(float time[], unsigned long size, float omega[]); 
	void swap(float& , float&); 
	void four1(float*, unsigned long, int);
	void graph(float[],float[], unsigned long);
	void cosFilter(float[], unsigned long size, float freqStep);
	void revFilter(float[], unsigned long size, float freqStep);
	void boxFilter(float[], unsigned long size, float freqStep);
};
