#include <stdio.h>
#include <stdlib.h>

#include "sensores.h"
#include "carro.h"
#include "uteis.h"

typedef struct Passagem
{
    Sensor *idSensor;
    Carro *codVeiculo;
    Data *data;
    char TipoRegisto[2];

} Passagem, Pass, *ptPassagem, *ptPass;