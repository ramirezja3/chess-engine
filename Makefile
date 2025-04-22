CXX = g++
CXXFLAGS = -O2 -std=c++17 -Wall -Wextra

SRC = main.cpp \
      pieces.cpp \
      position.cpp \
      tables.cpp \
      types.cpp

OBJ = $(SRC:.cpp=.o)
DEPS = pieces.h position.h tables.h types.h

TARGET = chess_engine

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
