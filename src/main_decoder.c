#include "quadtree_decoder.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <arquivo_bin> <imagem_pgm>\n", argv[0]);
        return 1;
    }

    const char *input_bin = argv[1]; 
    const char *output_pgm = argv[2];

    FILE *bin_file = fopen(input_bin, "rb");
    if (!bin_file) {
        puts("Erro ao abrir o arquivo binário.\n");
        return 1;
    }

    int largura, altura;
    fread(&largura, sizeof(int), 1, bin_file);
    fread(&altura, sizeof(int), 1, bin_file);

    QuadtreeNode *root = lerQuadtree(bin_file);
    fclose(bin_file);

    unsigned char *pData = (unsigned char *)malloc(largura * altura * sizeof(unsigned char));

    reconstruirImagem(root, pData, 0, 0, largura, altura, largura);

    FILE *pgm_file = fopen(output_pgm, "wb");
    if (!pgm_file) {
        puts("Erro ao criar o arquivo PGM.\n");
        free(pData);
        liberarQuadtree(root);
        return 1;
    }

    fprintf(pgm_file, "P5\n%d %d\n255\n", largura, altura);
    fwrite(pData, sizeof(unsigned char), largura * altura, pgm_file);
    fclose(pgm_file);

    free(pData);
    liberarQuadtree(root);

    printf("Imagem %s reconstruída com sucesso.\n", output_pgm);
    return 0;
}