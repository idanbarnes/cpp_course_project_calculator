# Makefile for Simple Calculator course project
# Builds bin/calc from sources in src/

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I./src
LDFLAGS  :=

SRC_DIR  := src
BIN_DIR  := bin

SOURCES  := $(SRC_DIR)/main.cpp \
            $(SRC_DIR)/lexer.cpp \
            $(SRC_DIR)/parser.cpp \
            $(SRC_DIR)/evaluator.cpp \
            $(SRC_DIR)/symbolTable.cpp \
            $(SRC_DIR)/utils.cpp

OBJECTS  := $(SOURCES:.cpp=.o)
TARGET   := $(BIN_DIR)/calc

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(TARGET)

.PHONY: all clean