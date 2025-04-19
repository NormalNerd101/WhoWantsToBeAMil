CXX = g++
CXXFLAGS = -Wall -Iinclude -Isrc/
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SRC = main.cpp
OUT = main

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
