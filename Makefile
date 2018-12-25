CC=g++
CFLAGS=-std=c++11 -fsanitize=address -Wall -Wextra -Wpedantic
OBJECTS = $(patsubst src/%.cpp,obj/%.o,$(wildcard src/*.cpp))

all: sample.c

sample: sample.c
	gcc sample.c -o sample -Wall -Wextra -Wpedantic

run_c: sample
	./sample

sample.c: nitwit sample.ntwt
	@./nitwit

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CC) -o $@ -c $(CFLAGS) $<

nitwit: $(OBJECTS)
	$(CC) -o nitwit $(CFLAGS) $(OBJECTS)

clean:
	rm -rf obj
	rm -f nitwit

.PHONY: clean sample
