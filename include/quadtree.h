/*********************************************************************************************/
/* Alunos      : Yan Martins de Sousa - Matrícula: 20232045050392                            */
/*               Artur Moreira Martins - Matrícula: 20231045050262                           */
/*               Guilherme Amaro Clarindo - Matrícula: 20231045050050                        */
/*               Vênancio Silva Clarindo - Matrícula: XXXXXXXXXXXXXX                         */
/* Avaliação   : Trabalho Final                                                              */
/* Data        : 04/05/2024 - Professor: Daniel Ferreira                                     */
/* Compilador  : gcc versão 6.3.0                                                            */
/*********************************************************************************************/

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

struct QuadtreeNode* buildQuadTree(unsigned char *pData, int x, int y, int width, int height, int imgWidth);

int isHomogeneous(unsigned char *pData, int x, int y, int width, int height, int imgWidth);
unsigned char averageColor(unsigned char *pData, int x, int y, int width, int height, int imgWidth);
void writeQuadtree(struct QuadtreeNode *node, FILE *fp);
void freeQuadtree(struct QuadtreeNode *node);

#endif