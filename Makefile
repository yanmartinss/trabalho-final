# Definindo o compilador e flags
CC = gcc
CFLAGS = -Wall -Iinclude

# Diretórios onde os executáveis e arquivos objeto serão armazenados
BIN_DIR = bin
BUILD_DIR = build

# Alvo principal para compilar tanto o codificador quanto o decodificador
all: $(BIN_DIR)/main $(BIN_DIR)/main_decoder

# Compilação do codificador
$(BIN_DIR)/main: $(BUILD_DIR)/main.o $(BUILD_DIR)/quadtree.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/main $(BUILD_DIR)/main.o $(BUILD_DIR)/quadtree.o

# Geração do arquivo objeto do codificador
$(BUILD_DIR)/main.o: src/main.c include/quadtree.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/main.c -o $(BUILD_DIR)/main.o

# Geração do arquivo objeto com as funções da quadtree
$(BUILD_DIR)/quadtree.o: src/quadtree.c include/quadtree.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/quadtree.c -o $(BUILD_DIR)/quadtree.o

# Compilação do decodificador
$(BIN_DIR)/main_decoder: $(BUILD_DIR)/main_decoder.o $(BUILD_DIR)/quadtree_decoder.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/main_decoder $(BUILD_DIR)/main_decoder.o $(BUILD_DIR)/quadtree_decoder.o

# Geração do arquivo objeto do decodificador
$(BUILD_DIR)/main_decoder.o: src/main_decoder.c include/quadtree_decoder.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/main_decoder.c -o $(BUILD_DIR)/main_decoder.o

# Geração do arquivo objeto com as funções da quadtree para o decodificador
$(BUILD_DIR)/quadtree_decoder.o: src/quadtree_decoder.c include/quadtree_decoder.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/quadtree_decoder.c -o $(BUILD_DIR)/quadtree_decoder.o

# Criação do diretório build se ele não existir
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Limpeza dos arquivos objeto e dos executáveis gerados
clean:
	rm -f $(BUILD_DIR)/*.o $(BIN_DIR)/main $(BIN_DIR)/main_decoder
