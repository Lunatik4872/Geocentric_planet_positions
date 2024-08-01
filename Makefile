CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm
OBJ = main.o positionObj.o

all: resultat

resultat: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o resultat

