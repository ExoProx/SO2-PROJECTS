CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall

TARGET = philosophers
SRC = philosophers.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
