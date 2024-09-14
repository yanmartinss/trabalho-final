#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool verificarQuadranteHomogeneo(FILE *pgm_file, int largura, int altura, int start_x, int start_y, int quadrante_largura, int quadrante_altura);
void processarQuadrantes(FILE *pgm_file, int largura, int altura, int start_x, int start_y, int quadrante_largura, int quadrante_altura, FILE *bin_file);
void codificarPGMparaBitstream(const char *input_pgm, const char *output_bin);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <arquivo_pgm> <arquivo_bin>\n", argv[0]);
        return 1;
    }

    const char *input_pgm = argv[1];
    const char *output_bin = argv[2];

    codificarPGMparaBitstream(input_pgm, output_bin);

    return 0;
}

bool verificarQuadranteHomogeneo(FILE *pgm_file, int largura, int altura, int start_x, int start_y, int quadrante_largura, int quadrante_altura) {
    unsigned char pixel, primeiro_pixel;
    long posicao = (start_y * largura) + start_x;
    fseek(pgm_file, posicao, SEEK_SET);
    fread(&primeiro_pixel, sizeof(unsigned char), 1, pgm_file);

    for (int y = 0; y < quadrante_altura; y++) {
        posicao = ((start_y + y) * largura) + start_x;
        fseek(pgm_file, posicao, SEEK_SET);
        for (int x = 0; x < quadrante_largura; x++) {
            fread(&pixel, sizeof(unsigned char), 1, pgm_file);
            if (pixel != primeiro_pixel) return false;
        }
    }
    return true;
}

void processarQuadrantes(FILE *pgm_file, int largura, int altura, int start_x, int start_y, int quadrante_largura, int quadrante_altura, FILE *bin_file) {
    int metade_largura = quadrante_largura / 2, metade_altura = quadrante_altura / 2;
    for (int q = 0; q < 4; q++) {
        int sx = start_x + (q % 2) * metade_largura;
        int sy = start_y + (q / 2) * metade_altura;
        if (verificarQuadranteHomogeneo(pgm_file, largura, altura, sx, sy, metade_largura, metade_altura)) {
            unsigned char pixel;
            fseek(pgm_file, (sy * largura) + sx, SEEK_SET);
            fread(&pixel, sizeof(unsigned char), 1, pgm_file);
            fwrite(&pixel, sizeof(unsigned char), 1, bin_file);
        } else {
            processarQuadrantes(pgm_file, largura, altura, sx, sy, metade_largura, metade_altura, bin_file);
        }
    }
}

void codificarPGMparaBitstream(const char *input_pgm, const char *output_bin) {
    FILE *pgm_file = fopen(input_pgm, "rb");
    FILE *bin_file = fopen(output_bin, "wb");
    char formato[3]; int largura, altura, max_valor;

    fscanf(pgm_file, "%2s %d %d %d", formato, &largura, &altura, &max_valor);
    fgetc(pgm_file); 

    if (verificarQuadranteHomogeneo(pgm_file, largura, altura, 0, 0, largura, altura)) {
        unsigned char pixel;
        fread(&pixel, sizeof(unsigned char), 1, pgm_file);
        fwrite(&pixel, sizeof(unsigned char), 1, bin_file);
    } else {
        processarQuadrantes(pgm_file, largura, altura, 0, 0, largura, altura, bin_file);
    }

    fclose(pgm_file);
    fclose(bin_file);
}