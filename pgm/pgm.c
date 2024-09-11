#include <stdio.h>
#include <stdlib.h>

void codificarPGMparaBinario(const char *input_pgm, const char *output_bin) {
    FILE *pgm_file = fopen(input_pgm, "rb");
    if (pgm_file == NULL) {
        printf("Erro ao abrir a imagem PGM.\n");
        exit(1);
    }

    FILE *bin_file = fopen(output_bin, "wb");
    if (bin_file == NULL) {
        printf("Erro ao criar o arquivo binário.\n");
        fclose(pgm_file);
        exit(1);
    }

    // Ler o cabeçalho do PGM
    char formato[3];
    int largura, altura, max_valor;

    fscanf(pgm_file, "%2s", formato);
    if (formato[0] != 'P' || formato[1] != '5') {
        printf("Formato PGM inválido. Esperado P5 (binário).\n");
        fclose(pgm_file);
        fclose(bin_file);
        exit(1);
    }

    // Ler a largura, altura e o valor máximo de pixel
    fscanf(pgm_file, "%d %d %d", &largura, &altura, &max_valor);
    fgetc(pgm_file); // Consumir o caractere de nova linha após o cabeçalho

    // Exibir o valor máximo de pixel encontrado
    printf("Valor máximo de pixel encontrado: %d\n", max_valor);

    // Verificar se o valor máximo de pixel é suportado
    if (max_valor > 65535) {
        printf("Valor máximo de pixel %d não suportado.\n", max_valor);
        fclose(pgm_file);
        fclose(bin_file);
        exit(1);
    }

    // Determinar o número de bytes por pixel com base no valor máximo
    int bytes_por_pixel = (max_valor > 255) ? 2 : 1;

    // Gravar dados de imagem (binário) no arquivo .bin
    unsigned char *buffer = (unsigned char *)malloc(largura * altura * bytes_por_pixel);
    if (buffer == NULL) {
        printf("Erro de alocação de memória.\n");
        fclose(pgm_file);
        fclose(bin_file);
        exit(1);
    }

    fread(buffer, bytes_por_pixel, largura * altura, pgm_file);

    // Ajustar o valor dos pixels para o intervalo de 0 a 255 (normalização)
    if (max_valor != 255) {
        for (int i = 0; i < largura * altura * bytes_por_pixel; i++) {
            buffer[i] = (buffer[i] * 255) / max_valor;  // Normaliza os valores de pixel para o intervalo 0-255
        }
    }

    // Escrever os dados normalizados no arquivo binário
    fwrite(buffer, bytes_por_pixel, largura * altura, bin_file);

    // Fechar arquivos e liberar memória
    free(buffer);
    fclose(pgm_file);
    fclose(bin_file);

    printf("Arquivo %s criado com sucesso.\n", output_bin);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_pgm>\n", argv[0]);
        return 1;
    }

    const char *input_pgm = argv[1];  // Nome do arquivo PGM passado como argumento
    const char *output_bin = "bitstream.bin";

    codificarPGMparaBinario(input_pgm, output_bin);

    return 0;
}
