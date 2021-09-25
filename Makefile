test: turntable.o
	g++ -c -o turntable.o turntable.cpp
	g++ -o turntable turntable.o
	rm *.o

clean:
	rm -f *.o turntable