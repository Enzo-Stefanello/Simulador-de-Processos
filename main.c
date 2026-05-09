#include <stdio.h>

#include "processo.h"
#include "parser.h"
#include "cpu.h"
#include "scheduler.h"

int main() {

    Processo processos[2];

    carregarPrograma("programas/p1.asm", &processos[0]);
    carregarPrograma("programas/p2.asm", &processos[1]);

    strcpy(processos[0].nome, "P1");
    strcpy(processos[1].nome, "P2");

    processos[0].pc = 0;
    processos[0].acc = 0;
    processos[0].estado = READY;
    processos[0].arrival_time = 0;
    processos[0].Ci = 4;
    processos[0].Pi = 10;
    processos[0].deadline = 10;

    processos[1].pc = 0;
    processos[1].acc = 0;
    processos[1].estado = READY;
    processos[1].arrival_time = 0;
    processos[1].Ci = 3;
    processos[1].Pi = 5;
    processos[1].deadline = 5;

    int tempo_global = 0;

    while(1) {

        Processo *atual = escolherProcesso(processos, 2);

        if(atual == NULL) {
            break;
        }

        atual->estado = RUNNING;

        printf("\n========================\n");
        printf("TEMPO: %d\n", tempo_global);
        printf("PROCESSO: %s\n", atual->nome);
        printf("DEADLINE: %d\n", atual->deadline);
        printf("========================\n");

        executarInstrucao(atual);

        printf("ACC = %d\n", atual->acc);

        if(atual->estado == RUNNING) {
            atual->estado = READY;
        }

        tempo_global++;
    }

    printf("\nTodos os processos finalizaram.\n");

    return 0;
}