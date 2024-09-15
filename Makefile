# Definindo o compilador e flags
CC = gcc
CFLAGS = -Wall -Iinclude

# Diretórios
BIN_DIR = bin
BUILD_DIR = build

# Alvo para compilar o codificador e o decodificador
all: $(BIN_DIR)/main $(BIN_DIR)/main_decoder

# Compilação do codificador
$(BIN_DIR)/main: $(BUILD_DIR)/main.o $(BUILD_DIR)/quadtree.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/main $(BUILD_DIR)/main.o $(BUILD_DIR)/quadtree.o

$(BUILD_DIR)/main.o: src/main.c include/quadtree.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/quadtree.o: src/quadtree.c include/quadtree.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/quadtree.c -o $(BUILD_DIR)/quadtree.o

# Compilação do decodificador
$(BIN_DIR)/main_decoder: $(BUILD_DIR)/main_decoder.o $(BUILD_DIR)/quadtree_decoder.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/main_decoder $(BUILD_DIR)/main_decoder.o $(BUILD_DIR)/quadtree_decoder.o

$(BUILD_DIR)/main_decoder.o: src/main_decoder.c include/quadtree_decoder.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/main_decoder.c -o $(BUILD_DIR)/main_decoder.o

$(BUILD_DIR)/quadtree_decoder.o: src/quadtree_decoder.c include/quadtree_decoder.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/quadtree_decoder.c -o $(BUILD_DIR)/quadtree_decoder.o

# Criação do diretório build se não existir
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Limpeza dos arquivos de objeto e executáveis
clean:
	rm -f $(BUILD_DIR)/*.o $(BIN_DIR)/main $(BIN_DIR)/main_decoder
