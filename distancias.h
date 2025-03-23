#ifndef DISTANCIAS_HEADERS
#define DISTANCIAS_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "sensores.h"

typedef struct {
    Sensor *sensor1, *sensor2;
    float distancia;
} Distancias, Dist, *ptDistancias, *ptDist;

#endif