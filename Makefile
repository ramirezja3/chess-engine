# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# Source files
SRCS = chess_engine.cpp position.cpp tables.cpp types.cpp

# Header dependencies (not compiled, just for reference)
HDRS = position.h tables.h types.h

# Output binary
TARGET = chess_engine

# Build rules
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
