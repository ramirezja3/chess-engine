CXX = g++
CXXFLAGS = -O2 -std=c++17 -Wall -Wextra -Wno-unknown-pragmas -Wno-reorder

SRC = \
    chess_engine.cpp \
    pieces.cpp \
    position.cpp \
    tables.cpp \
    types.cpp

OBJ = $(SRC:.cpp=.o)
TARGET = chess_engine

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
