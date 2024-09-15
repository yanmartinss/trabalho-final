#ifndef QUADTREE_DECODER_H
#define QUADTREE_DECODER_H

#include <stdio.h>
#include <stdlib.h>

// Estrutura da quadtree
typedef struct QuadtreeNode {
    int nodoFolha;
    unsigned char color;
    struct QuadtreeNode *topLeft, *topRight, *bottomLeft, *bottomRight;
} QuadtreeNode;

// Funções para o decodificador
QuadtreeNode* lerQuadtree(FILE *fp);
void reconstruirImagem(QuadtreeNode *node, unsigned char *pData, int x, int y, int largura, int altura, int imgLargura);
void liberarQuadtree(QuadtreeNode *node);

#endif