CFLAGS := -g -std=c++11 -lgmp
CC := gcc $(CFLAGS)
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))

.PHONY: all clean

all:

clean:
	-rm $(OBJS)

rsa-primes.o: %.o: %.cpp %.hpp
	$(CC) -c -o $@ $<

rsa-exponents.o: %.o: %.cpp %.hpp
	$(CC) -c -o $@ $<
