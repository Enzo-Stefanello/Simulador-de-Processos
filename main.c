#include <stdio.h>

#include "processo.h"
#include "parser.h"

int main() {

    Processo p;

    carregarPrograma("programas/teste.asm", &p);

    printf("Programa carregado!\n\n");

    printf("INSTRUCOES:\n\n");

    for(int i = 0; i < p.total_instrucoes; i++) {

        printf(
            "%d -> %s %s\n",
            i,
            p.codigo[i].opcode,
            p.codigo[i].operando
        );
    }

    printf("\n");

    printf("VARIAVEIS:\n\n");

    for(int i = 0; i < p.total_variaveis; i++) {

        printf(
            "%s = %d\n",
            p.memoria[i].nome,
            p.memoria[i].valor
        );
    }

    return 0;
}