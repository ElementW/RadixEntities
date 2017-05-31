.PHONY: all

CPPFLAGS := -g -std=c++14 -Wall -Wextra

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $<

test: Entity.o Property.o Signal.o Method.o test.o
	$(CXX) $(CPPFLAGS) -o $@ $^

all: test

clean:
	rm *.o test
