#ifndef CARRO_HEADERS
#define CARRO_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dono.h"
#include "constantes.h"
#include "bdados.h"

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
int compCodVeiculo(void *carro, void *codigo);
void freeCarro(void *carro);
void mostrarCarro(void *carro);

#endif