
CXXFLAGS = -I$(HOME)/junk/boost -I$(HOME)/junk/pstade -I../.. -Wall -Wextra -Werror -pedantic-errors -std=gnu++0x -g
CXX = g++-trunk
TARGETS := $(patsubst %.cpp,%,$(wildcard *.cpp))

all: $(TARGETS)
$(TARGETS) : % : %.cpp
	$(CXX) $(CXXFLAGS) $(ADDFLAGS) -o $@ $^

.PHONY: clean test

clean:
	rm -f $(TARGETS)

test: $(TARGETS)
	for i in $^; do ./$$i || break; done
