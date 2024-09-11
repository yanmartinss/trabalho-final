#include <stdio.h>
#include <stdlib.h>

// Função para realizar a compressão RLE (Run-Length Encoding)
void compressRLE(char *inputFile, char *outputFile) {
    FILE *in = fopen(inputFile, "rb");
    FILE *out = fopen(outputFile, "wb");

    if (!in || !out) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    int current, count;
    int prev = fgetc(in);  // Lê o primeiro byte

    while (prev != EOF) {
        count = 1;
        while ((current = fgetc(in)) == prev && count < 255) {
            count++;
        }

        fputc(prev, out);   // Escreve o valor do pixel
        fputc(count, out);  // Escreve o número de repetições

        prev = current;
    }

    fclose(in);
    fclose(out);
    printf("Compressão concluída!\n");
}

// Função para realizar a descompressão RLE
void decompressRLE(char *inputFile, char *outputFile) {
    FILE *in = fopen(inputFile, "rb");
    FILE *out = fopen(outputFile, "wb");

    if (!in || !out) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    int value, count;
    
    while ((value = fgetc(in)) != EOF) {
        count = fgetc(in);
        for (int i = 0; i < count; i++) {
            fputc(value, out);
        }
    }

    fclose(in);
    fclose(out);
    printf("Descompressão concluída!\n");
}

int main() {
    char inputFile[] = "./dataset/novasImagens/novaimg01.pgm";
    char compressedFile[] = "compressed.pgm";
    char decompressedFile[] = "decompressed.pgm";

    // Compressão
    compressRLE(inputFile, compressedFile);

    // Descompressão
    decompressRLE(compressedFile, decompressedFile);

    return 0;
}
