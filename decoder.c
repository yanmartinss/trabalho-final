#include <stdio.h>
#include <stdlib.h>

void decodificarBinarioParaPGM(const char *input_bin, const char *output_pgm, int largura, int altura, int max_valor) {
    FILE *bin_file = fopen(input_bin, "rb");
    if (bin_file == NULL) {
        printf("Erro ao abrir o arquivo binário.\n");
        exit(1);
    }

    FILE *pgm_file = fopen(output_pgm, "wb");
    if (pgm_file == NULL) {
        printf("Erro ao criar a imagem PGM.\n");
        fclose(bin_file);
        exit(1);
    }

    // Escrever o cabeçalho da imagem PGM
    fprintf(pgm_file, "P5\n%d %d\n%d\n", largura, altura, max_valor);

    // Ler o bitstream e reconstruir a imagem
    unsigned char *linha = (unsigned char *)malloc(largura * sizeof(unsigned char));
    if (linha == NULL) {
        printf("Erro de alocação de memória para a linha.\n");
        fclose(bin_file);
        fclose(pgm_file);
        exit(1);
    }

    for (int y = 0; y < altura; y++) {
        fread(linha, sizeof(unsigned char), largura, bin_file);
        fwrite(linha, sizeof(unsigned char), largura, pgm_file);
    }

    free(linha);
    fclose(bin_file);
    fclose(pgm_file);

    printf("Imagem PGM %s reconstruída com sucesso.\n", output_pgm);
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Uso: %s <output_pgm> <largura> <altura> <max_valor>\n", argv[0]);
        return 1;
    }

    const char *input_bin = "./bitstream/bitstream.bin";
    const char *output_pgm = argv[1];
    int largura = atoi(argv[2]);
    int altura = atoi(argv[3]);
    int max_valor = atoi(argv[4]);

    decodificarBinarioParaPGM(input_bin, output_pgm, largura, altura, max_valor);

    return 0;
}