#ifndef PASSAGENS_HEADERS
#define PASSAGENS_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "sensores.h"
#include "carro.h"
#include "uteis.h"

typedef struct {
    int idSensor; //PRIMARY KEY / FOREIGN KEY(idSensor) - REFERENCES Sensor(codSensor)
    Carro * veiculo;
    Data data;
    char tipoRegisto;
} Passagem, Pass, *ptPassagem, *ptPass;


int inserirPassagemLido(Bdados *bd, int idSensor, int codVeiculo, Data date, char tipoRegisto);
int compararPassagens(void *passagem1, void *passagem2);
Carro * obterCarroPorId(Bdados *bd, int codigo);
void freePassagem(void *passagem);


#endif