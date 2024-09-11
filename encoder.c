#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VAL 255

// Função para carregar uma imagem PGM
unsigned char** load_pgm(const char* filename, int* width, int* height, int* max_val) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char format[3];
    if (fscanf(file, "%2s", format) != 1 || format[0] != 'P' || format[1] != '2') {
        fprintf(stderr, "Formato PGM não suportado\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Ignorar comentários
    int c;
    while ((c = fgetc(file)) == '#') {
        while (fgetc(file) != '\n');
    }
    ungetc(c, file); // Coloca o caractere lido de volta no stream

    if (fscanf(file, "%d %d", width, height) != 2) {
        fprintf(stderr, "Erro ao ler as dimensões da imagem\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d", max_val) != 1) {
        fprintf(stderr, "Erro ao ler o valor máximo\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    unsigned char** image = (unsigned char**)malloc(*height * sizeof(unsigned char*));
    if (image == NULL) {
        perror("Erro ao alocar memória");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < *height; i++) {
        image[i] = (unsigned char*)malloc(*width * sizeof(unsigned char));
        if (image[i] == NULL) {
            perror("Erro ao alocar memória");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            int pixel;
            if (fscanf(file, "%d", &pixel) != 1) {
                fprintf(stderr, "Erro ao ler o pixel\n");
                fclose(file);
                exit(EXIT_FAILURE);
            }
            image[i][j] = (unsigned char)pixel;
        }
    }

    fclose(file);
    return image;
}


// Verificar se um bloco da imagem é homogêneo
bool is_homogeneous(unsigned char** image, int x, int y, int size, int width, int height) {
    if (x + size > width || y + size > height) return false;

    unsigned char first_pixel = image[y][x];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (image[y + i][x + j] != first_pixel) {
                return false;
            }
        }
    }
    return true;
}

// Função de codificação recursiva usando quadtree
void encode_quadtree(FILE* bitstream, unsigned char** image, int x, int y, int size, int width, int height) {
    if (is_homogeneous(image, x, y, size, width, height)) {
        fprintf(bitstream, "1 %d ", image[y][x]);  // 1 seguido do valor do pixel
    } else {
        fprintf(bitstream, "0 ");  // 0 indica que será subdividido
        int half = size / 2;
        encode_quadtree(bitstream, image, x, y, half, width, height);               // Superior esquerdo
        encode_quadtree(bitstream, image, x + half, y, half, width, height);         // Superior direito
        encode_quadtree(bitstream, image, x, y + half, half, width, height);         // Inferior esquerdo
        encode_quadtree(bitstream, image, x + half, y + half, half, width, height);  // Inferior direito
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <imagem_entrada.pgm> <bitstream_saida.bin>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* input_pgm = argv[1];
    const char* bitstream_file = argv[2];

    int width, height, max_val;
    unsigned char** image = load_pgm(input_pgm, &width, &height, &max_val);

    FILE* bitstream = fopen(bitstream_file, "wb");
    if (bitstream == NULL) {
        perror("Erro ao abrir o arquivo de bitstream");
        return EXIT_FAILURE;
    }

    encode_quadtree(bitstream, image, 0, 0, width, width, height);

    fclose(bitstream);

    for (int i = 0; i < height; i++) {
        free(image[i]);
    }
    free(image);

    return EXIT_SUCCESS;
}
