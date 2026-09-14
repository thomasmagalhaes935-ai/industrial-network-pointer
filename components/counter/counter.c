#include "counter.h"

#include <stdio.h>

static int cont_produtos_bons = 0;
static int cont_produtos_ruins = 0;

void counter_init(void)
{
    cont_produtos_bons = 0;
    cont_produtos_ruins = 0;

    printf("Contadores inicializados.\n");
}

void counter_bom_process_detection(void)
{
    cont_produtos_bons++;

    printf("Produtos bons: %d\n", cont_produtos_bons);
}

void counter_ruim_process_detection(void)
{
    cont_produtos_ruins++;

    printf("Produtos ruins: %d\n", cont_produtos_ruins);
}

int counter_get_bom_count(void)
{
    return cont_produtos_bons;
}

int counter_get_ruim_count(void)
{
    return cont_produtos_ruins;
}