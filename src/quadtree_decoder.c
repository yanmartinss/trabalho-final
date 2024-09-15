#include "quadtree_decoder.h"

// Função para ler a quadtree do arquivo binário
struct QuadtreeNode* readQuadtree(FILE *fp) {
    struct QuadtreeNode *node = (struct QuadtreeNode *)malloc(sizeof(struct QuadtreeNode));
    fread(&node->isLeaf, sizeof(int), 1, fp);

    if (node->isLeaf) {
        fread(&node->color, sizeof(unsigned char), 1, fp);
        node->topLeft = node->topRight = node->bottomLeft = node->bottomRight = NULL;
    } else {
        node->topLeft = readQuadtree(fp);
        node->topRight = readQuadtree(fp);
        node->bottomLeft = readQuadtree(fp);
        node->bottomRight = readQuadtree(fp);
    }
    return node;
}

// Função para reconstruir a imagem a partir da quadtree
void reconstructImage(struct QuadtreeNode *node, unsigned char *pData, int x, int y, int width, int height, int imgWidth) {
    if (node->isLeaf) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                pData[(x + i) * imgWidth + (y + j)] = node->color;
            }
        }
    } else {
        int halfWidth = width / 2;
        int halfHeight = height / 2;

        reconstructImage(node->topLeft, pData, x, y, halfWidth, halfHeight, imgWidth);
        reconstructImage(node->topRight, pData, x, y + halfWidth, halfWidth, halfHeight, imgWidth);
        reconstructImage(node->bottomLeft, pData, x + halfHeight, y, halfWidth, halfHeight, imgWidth);
        reconstructImage(node->bottomRight, pData, x + halfHeight, y + halfWidth, halfWidth, halfHeight, imgWidth);
    }
}

// Função para liberar a memória da quadtree
void freeQuadtree(struct QuadtreeNode *node) {
    if (node) {
        freeQuadtree(node->topLeft);
        freeQuadtree(node->topRight);
        freeQuadtree(node->bottomLeft);
        freeQuadtree(node->bottomRight);
        free(node);
    }
}