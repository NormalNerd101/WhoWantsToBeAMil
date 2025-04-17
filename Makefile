CXX = g++
CXXFLAGS = -Wall -Iinclude -Iinclude/GUI
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SRC = test.cpp
OUT = test

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
