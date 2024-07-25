build :
	gcc -Wall -o test.o -c positionObj.c
	gcc -o test test.o -lm
	./test

clean:
	rm -f *.o test
