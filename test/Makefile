
TARGETS := $(patsubst %.cpp,%,$(wildcard *.cpp))

-include Makefile.local

all: $(TARGETS)
$(TARGETS) : % : %.cpp
	$(CXX) $(CXXFLAGS) $(EXTRAFLAGS) -o $@ $^

.PHONY: clean test

clean:
	rm -f $(TARGETS)

test: $(TARGETS)
	for i in $^; do ./$$i || break; done
