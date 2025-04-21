#ifndef DISTANCIAS_HEADERS
#define DISTANCIAS_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "passagens.h"

struct Bdados;

typedef struct {
    float *matriz;
    int nColunas;
} Distancias;

int inserirDistanciaLido(struct Bdados *bd, int codSensor1, int codSensor2, float distancia);
void inicializarMatrizDistancias(struct Bdados *bd);
int realocarMatrizDistancias(struct Bdados *bd, int nColunas);
void freeMatrizDistancias(Distancias *distancia);
void guardarDistanciasBin(Distancias *distancia, FILE *file);
Distancias *readDistanciasBin(FILE *file);
void exportarDistanciasXML(Distancias *d, int indentacao, FILE *file);


#endif