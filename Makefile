.PHONY: all

CPPFLAGS := -g -std=c++14 -Wall -Wextra

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $<

test: entity/Entity.o entity/io/Property.o entity/io/Signal.o entity/io/Method.o test.o
	$(CXX) $(CPPFLAGS) -o $@ $^

all: test

clean:
	rm *.o test
