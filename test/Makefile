.PHONY: all

CPPFLAGS += -std=c++14 -Wall -Wextra
CPPFLAGS += -g

LDFLAGS += -fPIC -pie
CPPFLAGS += -fPIC -pie

SRCS := ../iotypes/ValueType.cpp ../Entity.cpp ../Property.cpp ../Signal.cpp ../Method.cpp test.cpp
OBJS := $(SRCS:.cpp=.o)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ -MF  ./.depend;

include .depend

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -o $@ -c $<

test: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

all: test

clean:
	rm $(OBJS) test
