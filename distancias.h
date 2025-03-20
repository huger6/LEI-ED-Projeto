#include <stdio.h>
#include <stdlib.h>

#include "sensores.h"

typedef struct Distancias
{
    Sensor *SensorN1, *SensorN2;
    float distancia;
} Distancias, Dist, *ptDistancias, *ptDist;