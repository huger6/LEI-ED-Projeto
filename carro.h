#ifndef CARRO_HEADERS
#define CARRO_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "dono.h"

#define MAX_MATRICULA 8

typedef struct {
    char matricula[MAX_MATRICULA + 1];
    char *marca;
    char *modelo;
    short ano;
    int nifDono; //FOREIGN KEY - REFERENCES Dono(nif)
    int codVeiculo; //PRIMARY KEY
} Carro, Car, *ptCar, *ptCarro;


#endif