#ifndef BD_HEADERS
#define BD_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "structsGenericas.h"

typedef struct {
    Lista *donos;
    Lista *carros;
    Lista *sensores;
    float *distancias;
    int nColunasMatrizDistancias;
    Lista *viagens;
} Bdados;


int inicializarBD(Bdados *bd);
void freeTudo(Bdados *bd);
int realocarMatrizDistancias(Bdados *bd, int tamanho);


#endif