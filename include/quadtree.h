#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_DIFF 10

struct QuadtreeNode {
    int isLeaf;
    unsigned char color;
    struct QuadtreeNode *topLeft, *topRight, *bottomLeft, *bottomRight;
};

/* 
 * Função: buildQuadTree
 * ---------------------
 * Constrói a quadtree a partir dos dados da imagem.
 * 
 * Parâmetros:
 *    pData     - ponteiro para os dados da imagem.
 *    x, y      - coordenadas do canto superior esquerdo do bloco.
 *    width     - largura do bloco.
 *    height    - altura do bloco.
 *    imgWidth  - largura total da imagem (usada para indexação).
 * 
 * Retorno:
 *    Retorna um ponteiro para o nó raiz da quadtree.
 */
struct QuadtreeNode* buildQuadTree(unsigned char *pData, int x, int y, int width, int height, int imgWidth);

/* 
 * Função: isHomogeneous
 * ---------------------
 * Verifica se o bloco da imagem é homogêneo.
 * 
 * Parâmetros:
 *    pData     - ponteiro para os dados da imagem.
 *    x, y      - coordenadas do canto superior esquerdo do bloco.
 *    width     - largura do bloco.
 *    height    - altura do bloco.
 *    imgWidth  - largura total da imagem (usada para indexação).
 * 
 * Retorno:
 *    Retorna 1 se o bloco for homogêneo, caso contrário, retorna 0.
 */
int isHomogeneous(unsigned char *pData, int x, int y, int width, int height, int imgWidth);

/* 
 * Função: averageColor
 * --------------------
 * Calcula a cor média de um bloco de imagem.
 * 
 * Parâmetros:
 *    pData     - ponteiro para os dados da imagem.
 *    x, y      - coordenadas do canto superior esquerdo do bloco.
 *    width     - largura do bloco.
 *    height    - altura do bloco.
 *    imgWidth  - largura total da imagem (usada para indexação).
 * 
 * Retorno:
 *    Retorna o valor médio da cor (em tons de cinza) do bloco.
 */
unsigned char averageColor(unsigned char *pData, int x, int y, int width, int height, int imgWidth);

/* 
 * Função: writeQuadtree
 * ---------------------
 * Escreve a quadtree em um arquivo binário.
 * 
 * Parâmetros:
 *    node - ponteiro para o nó da quadtree que será escrito.
 *    fp   - ponteiro para o arquivo binário onde a quadtree será escrita.
 */
void writeQuadtree(struct QuadtreeNode *node, FILE *fp);

/* 
 * Função: freeQuadtree
 * --------------------
 * Libera a memória alocada para a quadtree.
 * 
 * Parâmetros:
 *    node - ponteiro para o nó raiz da quadtree a ser liberada.
 */
void freeQuadtree(struct QuadtreeNode *node);

#endif