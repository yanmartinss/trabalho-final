#ifndef QUADTREE_DECODER_H
#define QUADTREE_DECODER_H

#include <stdio.h>
#include <stdlib.h>

struct QuadtreeNode {
    int isLeaf;
    unsigned char color; 
    
    struct QuadtreeNode *topLeft, *topRight, *bottomLeft, *bottomRight;
};

/* 
 * Função: readQuadtree
 * -------------------
 * Lê a quadtree a partir de um arquivo binário e constrói a árvore.
 * 
 * Parâmetros:
 *    fp - ponteiro para o arquivo binário de onde a quadtree será lida.
 * 
 * Retorno:
 *    Retorna um ponteiro para o nó raiz da quadtree.
 */
struct QuadtreeNode* readQuadtree(FILE *fp);

/* 
 * Função: reconstructImage
 * -------------------------
 * Reconstrói a imagem a partir da quadtree e preenche a matriz de dados da imagem.
 * 
 * Parâmetros:
 *    node      - ponteiro para o nó atual da quadtree.
 *    pData     - ponteiro para os dados da imagem (onde serão colocados os pixels).
 *    x, y      - coordenadas do canto superior esquerdo do bloco que está sendo preenchido.
 *    width     - largura do bloco.
 *    height    - altura do bloco.
 *    imgWidth  - largura total da imagem (usada para indexação).
 */
void reconstructImage(struct QuadtreeNode *node, unsigned char *pData, int x, int y, int width, int height, int imgWidth);

/* 
 * Função: freeQuadtree
 * -----------------------
 * Libera a memória alocada para a quadtree.
 * 
 * Parâmetros:
 *    node - ponteiro para o nó raiz da quadtree a ser liberada.
 */
void freeQuadtree(struct QuadtreeNode *node);

#endif
