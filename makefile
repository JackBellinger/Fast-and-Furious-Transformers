FLAGS=-lutil -lboost_iostreams -lboost_system -lboost_filesystem -fPIC

all: run
run:FFT.o main.o 
	g++ $(FLAGS) -lsndfile -std=c++11 FFT.o main.o  -o run 
FFT.o: FFT.cpp
	g++ -g -std=c++11 -c FFT.cpp -fPIC
main.o: main.cpp
	g++ -g -std=c++11 -c main.cpp -fPIC
clean: 
	-rm *o run
