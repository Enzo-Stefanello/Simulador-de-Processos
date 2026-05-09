#include "cpu.h"

int buscarVariavel(Processo *p, char *nome) {

    for(int i = 0; i < p->total_variaveis; i++) {

        if(strcmp(p->memoria[i].nome, nome) == 0) {
            return p->memoria[i].valor;
        }
    }

    return 0;
}

int buscarLabel(Processo *p, char *nome) {

    for(int i = 0; i < p->total_labels; i++) {

        if(strcmp(p->labels[i].nome, nome) == 0) {
            return p->labels[i].linha;
        }
    }

    return -1;
}

void executarInstrucao(Processo *p) {

    Instrucao atual = p->codigo[p->pc];

    printf(
        "Executando: %s %s\n",
        atual.opcode,
        atual.operando
    );

    if(strcmp(atual.opcode, "LOAD") == 0) {

        if(atual.operando[0] == '#') {

            p->acc = atoi(&atual.operando[1]);

        } else {

            p->acc = buscarVariavel(p, atual.operando);
        }

        p->pc++;
    }

    else if(strcmp(atual.opcode, "ADD") == 0) {

        if(atual.operando[0] == '#') {

            p->acc += atoi(&atual.operando[1]);

        } else {

            p->acc += buscarVariavel(p, atual.operando);
        }

        p->pc++;
    }

    else if(strcmp(atual.opcode, "SUB") == 0) {

        if(atual.operando[0] == '#') {

            p->acc -= atoi(&atual.operando[1]);

        } else {

            p->acc -= buscarVariavel(p, atual.operando);
        }

        p->pc++;
    }

    else if(strcmp(atual.opcode, "BRANY") == 0) {

        p->pc = buscarLabel(p, atual.operando);
    }

    else if(strcmp(atual.opcode, "BRPOS") == 0) {

        if(p->acc > 0) {

            p->pc = buscarLabel(p, atual.operando);

        } else {

            p->pc++;
        }
    }

    else if(strcmp(atual.opcode, "BRZERO") == 0) {

        if(p->acc == 0) {

            p->pc = buscarLabel(p, atual.operando);

        } else {

            p->pc++;
        }
    }

    else if(strcmp(atual.opcode, "BRNEG") == 0) {

        if(p->acc < 0) {

            p->pc = buscarLabel(p, atual.operando);

        } else {

            p->pc++;
        }
    }

    else if(strcmp(atual.opcode, "SYSCALL") == 0) {

        int syscall = atoi(atual.operando);

        if(syscall == 0) {

            printf("Processo encerrado.\n");

            p->estado = FINISHED;
        }

        else if(syscall == 1) {

            printf("PRINT: %d\n", p->acc);
        }

        p->pc++;
    }
}