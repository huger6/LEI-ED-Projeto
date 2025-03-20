#include <stdio.h>
#include <stdlib.h>

#include "dono.h"

#define MAX_MATRICULA 8

typedef struct Carro
{
    char matricula[MAX_MATRICULA + 1];
    char *marca;
    char *modelo;
    short ano;
    Pessoa *dono;
    int codVeiculo;

} Carro, Car, *ptCar, *ptCarro;