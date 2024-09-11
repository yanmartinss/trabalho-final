#include <stdio.h>
#include <stdlib.h>

#define MAX_VAL 255

// Função para escrever uma imagem PGM
void save_pgm(const char* filename, unsigned char** image, int width, int height) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "P2\n%d %d\n%d\n", width, height, MAX_VAL);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(file, "%d ", image[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

// Função de decodificação recursiva usando quadtree
void decode_quadtree(FILE* bitstream, unsigned char** image, int x, int y, int size, int width, int height) {
    int flag;
    if (fscanf(bitstream, "%d", &flag) != 1) {
        fprintf(stderr, "Erro ao ler o bitstream\n");
        exit(EXIT_FAILURE);
    }

    if (flag == 1) {
        int pixel_value;
        if (fscanf(bitstream, "%d", &pixel_value) != 1) {
            fprintf(stderr, "Erro ao ler o valor do pixel\n");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (x + j < width && y + i < height) {
                    image[y + i][x + j] = (unsigned char)pixel_value;
                }
            }
        }
    } else {
        int half = size / 2;
        decode_quadtree(bitstream, image, x, y, half, width, height);               // Superior esquerdo
        decode_quadtree(bitstream, image, x + half, y, half, width, height);         // Superior direito
        decode_quadtree(bitstream, image, x, y + half, half, width, height);         // Inferior esquerdo
        decode_quadtree(bitstream, image, x + half, y + half, half, width, height);  // Inferior direito
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <bitstream_entrada.bin> <imagem_saida.pgm>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* bitstream_file = argv[1];
    const char* output_pgm = argv[2];

    // Supondo que a imagem tenha dimensões 256x256 para o exemplo. Ajuste conforme necessário.
    int width = 256, height = 256;
    unsigned char** image = (unsigned char**)malloc(height * sizeof(unsigned char*));
    for (int i = 0; i < height; i++) {
        image[i] = (unsigned char*)malloc(width * sizeof(unsigned char));
    }

    FILE* bitstream = fopen(bitstream_file, "rb");
    if (bitstream == NULL) {
        perror("Erro ao abrir o arquivo de bitstream");
        return EXIT_FAILURE;
    }

    decode_quadtree(bitstream, image, 0, 0, width, width, height);

    fclose(bitstream);

    save_pgm(output_pgm, image, width, height);

    for (int i = 0; i < height; i++) {
        free(image[i]);
    }
    free(image);

    return EXIT_SUCCESS;
}
