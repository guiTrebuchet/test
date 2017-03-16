all: test.exe

test.exe: test.o
	 g++ -o test.exe main.o

test.o: main.cpp
	 g++ -c main.cpp
     
clean:
	 rm test.o test.exe
