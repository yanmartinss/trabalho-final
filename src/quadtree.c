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

// Função para construir a árvore quadtree
struct QuadtreeNode* buildQuadTree(unsigned char *pData, int x, int y, int width, int height, int imgWidth) {
    // Aloca memória para um novo nó da árvore quadtree
    struct QuadtreeNode *node = malloc(sizeof(struct QuadtreeNode));

    // Verifica se a região é homogênea (se todos os pixels são semelhantes)
    if (isHomogeneous(pData, x, y, width, height, imgWidth)) {
        node->isLeaf = 1;  // Marca o bloco como homogêneo
       // Calcula a cor média da região homogênea
        node->color = averageColor(pData, x, y, width, height, imgWidth);
        // Inicializa os ponteiros como NULL
        node->topLeft = node->topRight = node->bottomLeft = node->bottomRight = NULL;
    } else {
        int halfWidth = width / 2, halfHeight = height / 2;
        node->isLeaf = 0;  // Marca o bloco como não homogeneo
        // Constrói recursivamente os quadrantes
        node->topLeft = buildQuadTree(pData, x, y, halfWidth, halfHeight, imgWidth);
        node->topRight = buildQuadTree(pData, x, y + halfWidth, halfWidth, halfHeight, imgWidth);
        node->bottomLeft = buildQuadTree(pData, x + halfHeight, y, halfWidth, halfHeight, imgWidth);
        node->bottomRight = buildQuadTree(pData, x + halfHeight, y + halfWidth, halfWidth, halfHeight, imgWidth);
    }
    return node;  // Retorna a imagem separada por blocos
    
    
}

// Função para verificar se uma região é homogênea
int isHomogeneous(unsigned char *pData, int x, int y, int width, int height, int imgWidth) {
    unsigned char first = pData[x * imgWidth + y];  // Pega o valor do primeiro pixel na região
    // Verifica todos os pixels na região para comparar com o primeiro pixel
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            // Se a diferença entre o pixel atual e o primeiro for maior que MAX_DIFF, a região não é homogênea
            if (abs(pData[(x + i) * imgWidth + (y + j)] - first) > MAX_DIFF)
                return 0;
    return 1;  // Retorna 1 se a região for homogênea
}

// Função para calcular a cor média de uma região
unsigned char averageColor(unsigned char *pData, int x, int y, int width, int height, int imgWidth) {
    int sum = 0;  // Inicializa a soma das cores
    // Soma todos os valores dos pixels na região
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            sum += pData[(x + i) * imgWidth + (y + j)];
    // Calcula a média e retorna o valor da cor média
    return sum / (width * height);
}

// Função para escrever a árvore quadtree em um arquivo
void writeQuadtree(struct QuadtreeNode *node, FILE *fp) {
    // Escreve o valor de isLeaf do nó no arquivo
    fwrite(&node->isLeaf, sizeof(int), 1, fp);
    if (node->isLeaf)
        // Se for um nó folha, escreve o valor da cor no arquivo
        fwrite(&node->color, sizeof(unsigned char), 1, fp);
    else {
        // Se não for um nó folha, escreve recursivamente os quatro filhos
        writeQuadtree(node->topLeft, fp);
        writeQuadtree(node->topRight, fp);
        writeQuadtree(node->bottomLeft, fp);
        writeQuadtree(node->bottomRight, fp);
    }
}

// Função para liberar a memória alocada para a árvore quadtree
void freeQuadtree(struct QuadtreeNode *node) {
    if (node) {
        // Libera recursivamente a memória dos quatro filhos
        freeQuadtree(node->topLeft);
        freeQuadtree(node->topRight);
        freeQuadtree(node->bottomLeft);
        freeQuadtree(node->bottomRight);
        // Libera a memória do nó atual
        free(node);
    }
}
