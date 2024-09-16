/*********************************************************************************************/
/* Alunos      : Yan Martins de Sousa - Matrícula: 20232045050392                            */
/*               Artur Moreira Martins - Matrícula: 20231045050262                           */
/*               Guilherme Amaro Clarindo - Matrícula: 20231045050050                        */
/*               Vênancio Silva Clarindo - Matrícula: 20232045050066                         */
/* Avaliação   : Trabalho Final                                                              */
/* Data        : 04/05/2024 - Professor: Daniel Ferreira                                     */
/* Compilador  : gcc versão 6.3.0                                                            */
/*********************************************************************************************/

#include "quadtree.h"

int main(int argc, char *argv[]) { // recebe os argumentos da linha de comando, argc indica o número de argumentos e argv contém os argumentos
    if (argc < 3) return printf("Uso: %s <arquivo_pgm> <arquivo_bin>\n", argv[0]), 1; // aqui o programa verifica se foram passados os parametros certos

    // FILE gerencia arquivos abertos e o ponteiro eh necessario pq fopen retorna um endereço
    FILE *pgm_file = fopen(argv[1], "rb"), *bin_file = fopen(argv[2], "wb"); // abre o argumento 1 do terminal em leitura("readbinary") de binario e escreve("writebinary") o 2 argumento 
    if (!pgm_file || !bin_file) return puts("Erro ao abrir o arquivo.\n"), 1; // erro na escrita dos argumentos

    int width, height, max_value;
    fscanf(pgm_file, "%*s %d %d %d", &width, &height, &max_value); // lê a largura a altura e o valor máximo da imagem e ingora o p5
    fgetc(pgm_file); // consome o caractere de nova linha depois do cabeçalho

    // cada pixel eh 1 byte por isso eh melhor usar unsigned char
    unsigned char *pData = malloc(width * height * sizeof(unsigned char)); // calcula o tanto de memoria para armezanar o tama. da img
    fread(pData, sizeof(unsigned char), width * height, pgm_file); // armazena os pixels no pdata que estao sendo lidos no pgm
    fclose(pgm_file); // fechou o arquivo


    // tem ponteiros para topLeft, topRight, bottomLeft, bottomRight e uma variável color para armazenar a cor media da região, e o root eh o quadrado apontado
    struct QuadtreeNode *root = buildQuadTree(pData, 0, 0, width, height, width); // usa a funçao quadtree do quadtree.c usando os dados de pdata os 0 significam o ponto inicial x e y
    
    // escreve a largura e altura no bitstream gerado
    fwrite(&width, sizeof(int), 1, bin_file);
    fwrite(&height, sizeof(int), 1, bin_file);

    writeQuadtree(root, bin_file); // a funçao escreve no bitstream, passa o root que diz qual é o quadrante e passa o arquivo bitstream

    free(pData);
    freeQuadtree(root);
    
    fclose(bin_file);

    printf("Arquivo %s criado com sucesso.\n", argv[2]);
    return 0;
}

/* Verificação dos parâmetros: O programa garante que dois arquivos (entrada e saída) sejam fornecidos pela linha de comando.

Leitura e processamento da imagem PGM: A imagem PGM é lida e convertida em uma estrutura de dados (pData) que será usada para construir a quadtree.

Construção da quadtree: A função buildQuadTree constrói a estrutura hierárquica que representa a imagem em quadrantes, otimizando áreas homogêneas.

Gravação no arquivo binário: O programa grava as dimensões da imagem e a quadtree no arquivo binário, que será usado pelo decodificador para reconstruir a 
imagem.

Gerenciamento de memória: O código libera corretamente a memória alocada após o uso.*/
