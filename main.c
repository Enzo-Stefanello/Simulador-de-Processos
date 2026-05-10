#include <stdio.h>
#include <time.h>

#include "processo.h"
#include "parser.h"
#include "cpu.h"
#include "scheduler.h"

// Retorna nome do estado
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

// Mostra estados dos processos
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

// Atualiza processos bloqueados
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

// Atualiza chegada dos processos
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

// Verifica perda de deadline
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

// Reinicia tarefas periódicas
void renovarPeriodicos(Processo processos[], int total, int tempo_global) {

    for(int i = 0; i < total; i++) {

        if(processos[i].estado == FINISHED) {

            // Verifica novo período
            if(tempo_global >= processos[i].arrival_time + processos[i].Pi) {

                printf(
                    "\n[TAREFA PERIODICA] %s reiniciada.\n",
                    processos[i].nome
                );

                // Reinicia execução
                processos[i].pc = 0;
                processos[i].acc = 0;

                processos[i].estado = READY;

                // Atualiza período
                processos[i].arrival_time = tempo_global;

                // Atualiza deadline
                processos[i].deadline =
                    tempo_global + processos[i].Pi;

                processos[i].perdeu_deadline = 0;
            }
        }
    }
}

// Função principal
int main() {

    // Inicializa aleatoriedade
    srand(time(NULL));

    // Cria processos
    Processo processos[2];

    // Carrega programas assembly
    carregarPrograma("programas/p1.asm", &processos[0]);
    carregarPrograma("programas/p2.asm", &processos[1]);

    // Define nomes
    strcpy(processos[0].nome, "P1");
    strcpy(processos[1].nome, "P2");

    // Configuração inicial do P1
    processos[0].pc = 0;
    processos[0].acc = 0;
    processos[0].estado = NEW;
    processos[0].arrival_time = 0;
    processos[0].Ci = 4;
    processos[0].Pi = 10;
    processos[0].deadline = 10;
    processos[0].perdeu_deadline = 0;

    // Configuração inicial do P2
    processos[1].pc = 0;
    processos[1].acc = 0;
    processos[1].estado = NEW;
    processos[1].arrival_time = 4;
    processos[1].Ci = 3;
    processos[1].Pi = 5;
    processos[1].deadline = 5;
    processos[1].perdeu_deadline = 0;

    // Relógio global
    int tempo_global = 0;

    // Controle de preempção
    Processo *processo_anterior = NULL;

    Estado estado_anterior = READY;

    // Loop principal do simulador
    while(tempo_global < 40) {

        // Atualiza chegadas
        atualizarChegadas(processos, 2, tempo_global);

        // Atualiza desbloqueios
        atualizarBloqueados(processos, 2, tempo_global);

        // Verifica deadlines
        verificarDeadlines(processos, 2, tempo_global);

        // Atualiza tarefas periódicas
        renovarPeriodicos(processos, 2, tempo_global);

        // Mostra estados
        mostrarEstados(processos, 2);

        // EDF escolhe processo
        Processo *atual = escolherProcesso(processos, 2);

        // CPU ociosa
        if(atual == NULL) {

            printf("\nTEMPO %d -> CPU OCIOSA\n", tempo_global);

            tempo_global++;

            continue;
        }

        // Detecta preempção
        if(processo_anterior != NULL &&
           processo_anterior != atual &&
           estado_anterior == RUNNING) {

            printf(
                "\n[PREEMPCAO] %s interrompido -> %s assumiu CPU\n",
                processo_anterior->nome,
                atual->nome
            );
        }

        // Processo entra em execução
        atual->estado = RUNNING;

        // Mostra execução atual
        printf("\n========================\n");
        printf("TEMPO: %d\n", tempo_global);
        printf("PROCESSO: %s\n", atual->nome);
        printf("DEADLINE: %d\n", atual->deadline);
        printf("========================\n");

        // Executa uma instrução
        executarInstrucao(atual, tempo_global);

        printf("ACC = %d\n", atual->acc);

        // Salva estado anterior
        estado_anterior = atual->estado;

        // Processo volta para READY
        if(atual->estado == RUNNING) {
            atual->estado = READY;
        }

        // Salva último processo executado
        processo_anterior = atual;

        // Avança tempo
        tempo_global++;
    }

    printf("\nSimulacao encerrada.\n");

    return 0;
}