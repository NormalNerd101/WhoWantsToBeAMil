CXX = g++
CXXFLAGS = -Wall
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SRC = test.cpp
OUT = test

all:
	$(CXX) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
