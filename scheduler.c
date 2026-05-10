#include "scheduler.h"

// EDF
// Escolhe processo READY com menor deadline
Processo* escolherProcesso(Processo processos[], int total_processos) {

    Processo *melhor = NULL;

    // Percorre todos os processos
    for(int i = 0; i < total_processos; i++) {

        // Considera apenas processos READY
        if(processos[i].estado == READY) {

            // Primeiro processo encontrado
            if(melhor == NULL ||
               
               // Escolhe menor deadline
               processos[i].deadline < melhor->deadline) {

                melhor = &processos[i];
            }
        }
    }

    // Retorna processo escolhido
    return melhor;
}