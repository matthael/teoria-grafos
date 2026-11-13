
CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -O2

TARGET = main

OBJS = main.o hamiltoniano.o tps.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c hamiltoniano.h tps.h
	$(CC) $(CFLAGS) -c main.c

hamiltoniano.o: hamiltoniano.c hamiltoniano.h
	$(CC) $(CFLAGS) -c hamiltoniano.c

tps.o: tps.c tps.h hamiltoniano.h
	$(CC) $(CFLAGS) -c tps.c

clean:
	rm -f $(OBJS) $(TARGET) main.exe

.PHONY: all clean
