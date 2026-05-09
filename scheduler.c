#include "scheduler.h"

Processo* escolherProcesso(Processo processos[], int total_processos) {

    Processo *melhor = NULL;

    for(int i = 0; i < total_processos; i++) {

        if(processos[i].estado == READY) {

            if(melhor == NULL ||
               processos[i].deadline < melhor->deadline) {

                melhor = &processos[i];
            }
        }
    }

    return melhor;
}