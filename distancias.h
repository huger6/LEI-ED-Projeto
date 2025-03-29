#ifndef DISTANCIAS_HEADERS
#define DISTANCIAS_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "sensores.h"

typedef struct {
    Sensor *sensor1, *sensor2;
    float distancia;
} Distancias, Dist, *ptDistancias, *ptDist;

int inserirDistanciaLido(BaseDados *bd, int codSensor1, int codSensor2, float distancia);
int compararDistancias(void *dist1, void *dist2);
Sensor * obterSensorPorId(BaseDados *bd, int codigo);


#endif