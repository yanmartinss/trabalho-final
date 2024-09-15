#include "quadtree.h"

int main(int argc, char *argv[]) {
    // Verifica se os parâmetros corretos foram passados
    if (argc < 3) return printf("Uso: %s <arquivo_pgm> <arquivo_bin>\n", argv[0]), 1;

    // Abre os arquivos PGM (entrada) e binário (saída)
    FILE *pgm_file = fopen(argv[1], "rb"), *bin_file = fopen(argv[2], "wb");
    if (!pgm_file || !bin_file) return puts("Erro ao abrir o arquivo.\n"), 1;

    // Lê o cabeçalho do arquivo PGM para obter largura, altura e valor máximo de cor
    int width, height, max_value;
    fscanf(pgm_file, "%*s %d %d %d", &width, &height, &max_value);
    fgetc(pgm_file);  // Consome o caractere de nova linha após o cabeçalho

    // Aloca memória para os dados da imagem
    unsigned char *pData = malloc(width * height * sizeof(unsigned char));
    fread(pData, sizeof(unsigned char), width * height, pgm_file);
    fclose(pgm_file);  // Fecha o arquivo PGM após a leitura

    // Constrói a quadtree a partir dos dados da imagem
    struct QuadtreeNode *root = buildQuadTree(pData, 0, 0, width, height, width);
    
    // Grava a largura e a altura no arquivo binário
    fwrite(&width, sizeof(int), 1, bin_file);
    fwrite(&height, sizeof(int), 1, bin_file);

    // Escreve a quadtree no arquivo binário
    writeQuadtree(root, bin_file);

    // Libera a memória alocada para os dados e para a quadtree
    free(pData);
    freeQuadtree(root);
    fclose(bin_file);  // Fecha o arquivo binário após a escrita

    printf("Arquivo %s criado com sucesso.\n", argv[2]);
    return 0;
}
