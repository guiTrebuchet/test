all: test.exe

test.exe: test.o
	 g++ -std=c++11 -o test.exe main.o

test.o: main.cpp
	 g++ -std=c++11 -c main.cpp
     
clean:
	 rm test.o test.exe
