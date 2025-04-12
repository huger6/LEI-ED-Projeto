#ifndef DISTANCIAS_HEADERS
#define DISTANCIAS_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "passagens.h"

typedef struct Bdados;

typedef struct {
    float *matriz;
    int nColunas;
} Distancias;

int inserirDistanciaLido(Bdados *bd, int codSensor1, int codSensor2, float distancia);
void inicializarMatrizDistancias(Bdados *bd);
int realocarMatrizDistancias(Bdados *bd, int tamanho);
void getStatsViagem(Bdados *bd, Viagem *v);


#endif