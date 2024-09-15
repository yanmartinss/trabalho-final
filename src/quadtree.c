/*********************************************************************************************/
/* Alunos      : Yan Martins de Sousa - Matrícula: 20232045050392                            */
/*               Artur Moreira Martins - Matrícula: 20231045050262                           */
/*               Guilherme Amaro Clarindo - Matrícula: 20231045050050                        */
/*               Vênancio Silva Clarindo - Matrícula: XXXXXXXXXXXXXX                         */
/* Avaliação   : Trabalho Final                                                              */
/* Data        : 04/05/2024 - Professor: Daniel Ferreira                                     */
/* Compilador  : gcc versão 6.3.0                                                            */
/*********************************************************************************************/

#include "quadtree.h"

struct QuadtreeNode* buildQuadTree(unsigned char *pData, int x, int y, int width, int height, int imgWidth) {
    struct QuadtreeNode *node = malloc(sizeof(struct QuadtreeNode));
    if (isHomogeneous(pData, x, y, width, height, imgWidth)) {
        node->isLeaf = 1;
        node->color = averageColor(pData, x, y, width, height, imgWidth);
        node->topLeft = node->topRight = node->bottomLeft = node->bottomRight = NULL;

    } else {
        int halfWidth = width / 2, halfHeight = height / 2;
        node->isLeaf = 0;
        node->topLeft = buildQuadTree(pData, x, y, halfWidth, halfHeight, imgWidth);
        node->topRight = buildQuadTree(pData, x, y + halfWidth, halfWidth, halfHeight, imgWidth);
        node->bottomLeft = buildQuadTree(pData, x + halfHeight, y, halfWidth, halfHeight, imgWidth);
        node->bottomRight = buildQuadTree(pData, x + halfHeight, y + halfWidth, halfWidth, halfHeight, imgWidth);
    }
    return node;
}

int isHomogeneous(unsigned char *pData, int x, int y, int width, int height, int imgWidth) {
    unsigned char first = pData[x * imgWidth + y];
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (abs(pData[(x + i) * imgWidth + (y + j)] - first) > MAX_DIFF)
                return 0;
    return 1;
}

unsigned char averageColor(unsigned char *pData, int x, int y, int width, int height, int imgWidth) {
    int sum = 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            sum += pData[(x + i) * imgWidth + (y + j)];
    return sum / (width * height);
}

void writeQuadtree(struct QuadtreeNode *node, FILE *fp) {
    fwrite(&node->isLeaf, sizeof(int), 1, fp);
    if (node->isLeaf)
        fwrite(&node->color, sizeof(unsigned char), 1, fp);
    else {
        writeQuadtree(node->topLeft, fp);
        writeQuadtree(node->topRight, fp);
        writeQuadtree(node->bottomLeft, fp);
        writeQuadtree(node->bottomRight, fp);
    }
}

void freeQuadtree(struct QuadtreeNode *node) {
    if (node) {
        freeQuadtree(node->topLeft);
        freeQuadtree(node->topRight);
        freeQuadtree(node->bottomLeft);
        freeQuadtree(node->bottomRight);
        free(node);
    }
}