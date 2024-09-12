#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool verificarQuadranteHomogeneo(FILE *pgm_file, int largura, int altura, int start_x, int start_y, int quadrante_largura, int quadrante_altura) {
    // Verificar se o quadrante é homogêneo
    unsigned char primeiro_pixel, pixel;
    long posicao = (start_y * largura) + start_x;
    fseek(pgm_file, posicao, SEEK_SET);  // Ajustado aqui, removido ftell()
    fread(&primeiro_pixel, sizeof(unsigned char), 1, pgm_file);

    for (int y = 0; y < quadrante_altura; y++) {
        posicao = ((start_y + y) * largura) + start_x;
        fseek(pgm_file, posicao, SEEK_SET);  // Ajustado aqui, removido ftell()

        for (int x = 0; x < quadrante_largura; x++) {
            fread(&pixel, sizeof(unsigned char), 1, pgm_file);
            if (pixel != primeiro_pixel) {
                return false; // Não é homogêneo
            }
        }
    }
    return true; // Homogêneo
}

void codificarPGMparaBinario(const char *input_pgm, const char *output_bin) {
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

    fscanf(pgm_file, "%2s", formato);
    if (formato[0] != 'P' || formato[1] != '5') {
        printf("Formato PGM inválido. Esperado P5 (binário).\n");
        fclose(pgm_file);
        fclose(bin_file);
        exit(1);
    }

    // Ler a largura, altura e o valor máximo de pixel
    fscanf(pgm_file, "%d %d %d", &largura, &altura, &max_valor);
    fgetc(pgm_file); // Consumir o caractere de nova linha após o cabeçalho

    // Dividir a imagem em 4 quadrantes
    int half_width = largura / 2;
    int half_height = altura / 2;
    bool homogeneo = true;

    for (int quadrante = 0; quadrante < 4; quadrante++) {
        int start_x = (quadrante % 2) * half_width;
        int start_y = (quadrante / 2) * half_height;

        if (!verificarQuadranteHomogeneo(pgm_file, largura, altura, start_x, start_y, half_width, half_height)) {
            homogeneo = false;
            break;
        }
    }

    rewind(pgm_file); // Voltar ao início do arquivo para ler os dados novamente

    if (!homogeneo) {
        printf("A imagem não é homogênea. Codificando normalmente...\n");

        // Processar a imagem inteira
        unsigned char *linha = (unsigned char *)malloc(largura * sizeof(unsigned char));
        if (linha == NULL) {
            printf("Erro de alocação de memória para a linha.\n");
            fclose(pgm_file);
            fclose(bin_file);
            exit(1);
        }

        for (int y = 0; y < altura; y++) {
            fread(linha, sizeof(unsigned char), largura, pgm_file);
            fwrite(linha, sizeof(unsigned char), largura, bin_file);
        }

        free(linha);
    } else {
        printf("A imagem é homogênea. Dividindo em quadrantes...\n");

        unsigned char *linha = (unsigned char *)malloc(half_width * sizeof(unsigned char));
        if (linha == NULL) {
            printf("Erro de alocação de memória para a linha.\n");
            fclose(pgm_file);
            fclose(bin_file);
            exit(1);
        }

        for (int quadrante = 0; quadrante < 4; quadrante++) {
            int start_x = (quadrante % 2) * half_width;
            int start_y = (quadrante / 2) * half_height;

            printf("Codificando quadrante %d...\n", quadrante + 1);

            // Processar cada linha do quadrante
            for (int y = 0; y < half_height; y++) {
                long posicao = ((start_y + y) * largura) + start_x;
                fseek(pgm_file, posicao, SEEK_SET);  // Ajustado aqui, removido ftell()

                fread(linha, sizeof(unsigned char), half_width, pgm_file);
                fwrite(linha, sizeof(unsigned char), half_width, bin_file);
            }
        }

        free(linha);
    }

    fclose(pgm_file);
    fclose(bin_file);

    printf("Arquivo %s criado com sucesso.\n", output_bin);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_pgm>\n", argv[0]);
        return 1;
    }

    const char *input_pgm = argv[1];
    const char *output_bin = "./bitstream/bitstream.bin";

    codificarPGMparaBinario(input_pgm, output_bin);

    return 0;
}
