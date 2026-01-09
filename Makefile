# Nom de l'exécutable
TARGET = OptiChemin

# Compilateur et dossiers
CXX = g++
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Liste des fichiers
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

# Flags de base
CXXFLAGS = -std=c++17 -I$(INC_DIR) -Wall -Wextra -MMD -MP
LDFLAGS = 

# --- Gestion des Modes ---
MODE ?= debug

ifeq ($(MODE), debug)
    # Mode Debug
    CXXFLAGS += -g -O0
else ifeq ($(MODE), release)
    # Mode Release (Optimisations agressives)
    CXXFLAGS += -O3 -march=native -ffast-math -funroll-loops -flto -DNDEBUG
    LDFLAGS += -flto -s
else
    $(error Mode invalide. Utiliser MODE=debug ou MODE=release)
endif

# --- Règles ---

all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	@echo "Liaison de $(TARGET) (Mode: $(MODE))"
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compilation de $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Création dossier obj
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Inclusion des dépendances (.h)
-include $(DEPS)

# Tâche Benchmark (Recompile tout en release et lance)
benchmark:
	@echo ">>> Passage en mode Release et exécution..."
	$(MAKE) re MODE=release
	./$(TARGET)

# Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Rebuild complet
re: clean all

.PHONY: all clean re benchmark