#include <stdio.h>
#include <time.h>

#include "processo.h"
#include "parser.h"
#include "cpu.h"
#include "scheduler.h"

void atualizarBloqueados(Processo processos[], int total, int tempo_global) {

    for(int i = 0; i < total; i++) {

        if(processos[i].estado == BLOCKED &&
           tempo_global >= processos[i].bloqueado_ate) {

            processos[i].estado = READY;

            printf(
                "\nProcesso %s desbloqueado.\n",
                processos[i].nome
            );
        }
    }
}

void atualizarChegadas(Processo processos[], int total, int tempo_global) {

    for(int i = 0; i < total; i++) {

        if(processos[i].estado == NEW &&
           tempo_global >= processos[i].arrival_time) {

            processos[i].estado = READY;

            printf(
                "\nProcesso %s chegou no sistema.\n",
                processos[i].nome
            );
        }
    }
}

void verificarDeadlines(Processo processos[], int total, int tempo_global) {

    for(int i = 0; i < total; i++) {

        if(processos[i].estado != FINISHED &&
           tempo_global > processos[i].deadline) {

            printf(
                "\n[DEADLINE MISS] Processo %s perdeu o deadline no tempo %d\n",
                processos[i].nome,
                tempo_global
            );

            processos[i].deadline = 999999;
        }
    }
}

int main() {

    srand(time(NULL));

    Processo processos[2];

    carregarPrograma("programas/p1.asm", &processos[0]);
    carregarPrograma("programas/p2.asm", &processos[1]);

    strcpy(processos[0].nome, "P1");
    strcpy(processos[1].nome, "P2");

    processos[0].pc = 0;
    processos[0].acc = 0;
    processos[0].estado = NEW;
    processos[0].arrival_time = 0;
    processos[0].Ci = 4;
    processos[0].Pi = 10;
    processos[0].deadline = 10;

    processos[1].pc = 0;
    processos[1].acc = 0;
    processos[1].estado = NEW;
    processos[1].arrival_time = 4;
    processos[1].Ci = 3;
    processos[1].Pi = 5;
    processos[1].deadline = 5;

    int tempo_global = 0;

    while(1) {

        atualizarChegadas(processos, 2, tempo_global);

        atualizarBloqueados(processos, 2, tempo_global);

        verificarDeadlines(processos, 2, tempo_global);

        Processo *atual = escolherProcesso(processos, 2);

        if(atual == NULL) {

            int ativos = 0;

            for(int i = 0; i < 2; i++) {

                if(processos[i].estado != FINISHED) {
                    ativos = 1;
                }
            }

            if(!ativos) {
                break;
            }

            printf("\nTEMPO %d -> CPU OCIOSA\n", tempo_global);

            tempo_global++;

            continue;
        }

        atual->estado = RUNNING;

        printf("\n========================\n");
        printf("TEMPO: %d\n", tempo_global);
        printf("PROCESSO: %s\n", atual->nome);
        printf("DEADLINE: %d\n", atual->deadline);
        printf("========================\n");

        executarInstrucao(atual, tempo_global);

        printf("ACC = %d\n", atual->acc);

        if(atual->estado == RUNNING) {
            atual->estado = READY;
        }

        tempo_global++;
    }

    printf("\nTodos os processos finalizaram.\n");

    return 0;
}