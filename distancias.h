#ifndef DISTANCIAS_HEADERS
#define DISTANCIAS_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "sensores.h"

typedef struct {
    Sensor *sensor1, *sensor2;
    float distancia;
} Distancias, Dist, *ptDistancias, *ptDist;

int inserirDistanciaLido(Bdados *bd, int codSensor1, int codSensor2, float distancia);
int compararDistancias(void *dist1, void *dist2);
Sensor * obterSensorPorId(Bdados *bd, int codigo);
void freeDistancia(void *distancia);


#endif