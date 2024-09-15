#include <stdio.h>
#include <stdlib.h>

typedef struct QuadtreeNode {
    int nodoFolha;
    unsigned char color;
    struct QuadtreeNode *topLeft, *topRight, *bottomLeft, *bottomRight;
} QuadtreeNode;

// Funções necessárias para o decodificador
QuadtreeNode* lerQuadtree(FILE *fp);
void reconstruirImagem(QuadtreeNode *node, unsigned char *pData, int x, int y, int largura, int altura, int imgLargura);
void liberarQuadtree(QuadtreeNode *node);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <arquivo_bin> <imagem_pgm>\n", argv[0]);
        return 1;
    }

    const char *input_bin = argv[1];  // Arquivo bitstream
    const char *output_pgm = argv[2]; // Arquivo PGM de saída

    // Abrir o arquivo binário
    FILE *bin_file = fopen(input_bin, "rb");
    if (!bin_file) {
        puts("Erro ao abrir o arquivo binário.\n");
        return 1;
    }

    // Ler a largura e altura do arquivo binário
    int largura, altura;
    fread(&largura, sizeof(int), 1, bin_file);
    fread(&altura, sizeof(int), 1, bin_file);

    // Ler a quadtree do bitstream
    QuadtreeNode *root = lerQuadtree(bin_file);
    fclose(bin_file);

    // Alocar espaço para a imagem
    unsigned char *pData = (unsigned char *)malloc(largura * altura * sizeof(unsigned char));

    // Reconstruir a imagem a partir da quadtree
    reconstruirImagem(root, pData, 0, 0, largura, altura, largura);

    // Escrever a imagem PGM de saída
    FILE *pgm_file = fopen(output_pgm, "wb");
    if (!pgm_file) {
        puts("Erro ao criar o arquivo PGM.\n");
        free(pData);
        liberarQuadtree(root);
        return 1;
    }

    // Escrever o cabeçalho PGM
    fprintf(pgm_file, "P5\n%d %d\n255\n", largura, altura);
    fwrite(pData, sizeof(unsigned char), largura * altura, pgm_file);
    fclose(pgm_file);

    // Limpar a memória
    free(pData);
    liberarQuadtree(root);

    printf("Imagem %s reconstruída com sucesso.\n", output_pgm);
    return 0;
}

// Função para ler a quadtree do arquivo binário
QuadtreeNode* lerQuadtree(FILE *fp) {
    QuadtreeNode *node = (QuadtreeNode *)malloc(sizeof(QuadtreeNode));
    fread(&node->nodoFolha, sizeof(int), 1, fp);

    if (node->nodoFolha) {
        // Se for nó folha, ler a cor
        fread(&node->color, sizeof(unsigned char), 1, fp);
        node->topLeft = node->topRight = node->bottomLeft = node->bottomRight = NULL;
    } else {
        // Se não for nó folha, ler recursivamente os filhos
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
        // Se for um nó folha, preencher o bloco com a cor
        for (int i = 0; i < altura; i++) {
            for (int j = 0; j < largura; j++) {
                pData[(x + i) * imgLargura + (y + j)] = node->color;
            }
        }
    } else {
        // Se não for nó folha, continuar reconstruindo recursivamente os filhos
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
    if (node != NULL) {
        liberarQuadtree(node->topLeft);
        liberarQuadtree(node->topRight);
        liberarQuadtree(node->bottomLeft);
        liberarQuadtree(node->bottomRight);
        free(node);
    }
}
