#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_DIFF 10 

typedef struct QuadtreeNode {
    int nodoFolha;
    unsigned char color;
    struct QuadtreeNode *topLeft, *topRight, *bottomLeft, *bottomRight;
} QuadtreeNode;

QuadtreeNode* construirQuadTree(unsigned char *pData, int x, int y, int largura, int altura, int imgLargura);
int homogeneo(unsigned char *pData, int x, int y, int largura, int altura, int imgLargura);
unsigned char media(unsigned char *pData, int x, int y, int largura, int altura, int imgLargura);
void escreverQuadtree(QuadtreeNode *node, FILE *fp);
void freeQuadtree(QuadtreeNode *node);

#endif