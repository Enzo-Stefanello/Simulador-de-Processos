#include <time.h>

#include "cpu.h"

// Busca valor de variável na memória
int buscarVariavel(Processo *p, char *nome) {

    for(int i = 0; i < p->total_variaveis; i++) {

        if(strcmp(p->memoria[i].nome, nome) == 0) {
            return p->memoria[i].valor;
        }
    }

    return 0;
}

// Altera valor de variável
void alterarVariavel(Processo *p, char *nome, int valor) {

    for(int i = 0; i < p->total_variaveis; i++) {

        if(strcmp(p->memoria[i].nome, nome) == 0) {

            p->memoria[i].valor = valor;

            return;
        }
    }
}

// Busca linha da label
int buscarLabel(Processo *p, char *nome) {

    for(int i = 0; i < p->total_labels; i++) {

        if(strcmp(p->labels[i].nome, nome) == 0) {
            return p->labels[i].linha;
        }
    }

    return -1;
}

// CPU virtual
// Executa uma instrução por vez
void executarInstrucao(Processo *p, int tempo_global) {

    // Busca instrução atual usando PC
    Instrucao atual = p->codigo[p->pc];

    // Mostra instrução executada
    printf(
        "Executando: %s %s\n",
        atual.opcode,
        atual.operando
    );

    // LOAD -> carrega valor no ACC
    if(strcmp(atual.opcode, "LOAD") == 0) {

        // Valor imediato
        if(atual.operando[0] == '#') {

            p->acc = atoi(&atual.operando[1]);

        } 
        
        // Valor da memória
        else {

            p->acc = buscarVariavel(p, atual.operando);
        }

        p->pc++;
    }

    // STORE -> salva ACC na memória
    else if(strcmp(atual.opcode, "STORE") == 0) {

        alterarVariavel(p, atual.operando, p->acc);

        p->pc++;
    }

    // ADD -> soma no ACC
    else if(strcmp(atual.opcode, "ADD") == 0) {

        if(atual.operando[0] == '#') {

            p->acc += atoi(&atual.operando[1]);

        } else {

            p->acc += buscarVariavel(p, atual.operando);
        }

        p->pc++;
    }

    // SUB -> subtrai do ACC
    else if(strcmp(atual.opcode, "SUB") == 0) {

        if(atual.operando[0] == '#') {

            p->acc -= atoi(&atual.operando[1]);

        } else {

            p->acc -= buscarVariavel(p, atual.operando);
        }

        p->pc++;
    }

    // MULT -> multiplica ACC
    else if(strcmp(atual.opcode, "MULT") == 0) {

        if(atual.operando[0] == '#') {

            p->acc *= atoi(&atual.operando[1]);

        } else {

            p->acc *= buscarVariavel(p, atual.operando);
        }

        p->pc++;
    }

    // DIV -> divide ACC
    else if(strcmp(atual.opcode, "DIV") == 0) {

        int divisor;

        if(atual.operando[0] == '#') {

            divisor = atoi(&atual.operando[1]);

        } else {

            divisor = buscarVariavel(p, atual.operando);
        }

        // Evita divisão por zero
        if(divisor != 0) {

            p->acc /= divisor;

        } else {

            printf("Erro: divisao por zero.\n");
        }

        p->pc++;
    }

    // BRANY -> salto direto
    else if(strcmp(atual.opcode, "BRANY") == 0) {

        p->pc = buscarLabel(p, atual.operando);
    }

    // BRPOS -> salta se ACC > 0
    else if(strcmp(atual.opcode, "BRPOS") == 0) {

        if(p->acc > 0) {

            p->pc = buscarLabel(p, atual.operando);

        } else {

            p->pc++;
        }
    }

    // BRZERO -> salta se ACC == 0
    else if(strcmp(atual.opcode, "BRZERO") == 0) {

        if(p->acc == 0) {

            p->pc = buscarLabel(p, atual.operando);

        } else {

            p->pc++;
        }
    }

    // BRNEG -> salta se ACC < 0
    else if(strcmp(atual.opcode, "BRNEG") == 0) {

        if(p->acc < 0) {

            p->pc = buscarLabel(p, atual.operando);

        } else {

            p->pc++;
        }
    }

    // SYSCALL -> chamada de sistema
    else if(strcmp(atual.opcode, "SYSCALL") == 0) {

        int syscall = atoi(atual.operando);

        // SYSCALL 0 -> encerra processo
        if(syscall == 0) {

            printf("Processo encerrado.\n");

            p->estado = FINISHED;
        }

        // SYSCALL 1 -> imprime ACC
        else if(syscall == 1) {

            printf("PRINT: %d\n", p->acc);

            // Gera bloqueio aleatório
            int bloqueio = (rand() % 3) + 1;

            p->bloqueado_ate = tempo_global + bloqueio;

            p->estado = BLOCKED;

            printf(
                "Processo bloqueado por %d tempos.\n",
                bloqueio
            );
        }

        // SYSCALL 2 -> entrada via teclado
        else if(syscall == 2) {

            int valor;

            printf("INPUT: ");

            scanf("%d", &valor);

            // Salva entrada no ACC
            p->acc = valor;

            // Gera bloqueio aleatório
            int bloqueio = (rand() % 3) + 1;

            p->bloqueado_ate = tempo_global + bloqueio;

            p->estado = BLOCKED;

            printf(
                "Processo bloqueado por %d tempos.\n",
                bloqueio
            );
        }

        // Próxima instrução
        p->pc++;
    }
}