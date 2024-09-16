# Trabalho Final - Laboratório de Programação

Este projeto implementa a construção e decodificação de imagens utilizando a estrutura de dados **Quadtree**. O objetivo é compactar imagens no formato PGM (Portable GrayMap) e armazená-las em um arquivo binário, além de realizar a reconstrução da imagem a partir desse arquivo binário.

## Integrantes da Equipe

- **Artur Moreira Martins**
- **Guilherme Amaro Clarindo**
- **Venâncio Silva Clarindo**
- **Yan Martins de Sousa**

## Visão Geral do Projeto

O projeto está dividido em duas partes principais:

1. **Codificador (main.c)**  
   Responsável por ler a imagem PGM, construir a quadtree, e armazenar as informações compactadas em um arquivo binário. A quadtree permite a compressão ao representar regiões homogêneas da imagem de forma eficiente.

2. **Decodificador (main_decoder.c)**  
   Lê o arquivo binário gerado pelo codificador e reconstrói a imagem PGM original utilizando a quadtree.

## Estrutura do Projeto

```bash
├── include
│   ├── quadtree.h          # Definições para a quadtree
│   ├── quadtree_decoder.h  # Definições para o decodificador
├── src
│   ├── main.c              # Código principal do codificador
│   ├── main_decoder.c      # Código principal do decodificador
│   ├── quadtree.c          # Implementação da quadtree
│   ├── quadtree_decoder.c  # Implementação do decodificador
├── bin                     # Diretório para os arquivos binários gerados
├── build                   # Diretório para os arquivos objeto (.o)
└── Makefile                # Script para compilar o projeto
