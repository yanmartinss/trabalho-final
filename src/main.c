#include "quadtree.h"

int main(int argc, char *argv[]) {
    if (argc < 3) return printf("Uso: %s <arquivo_pgm> <arquivo_bin>\n", argv[0]), 1;

    FILE *pgm_file = fopen(argv[1], "rb"), *bin_file = fopen(argv[2], "wb");
    if (!pgm_file || !bin_file) return puts("Erro ao abrir o arquivo.\n"), 1;

    int largura, altura, max_valor;
    fscanf(pgm_file, "%*s %d %d %d", &largura, &altura, &max_valor);
    fgetc(pgm_file);

    unsigned char *pData = malloc(largura * altura * sizeof(unsigned char));
    fread(pData, sizeof(unsigned char), largura * altura, pgm_file);
    fclose(pgm_file);

    QuadtreeNode *root = construirQuadTree(pData, 0, 0, largura, altura, largura);
    fwrite(&largura, sizeof(int), 1, bin_file);
    fwrite(&altura, sizeof(int), 1, bin_file);
    escreverQuadtree(root, bin_file);

    free(pData);
    freeQuadtree(root);
    fclose(bin_file);

    printf("Arquivo %s criado com sucesso.\n", argv[2]);
    return 0;
}