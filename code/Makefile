TARGET = ../main.exe

# Compilateur C
CC = gcc

CFLAGS = -Wall -Wextra -g -Iinclude

SRC_DIR = src
# Dossier pour les fichiers objets
OBJ_DIR = obj

# Trouve automatiquement tous les fichiers .c dans le dossier src
SRC = $(wildcard $(SRC_DIR)/*.c)

# Génère les noms des fichiers objets en changeant le préfixe de src/ à obj/
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

all: $(TARGET)

# Règle pour créer l'exécutable à partir des fichiers objets
$(TARGET): $(OBJ)
	@echo "Édition des liens pour créer l'exécutable..."
	$(CC) $(OBJ) -o $(TARGET)
	@echo "Exécutable '$(TARGET)' créé avec succès"

# Règle pour compiler les sources .c en objets .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@# Crée le dossier obj/ s'il n'existe pas
	@mkdir -p $(OBJ_DIR)
	@echo "Compilation de $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers compilés
clean:
	@echo "Nettoyage des fichiers de compilation..."
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)
	@echo "Nettoyage terminé."

re: clean all

.PHONY: all clean re