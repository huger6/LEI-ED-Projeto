#ifndef PASSAGENS_HEADERS
#define PASSAGENS_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "sensores.h"
#include "carro.h"
#include "uteis.h"

typedef struct {
    int idSensor; //PRIMARY KEY / FOREIGN KEY(idSensor) - REFERENCES Sensor(codSensor)
    int codVeiculo; //FOREIGN KEY - REFERENCES Carro(codVeiculo)
    Data data;
    char tipoRegisto;
} Passagem, Pass, *ptPassagem, *ptPass;

#endif