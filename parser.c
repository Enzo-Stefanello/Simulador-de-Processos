#include "parser.h"

void carregarPrograma(char *arquivo, Processo *p) {

    FILE *fp = fopen(arquivo, "r");

    if(fp == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    char linha[100];

    int dentroCode = 0;
    int dentroData = 0;

    p->total_instrucoes = 0;
    p->total_variaveis = 0;

    while(fgets(linha, sizeof(linha), fp)) {

        linha[strcspn(linha, "\r\n")] = '\0';

        if(strlen(linha) == 0) {
            continue;
        }

        if(strcmp(linha, ".code") == 0) {
            dentroCode = 1;
            continue;
        }

        if(strcmp(linha, ".endcode") == 0) {
            dentroCode = 0;
            continue;
        }

        if(strcmp(linha, ".data") == 0) {
            dentroData = 1;
            continue;
        }

        if(strcmp(linha, ".enddata") == 0) {
            dentroData = 0;
            continue;
        }

        if(dentroCode) {

            Instrucao instrucao;

            strcpy(instrucao.operando, "");

            sscanf(
                linha,
                "%s %s",
                instrucao.opcode,
                instrucao.operando
            );

            p->codigo[p->total_instrucoes] = instrucao;

            p->total_instrucoes++;
        }

        if(dentroData) {

            Variavel var;

            sscanf(
                linha,
                "%s %d",
                var.nome,
                &var.valor
            );

            p->memoria[p->total_variaveis] = var;

            p->total_variaveis++;
        }
    }

    fclose(fp);
}