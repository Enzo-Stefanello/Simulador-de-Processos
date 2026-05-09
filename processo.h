#ifndef PROCESSO_H
#define PROCESSO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    FINISHED
} Estado;

typedef struct {
    char opcode[20];
    char operando[50];
} Instrucao;

typedef struct {
    char nome[30];
    int valor;
} Variavel;

typedef struct {
    char nome[30];
    int linha;
} Label;

typedef struct {

    char nome[30];

    int pc;
    int acc;

    Estado estado;

    int arrival_time;

    int Ci;
    int Pi;

    int deadline;

    int tempo_executado;

    int bloqueado_ate;

    Instrucao codigo[100];
    int total_instrucoes;

    Variavel memoria[100];
    int total_variaveis;

    Label labels[100];
    int total_labels;

} Processo;

#endif