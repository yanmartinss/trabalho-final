/*********************************************************************************************/
/* Alunos      : Yan Martins de Sousa - Matrícula: 20232045050392                            */
/*               Artur Moreira Martins - Matrícula: 20231045050262                           */
/*               Guilherme Amaro Clarindo - Matrícula: 20231045050050                        */
/*               Vênancio Silva Clarindo - Matrícula: XXXXXXXXXXXXXX                         */
/* Avaliação   : Trabalho Final                                                              */
/* Data        : 04/05/2024 - Professor: Daniel Ferreira                                     */
/* Compilador  : gcc versão 6.3.0                                                            */
/*********************************************************************************************/

#ifndef QUADTREE_DECODER_H
#define QUADTREE_DECODER_H

#include <stdio.h>
#include <stdlib.h>

struct QuadtreeNode {
    int isLeaf;
    unsigned char color; 
    
    struct QuadtreeNode *topLeft, *topRight, *bottomLeft, *bottomRight;
};

struct QuadtreeNode* readQuadtree(FILE *fp);

void reconstructImage(struct QuadtreeNode *node, unsigned char *pData, int x, int y, int width, int height, int imgWidth);
void freeQuadtree(struct QuadtreeNode *node);

#endif