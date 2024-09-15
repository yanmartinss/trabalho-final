#include "quadtree.h"

QuadtreeNode* construirQuadTree(unsigned char *pData, int x, int y, int largura, int altura, int imgLargura) {
    QuadtreeNode *node = malloc(sizeof(QuadtreeNode));
    if (homogeneo(pData, x, y, largura, altura, imgLargura)) {
        node->nodoFolha = 1;
        node->color = media(pData, x, y, largura, altura, imgLargura);
        node->topLeft = node->topRight = node->bottomLeft = node->bottomRight = NULL;
    } else {
        int metadeLargura = largura / 2, metadeAltura = altura / 2;
        node->nodoFolha = 0;
        node->topLeft = construirQuadTree(pData, x, y, metadeLargura, metadeAltura, imgLargura);
        node->topRight = construirQuadTree(pData, x, y + metadeLargura, metadeLargura, metadeAltura, imgLargura);
        node->bottomLeft = construirQuadTree(pData, x + metadeAltura, y, metadeLargura, metadeAltura, imgLargura);
        node->bottomRight = construirQuadTree(pData, x + metadeAltura, y + metadeLargura, metadeLargura, metadeAltura, imgLargura);
    }
    return node;
}

int homogeneo(unsigned char *pData, int x, int y, int largura, int altura, int imgLargura) {
    unsigned char first = pData[x * imgLargura + y];
    for (int i = 0; i < altura; i++)
        for (int j = 0; j < largura; j++)
            if (abs(pData[(x + i) * imgLargura + (y + j)] - first) > MAX_DIFF)
                return 0;
    return 1;
}

unsigned char media(unsigned char *pData, int x, int y, int largura, int altura, int imgLargura) {
    int soma = 0;
    for (int i = 0; i < altura; i++)
        for (int j = 0; j < largura; j++)
            soma += pData[(x + i) * imgLargura + (y + j)];
    return soma / (largura * altura);
}

void escreverQuadtree(QuadtreeNode *node, FILE *fp) {
    fwrite(&node->nodoFolha, sizeof(int), 1, fp);
    if (node->nodoFolha)
        fwrite(&node->color, sizeof(unsigned char), 1, fp);
    else {
        escreverQuadtree(node->topLeft, fp);
        escreverQuadtree(node->topRight, fp);
        escreverQuadtree(node->bottomLeft, fp);
        escreverQuadtree(node->bottomRight, fp);
    }
}

void freeQuadtree(QuadtreeNode *node) {
    if (node) {
        freeQuadtree(node->topLeft);
        freeQuadtree(node->topRight);
        freeQuadtree(node->bottomLeft);
        freeQuadtree(node->bottomRight);
        free(node);
    }
}