.PHONY: all

CPPFLAGS += -std=c++14 -Wall -Wextra -Werror -Wpedantic -Wno-missing-braces -I../include
CPPFLAGS += -g

LDFLAGS += -fPIC -pie
CPPFLAGS += -fPIC

SRCS := ../src/iotypes/ValueType.cpp ../src/Entity.cpp ../src/Property.cpp ../src/Signal.cpp ../src/Method.cpp test.cpp
OBJS := $(SRCS:.cpp=.o)

all: test

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ -MF  ./.depend;

include .depend

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -o $@ -c $<

test: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) test
