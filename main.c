#include <stdio.h>
#include <time.h>

#include "processo.h"
#include "parser.h"
#include "cpu.h"
#include "scheduler.h"

char* nomeEstado(Estado e) {

    switch(e) {

        case NEW: return "NEW";
        case READY: return "READY";
        case RUNNING: return "RUNNING";
        case BLOCKED: return "BLOCKED";
        case FINISHED: return "FINISHED";
    }

    return "";
}

void mostrarEstados(Processo processos[], int total) {

    printf("\nESTADOS:\n");

    for(int i = 0; i < total; i++) {

        printf(
            "%s -> %s\n",
            processos[i].nome,
            nomeEstado(processos[i].estado)
        );
    }

    printf("\n");
}

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
           processos[i].perdeu_deadline == 0 &&
           tempo_global > processos[i].deadline) {

            printf(
                "\n[DEADLINE MISS] Processo %s perdeu o deadline no tempo %d\n",
                processos[i].nome,
                tempo_global
            );

            processos[i].perdeu_deadline = 1;
        }
    }
}

void renovarPeriodicos(Processo processos[], int total, int tempo_global) {

    for(int i = 0; i < total; i++) {

        if(processos[i].estado == FINISHED) {

            if(tempo_global >= processos[i].arrival_time + processos[i].Pi) {

                printf(
                    "\n[TAREFA PERIODICA] %s reiniciada.\n",
                    processos[i].nome
                );

                processos[i].pc = 0;
                processos[i].acc = 0;

                processos[i].estado = READY;

                processos[i].arrival_time = tempo_global;

                processos[i].deadline =
                    tempo_global + processos[i].Pi;

                processos[i].perdeu_deadline = 0;
            }
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
    processos[0].perdeu_deadline = 0;

    processos[1].pc = 0;
    processos[1].acc = 0;
    processos[1].estado = NEW;
    processos[1].arrival_time = 4;
    processos[1].Ci = 3;
    processos[1].Pi = 5;
    processos[1].deadline = 5;
    processos[1].perdeu_deadline = 0;

    int tempo_global = 0;

    Processo *processo_anterior = NULL;

    Estado estado_anterior = READY;

    while(tempo_global < 40) {

        atualizarChegadas(processos, 2, tempo_global);

        atualizarBloqueados(processos, 2, tempo_global);

        verificarDeadlines(processos, 2, tempo_global);

        renovarPeriodicos(processos, 2, tempo_global);

        mostrarEstados(processos, 2);

        Processo *atual = escolherProcesso(processos, 2);

        if(atual == NULL) {

            printf("\nTEMPO %d -> CPU OCIOSA\n", tempo_global);

            tempo_global++;

            continue;
        }

        if(processo_anterior != NULL &&
           processo_anterior != atual &&
           estado_anterior == RUNNING) {

            printf(
                "\n[PREEMPCAO] %s interrompido -> %s assumiu CPU\n",
                processo_anterior->nome,
                atual->nome
            );
        }

        atual->estado = RUNNING;

        printf("\n========================\n");
        printf("TEMPO: %d\n", tempo_global);
        printf("PROCESSO: %s\n", atual->nome);
        printf("DEADLINE: %d\n", atual->deadline);
        printf("========================\n");

        executarInstrucao(atual, tempo_global);

        printf("ACC = %d\n", atual->acc);

        estado_anterior = atual->estado;

        if(atual->estado == RUNNING) {
            atual->estado = READY;
        }

        processo_anterior = atual;

        tempo_global++;
    }

    printf("\nSimulacao encerrada.\n");

    return 0;
}