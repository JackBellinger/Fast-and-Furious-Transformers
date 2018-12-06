FLAGS=-lutil -lboost_iostreams -lboost_system -lboost_filesystem -fPIC

all: run
run:FFT.o main.o Options.o SoundHeader.o SoundFileRead.o
	g++ $(FLAGS) -g -std=c++11 FFT.o main.o -o run 
FFT.o: FFT.cpp 
	g++ -g -std=c++11 -c FFT.cpp -fPIC
main.o: main.cpp
	g++ -g -std=c++11 -c main.cpp -fPIC
Options.o:
	g++ -g -std=c++11 -c Options.cpp -fPIC
SoundHeader.o:
	g++ -g -std=c++11 -c SoundHeader.cpp -fPIC
SoundFileRead.o:
	g++ -g -std=c++11 -c SoundFileRead.cpp -fPIC
clean: 
	-rm *o run
