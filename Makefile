CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra
LDFLAGS :=

SRC := \
	chess_engine.cpp \
	pieces.cpp \
	position.cpp \
	tables.cpp \
	types.cpp

TEST := test.cpp

OBJ := $(SRC:.cpp=.o)
BIN := chess_engine
TEST_BIN := test_engine

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(SRC) $(TEST)
	$(CXX) $(CXXFLAGS) -o $(TEST_BIN) $(SRC) $(TEST)

clean:
	rm -f $(OBJ) $(BIN) $(TEST_BIN)

.PHONY: all clean test
