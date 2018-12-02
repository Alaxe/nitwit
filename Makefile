CC=g++
CFLAGS=-std=c++11 -fsanitize=address -Wall -Wextra -Wpedantic

all: run

run: build
	./nitwit

build: *.cpp
	$(CC) -o nitwit $(CFLAGS) *.cpp
