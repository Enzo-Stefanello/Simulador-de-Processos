#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

// Remove quebra de linha
void removerQuebraLinha(char *str) {

    str[strcspn(str, "\n")] = 0;
}

// Carrega programa assembly
void carregarPrograma(char *arquivo, Processo *p) {

    FILE *fp = fopen(arquivo, "r");

    // Verifica se abriu arquivo
    if(fp == NULL) {

        printf("Erro ao abrir arquivo %s\n", arquivo);

        exit(1);
    }

    char linha[100];

    int emCodigo = 0;
    int emDados = 0;

    p->total_instrucoes = 0;
    p->total_variaveis = 0;
    p->total_labels = 0;

    // Lê arquivo linha por linha
    while(fgets(linha, sizeof(linha), fp)) {

        removerQuebraLinha(linha);

        // Ignora linha vazia
        if(strlen(linha) == 0) {
            continue;
        }

        // Início da área de código
        if(strcmp(linha, ".code") == 0) {

            emCodigo = 1;

            continue;
        }

        // Final da área de código
        if(strcmp(linha, ".endcode") == 0) {

            emCodigo = 0;

            continue;
        }

        // Início da área de dados
        if(strcmp(linha, ".data") == 0) {

            emDados = 1;

            continue;
        }

        // Final da área de dados
        if(strcmp(linha, ".enddata") == 0) {

            emDados = 0;

            continue;
        }

        // Leitura das instruções
        if(emCodigo) {

            char label[50];
            char opcode[20];
            char operando[20];

            // Verifica se possui label
            if(strchr(linha, ':')) {

                sscanf(
                    linha,
                    "%[^:]: %s %s",
                    label,
                    opcode,
                    operando
                );

                // Salva label
                strcpy(
                    p->labels[p->total_labels].nome,
                    label
                );

                p->labels[p->total_labels].linha =
                    p->total_instrucoes;

                p->total_labels++;

            } else {

                // Linha sem label
                sscanf(linha, "%s %s", opcode, operando);
            }

            // Salva instrução
            strcpy(
                p->codigo[p->total_instrucoes].opcode,
                opcode
            );

            strcpy(
                p->codigo[p->total_instrucoes].operando,
                operando
            );

            p->total_instrucoes++;
        }

        // Leitura das variáveis
        else if(emDados) {

            char nome[50];
            int valor;

            sscanf(linha, "%s %d", nome, &valor);

            // Salva variável na memória
            strcpy(
                p->memoria[p->total_variaveis].nome,
                nome
            );

            p->memoria[p->total_variaveis].valor =
                valor;

            p->total_variaveis++;
        }
    }

    fclose(fp);
}