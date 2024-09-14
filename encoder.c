#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Declaração das funções
bool verificarQuadranteHomogeneo(FILE *pgm_file, int largura, int altura, int start_x, int start_y, int quadrante_largura, int quadrante_altura);
void codificarPGMparaBitstream(const char *input_pgm, const char *output_bin);
void dividirEProcessarQuadrantes(FILE *pgm_file, int largura, int altura, int start_x, int start_y, int quadrante_largura, int quadrante_altura, FILE *bin_file);
void ignorarComentarios(FILE *pgm_file);

// Função principal
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_pgm>\n", argv[0]);
        return 1;
    }

    const char *input_pgm = argv[1];
    const char *output_bin = "./bitstream/bitstream.bin"; // Arquivo de saída

    codificarPGMparaBitstream(input_pgm, output_bin);

    return 0;
}

// Função para ignorar comentários no arquivo PGM
void ignorarComentarios(FILE *pgm_file) {
    int ch;
    while ((ch = fgetc(pgm_file)) == '#') {  // Enquanto for comentário
        while (fgetc(pgm_file) != '\n');     // Ignora o resto da linha
    }
    ungetc(ch, pgm_file);  // Devolve o último caractere lido que não era '#'
}

// Verifica se o quadrante é homogêneo
bool verificarQuadranteHomogeneo(FILE *pgm_file, int largura, int altura, int start_x, int start_y, int quadrante_largura, int quadrante_altura) {
    unsigned char primeiro_pixel, pixel;
    long posicao = (start_y * largura) + start_x;
    fseek(pgm_file, posicao + sizeof(unsigned char), SEEK_CUR);
    fread(&primeiro_pixel, sizeof(unsigned char), 1, pgm_file);

    for (int y = 0; y < quadrante_altura; y++) {
        posicao = ((start_y + y) * largura) + start_x;
        fseek(pgm_file, posicao + sizeof(unsigned char), SEEK_CUR);

        for (int x = 0; x < quadrante_largura; x++) {
            fread(&pixel, sizeof(unsigned char), 1, pgm_file);
            if (pixel != primeiro_pixel) {
                return false; // Não é homogêneo
            }
        }
    }
    return true; // Homogêneo
}

// Função para codificar a imagem PGM para bitstream
void codificarPGMparaBitstream(const char *input_pgm, const char *output_bin) {
    FILE *pgm_file = fopen(input_pgm, "rb");
    if (pgm_file == NULL) {
        printf("Erro ao abrir a imagem PGM.\n");
        exit(1);
    }

    FILE *bin_file = fopen(output_bin, "wb");
    if (bin_file == NULL) {
        printf("Erro ao criar o arquivo binário.\n");
        fclose(pgm_file);
        exit(1);
    }

    // Ler o cabeçalho do PGM
    char formato[3];
    int largura, altura, max_valor;

    fscanf(pgm_file, "%2s", formato);  // Ler o formato (P5)
    ignorarComentarios(pgm_file);      // Ignorar possíveis comentários

    if (formato[0] != 'P' || formato[1] != '5') {
        printf("Formato PGM inválido. Esperado P5 (binário).\n");
        fclose(pgm_file);
        fclose(bin_file);
        exit(1);
    }

    // Ler a largura, altura e o valor máximo de pixel
    fscanf(pgm_file, "%d %d", &largura, &altura);
    ignorarComentarios(pgm_file);      // Ignorar possíveis comentários
    fscanf(pgm_file, "%d", &max_valor);
    fgetc(pgm_file); // Consumir o caractere de nova linha após o cabeçalho

    if (max_valor > 255) {
        printf("O programa suporta apenas imagens com valores de pixel entre 0 e 255.\n");
        fclose(pgm_file);
        fclose(bin_file);
        exit(1);
    }

    // Verificar se a imagem inteira é homogênea
    if (verificarQuadranteHomogeneo(pgm_file, largura, altura, 0, 0, largura, altura)) {
        printf("A imagem inteira é homogênea. Salvando como bitstream...\n");
        unsigned char pixel;
        fread(&pixel, sizeof(unsigned char), 1, pgm_file);
        fwrite(&pixel, sizeof(unsigned char), 1, bin_file);
    } else {
        // Caso contrário, dividir em quadrantes
        printf("Dividindo a imagem em quadrantes...\n");
        dividirEProcessarQuadrantes(pgm_file, largura, altura, 0, 0, largura, altura, bin_file);
    }

    fclose(pgm_file);
    fclose(bin_file);

    printf("Arquivo %s criado com sucesso.\n", output_bin);
}

// Função para dividir e processar os quadrantes
void dividirEProcessarQuadrantes(FILE *pgm_file, int largura, int altura, int start_x, int start_y, int quadrante_largura, int quadrante_altura, FILE *bin_file) {
    int half_width = quadrante_largura / 2;
    int half_height = quadrante_altura / 2;

    for (int quadrante = 0; quadrante < 4; quadrante++) {
        int sub_start_x = start_x + (quadrante % 2) * half_width;
        int sub_start_y = start_y + (quadrante / 2) * half_height;

        if (verificarQuadranteHomogeneo(pgm_file, largura, altura, sub_start_x, sub_start_y, half_width, half_height)) {
            printf("Quadrante %d é homogêneo. Salvando no bitstream...\n", quadrante + 1);
            unsigned char pixel;
            long posicao = (sub_start_y * largura) + sub_start_x;
            fseek(pgm_file, posicao + sizeof(unsigned char), SEEK_CUR);
            fread(&pixel, sizeof(unsigned char), 1, pgm_file);
            fwrite(&pixel, sizeof(unsigned char), 1, bin_file);
        } else {
            printf("Quadrante %d não é homogêneo. Dividindo...\n", quadrante + 1);
            dividirEProcessarQuadrantes(pgm_file, largura, altura, sub_start_x, sub_start_y, half_width, half_height, bin_file);
        }
    }
}
