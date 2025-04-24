CXX = g++
CXXFLAGS = -Wall -Iinclude -Isrc/
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SRC = main.cpp
OUT = main

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
