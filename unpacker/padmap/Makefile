# Makefile

SRC = PadMapping.cc Data.cc PadGeom.cc
OBJ = PadMapping.o Data.o PadGeom.o
target = PadMap

CXX = g++
CFLAGS = -Wall

$(target): $(OBJ)
	$(CXX) -o $(target) $^


.cc.o: 
	$(CXX) $(CFLAGS) -c $<

clean::
	rm -f *.o 
	rm PadMap

