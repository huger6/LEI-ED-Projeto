#ifndef CARRO_HEADERS
#define CARRO_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dono.h"

#define MAX_MATRICULA 8

typedef struct {
    char matricula[MAX_MATRICULA + 1];
    char *marca;
    char *modelo;
    short ano;
    int codVeiculo; //PRIMARY KEY
    Dono *ptrPessoa;
} Carro, Car, *ptCar, *ptCarro;


int compararCarros(void *carro1, void *carro2);
int inserirCarroLido(Bdados *bd, char *matricula, char *marca, char *modelo, short ano, int nif, int codVeiculo);
Dono * obterDonoCarro(Bdados *bd, int nif);
void freeCarro(void *carro);

#endif