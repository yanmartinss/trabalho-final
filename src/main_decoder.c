/*********************************************************************************************/
/* Alunos      : Yan Martins de Sousa - Matrícula: 20232045050392                            */
/*               Artur Moreira Martins - Matrícula: 20231045050262                           */
/*               Guilherme Amaro Clarindo - Matrícula: 20231045050050                        */
/*               Vênancio Silva Clarindo - Matrícula: XXXXXXXXXXXXXX                         */
/* Avaliação   : Trabalho Final                                                              */
/* Data        : 04/05/2024 - Professor: Daniel Ferreira                                     */
/* Compilador  : gcc versão 6.3.0                                                            */
/*********************************************************************************************/

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

    int width, height;
    fread(&width, sizeof(int), 1, bin_file);
    fread(&height, sizeof(int), 1, bin_file);

    struct QuadtreeNode *root = readQuadtree(bin_file);
    fclose(bin_file);

    unsigned char *pData = (unsigned char *)malloc(width * height * sizeof(unsigned char));

    reconstructImage(root, pData, 0, 0, width, height, width);

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