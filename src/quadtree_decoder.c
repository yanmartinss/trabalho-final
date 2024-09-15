#include "quadtree_decoder.h"

// Função para ler a quadtree do arquivo binário
QuadtreeNode* lerQuadtree(FILE *fp) {
    QuadtreeNode *node = (QuadtreeNode *)malloc(sizeof(QuadtreeNode));
    fread(&node->nodoFolha, sizeof(int), 1, fp);

    if (node->nodoFolha) {
        fread(&node->color, sizeof(unsigned char), 1, fp);
        node->topLeft = node->topRight = node->bottomLeft = node->bottomRight = NULL;
    } else {
        node->topLeft = lerQuadtree(fp);
        node->topRight = lerQuadtree(fp);
        node->bottomLeft = lerQuadtree(fp);
        node->bottomRight = lerQuadtree(fp);
    }
    return node;
}

// Função para reconstruir a imagem a partir da quadtree
void reconstruirImagem(QuadtreeNode *node, unsigned char *pData, int x, int y, int largura, int altura, int imgLargura) {
    if (node->nodoFolha) {
        for (int i = 0; i < altura; i++) {
            for (int j = 0; j < largura; j++) {
                pData[(x + i) * imgLargura + (y + j)] = node->color;
            }
        }
    } else {
        int metadeLargura = largura / 2;
        int metadeAltura = altura / 2;

        reconstruirImagem(node->topLeft, pData, x, y, metadeLargura, metadeAltura, imgLargura);
        reconstruirImagem(node->topRight, pData, x, y + metadeLargura, metadeLargura, metadeAltura, imgLargura);
        reconstruirImagem(node->bottomLeft, pData, x + metadeAltura, y, metadeLargura, metadeAltura, imgLargura);
        reconstruirImagem(node->bottomRight, pData, x + metadeAltura, y + metadeLargura, metadeLargura, metadeAltura, imgLargura);
    }
}

// Função para liberar a memória da quadtree
void liberarQuadtree(QuadtreeNode *node) {
    if (node) {
        liberarQuadtree(node->topLeft);
        liberarQuadtree(node->topRight);
        liberarQuadtree(node->bottomLeft);
        liberarQuadtree(node->bottomRight);
        free(node);
    }
}