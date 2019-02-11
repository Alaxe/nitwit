SRC = $(wildcard src/**/*.cpp) $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp, obj/%.o, $(SRC) )

CXXFLAGS=-std=c++11 -Wall -Wextra -Wpedantic

all: CXXFLAGS+=-fsanitize=address 
all: CCFLAGS+=-fsanitize=address
all: sample

stable: CXXFLAGS+=-O2
stable: clean nitwit

sample: sample.c
	$(CC) sample.c -o sample $(CCFLAGS)

run_c: sample
	./sample

sample.c: nitwit sample.ntwt
	@./nitwit sample.ntwt sample.c
 
obj/%.o: src/%.cpp
	@mkdir -p $(@D);
	$(CXX) -o $@ -c $(CXXFLAGS) $<

nitwit: $(OBJECTS)
	$(CXX) -o nitwit $(CXXFLAGS) $(OBJECTS)

clean:
	rm -rf obj
	rm -f nitwit
	rm -f sample.c

.PHONY: clean
