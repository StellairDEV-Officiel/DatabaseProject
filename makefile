# Nom de l'exécutable
TARGET = DatabaseProject

# Dossiers
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include

# Options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -I$(INCLUDE_DIR)

# Liste des fichiers sources
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/command.c $(SRC_DIR)/database.c $(SRC_DIR)/records_btree.c $(SRC_DIR)/table_btree.c $(SRC_DIR)/storage.c

# Fichiers objets générés dans le dossier build
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Règle par défaut, construction de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilation des fichiers .c en fichiers .o dans le dossier build
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Crée le dossier build s'il n'existe pas
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)
