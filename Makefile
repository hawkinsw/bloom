test: main.cpp Makefile
	g++ -std=c++11 -o test main.cpp bloom.h -lpthread
clean:
	rm -f *.o core test
