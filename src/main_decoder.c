#include "quadtree_decoder.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <arquivo_bin> <imagem_pgm>\n", argv[0]);
        return 1;
    }

    // Definição dos caminhos dos arquivos de entrada e saída
    const char *input_bin = argv[1];  // Arquivo binário de entrada
    const char *output_pgm = argv[2]; // Arquivo PGM de saída

    // Abrir o arquivo binário para leitura
    FILE *bin_file = fopen(input_bin, "rb");
    if (!bin_file) {
        puts("Erro ao abrir o arquivo binário.\n");
        return 1;
    }

    // Ler a largura e altura da imagem do arquivo binário
    int width, height;
    fread(&width, sizeof(int), 1, bin_file);
    fread(&height, sizeof(int), 1, bin_file);

    // Ler a quadtree do arquivo binário
    struct QuadtreeNode *root = readQuadtree(bin_file);
    fclose(bin_file);

    // Alocar memória para os dados da imagem
    unsigned char *pData = (unsigned char *)malloc(width * height * sizeof(unsigned char));

    // Reconstruir a imagem a partir da quadtree
    reconstructImage(root, pData, 0, 0, width, height, width);

    // Criar o arquivo PGM para a imagem de saída
    FILE *pgm_file = fopen(output_pgm, "wb");
    if (!pgm_file) {
        puts("Erro ao criar o arquivo PGM.\n");
        free(pData);
        freeQuadtree(root);
        return 1;
    }

    fprintf(pgm_file, "P5\n%d %d\n255\n", width, height);
    fwrite(pData, sizeof(unsigned char), width * height, pgm_file);
    fclose(pgm_file);

    free(pData);
    freeQuadtree(root);

    printf("Imagem %s reconstruída com sucesso.\n", output_pgm);
    return 0;
}