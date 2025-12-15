
# Makefile minimal pour le projet OptiCheminTouristique
# - compile tous les .cpp de `src/` en objets dans `build/`
# - lie l'exécutable `optichemin`

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
DEBUGFLAGS := -g -O0

SRC_DIR := src
OBJ_DIR := build
BIN := optichemin

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

.PHONY: all debug release clean run

all: CXXFLAGS += -O2
all: $(BIN)

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(BIN)

release: CXXFLAGS += -O3
release: $(BIN)

$(BIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR) $(BIN)

run: all
	@./$(BIN)

