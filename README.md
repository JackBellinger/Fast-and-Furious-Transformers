# Fast-and-Furious-Transformers
FFT implementation for eliminating white noise in .wav files

run {file name} {graph type} {filter}

expects a .wav file
graph types: real - real data, imag - imaginary data, mag - magnitude frequency spectrum

filters: REV, BOX, COS

REV multiplies the FFT by a sin wave to increase the white noise. 
BOX keeps the first and last fourth of the FFT and zeros out the rest, can add some scratchy noises. 
COS multiplies the FFT by two cos waves zeroing out the high frequencies and eliminating white noise. 

Future goals: make the FFT class a namespace instead	
	      add more filters
	      test with wav files that start with more white noise

Boost must be installed. 
libsndfile must be installed (www.mega-nerd.com/libsndfile/)
