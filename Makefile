test: bloom.o bloom.cpp main.cpp
	g++ -std=c++11 -o test main.cpp bloom.h bloom.o -lpthread
bloom.o: bloom.h bloom.cpp
	g++ -std=c++11 -c bloom.cpp -o bloom.o
clean:
	rm -f *.o core test
