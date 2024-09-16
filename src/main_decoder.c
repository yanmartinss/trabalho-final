/*********************************************************************************************/
/* Alunos      : Yan Martins de Sousa - Matrícula: 20232045050392                            */
/*               Artur Moreira Martins - Matrícula: 20231045050262                           */
/*               Guilherme Amaro Clarindo - Matrícula: 20231045050050                        */
/*               Vênancio Silva Clarindo - Matrícula: 20232045050066                         */
/* Avaliação   : Trabalho Final                                                              */
/* Data        : 04/05/2024 - Professor: Daniel Ferreira                                     */
/* Compilador  : gcc versão 6.3.0                                                            */
/*********************************************************************************************/

#include "quadtree_decoder.h" //

int main(int argc, char *argv[]) { //função principal que recebe argumentos da linha de comando.
    if (argc < 3) { //argc é o número de argumentos e argv é o vetor de strings com os argumentos passados.
        printf("Uso: %s <arquivo_bin> <imagem_pgm>\n", argv[0]);
        return 1; //Verifica se o número de argumentos é menor que 3. Se for, imprime uma mensagem explicando o uso correto do programa e termina a execução retornando 1, indicando um erro.
    }

    const char *input_bin = argv[1]; // Armazena os dois argumentos da linha de comando em variáveis.
    const char *output_pgm = argv[2]; //input_bin é o nome do arquivo binario de entrada. output_bin é o nome do arquivo PGM de saida.

    FILE *bin_file = fopen(input_bin, "rb"); //Abre o arquivo binário de entrada para leitura em modo binário ("rb").
    if (!bin_file) {
        puts("Erro ao abrir o arquivo binário.\n"); //Se o arquivo não puder ser aberto (retornando NULL), imprime uma mensagem de erro e termina o programa com 1.
        return 1;
    }

    int width, height;
    fread(&width, sizeof(int), 1, bin_file); //Lê a largura e altura da imagem do arquivo binário. 
    fread(&height, sizeof(int), 1, bin_file); //Cada uma dessas informações ocupa 4 bytes . O fread lê esses valores do arquivo e os armazena nas variáveis width e height.

    struct QuadtreeNode *root = readQuadtree(bin_file); //Chama a função readQuadtree, que lê a árvore quaternária do arquivo binário e retorna um ponteiro para a raiz da árvore, armazenado em root.
    fclose(bin_file); //fecha o arquivo binario.

    unsigned char *pData = (unsigned char *)malloc(width * height * sizeof(unsigned char)); //Aloca dinamicamente um bloco de memória para armazenar os dados da imagem. 
    //Como a imagem é em tons de cinza (um byte por pixel), o espaço alocado é de width * height bytes.
    
    reconstructImage(root, pData, 0, 0, width, height, width); //Chama a função reconstructImage, que percorre a quadtree e reconstrói a imagem.

    FILE *pgm_file = fopen(output_pgm, "wb"); //Abre o arquivo PGM de saída para escrita em modo binário
    if (!pgm_file) {
        puts("Erro ao criar o arquivo PGM.\n"); //Se o arquivo não puder ser criado, imprime uma mensagem de erro
        free(pData); //libera a memória alocada para a imagem.
        freeQuadtree(root);
        return 1; 
    }

    fprintf(pgm_file, "P5\n%d %d\n255\n", width, height); //Escreve o cabeçalho do arquivo PGM. A forma P5 indica que é uma imagem PGM binária, e em seguida são escritos as dimensões da imagem. 
    fwrite(pData, sizeof(unsigned char), width * height, pgm_file); //Escreve os dados armazenados da imagem no arquivo PGM. width * height são valores de pixels, cada um de 1 byte
    fclose(pgm_file);

    free(pData); //Libera a memória alocada anteriormente para armazenar os dados da imagem.
    freeQuadtree(root);

    printf("Imagem %s reconstruída com sucesso.\n", output_pgm);
    return 0; //Programa finalizado com SUCESSO!
}
