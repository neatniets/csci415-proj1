CFLAGS := -g -std=c++11 -lgmp
CC := gcc $(CFLAGS)
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))

.PHONY: all clean

all: keygen.out

clean:
	-rm $(OBJS) keygen.out

keygen.out: keygen.o rsa-primes.o rsa-exponents.o
	$(CC) -o $@ $^

keygen.o: %.o: %.cpp rsa-primes.hpp rsa-exponents.hpp key-files.hpp
	$(CC) -c -o $@ $<

rsa-primes.o: %.o: %.cpp %.hpp
	$(CC) -c -o $@ $<

rsa-exponents.o: %.o: %.cpp %.hpp
	$(CC) -c -o $@ $<
