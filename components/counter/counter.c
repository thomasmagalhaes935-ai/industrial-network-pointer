#include "counter.h"

#include <stdio.h>

static int cont_produtos = 0;

void counter_init(void)
{
    cont_produtos = 0;

    printf("Contador inicializado.\n");
}

void counter_process_detection(void)
{
    cont_produtos++;

  //  printf("OBJETO DETECTADO!\n");
    printf("Quantidade de objetos: %d\n", cont_produtos);
}

int counter_get_count(void)
{
    return cont_produtos;
}