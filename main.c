#include <stdio.h>

#include "processo.h"
#include "parser.h"
#include "cpu.h"

int main() {

    Processo p;

    p.pc = 0;
    p.acc = 0;
    p.estado = READY;

    carregarPrograma("programas/teste.asm", &p);

    while(p.estado != FINISHED) {

        executarInstrucao(&p);

        printf("ACC = %d\n", p.acc);

        printf("\n");
    }

    return 0;
}