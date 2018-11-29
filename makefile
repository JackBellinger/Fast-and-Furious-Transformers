FLAGS=-lutil -lboost_iostreams -lboost_system -lboost_filesystem

all: run
run:FFT.o main.o
	g++ $(FLAGS) -std=c++11 FFT.o main.o -o run 
FFT.o: FFT.cpp
	g++ -g -std=c++11 -c FFT.cpp
main.o: main.cpp
	g++ -g -std=c++11 -c main.cpp
clean: 
	-rm *o run
