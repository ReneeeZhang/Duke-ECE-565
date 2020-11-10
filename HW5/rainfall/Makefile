CXXFLAGS := -std=gnu++11 -Wall -Werror -pedantic
CXX 	 := g++

TARGETS := rainfall_seq rainfall_pt
OBJECTS := $(TARGETS:=.o)

.PHONY: all clean
all: $(TARGETS)

$(TARGETS): %: %.o
	$(CXX) $(CXXFLAGS) -o $@ $<

$(TARGETS:=.o): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -f *~ $(TARGETS) $(OBJECTS)