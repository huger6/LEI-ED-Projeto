#ifndef SENSORES_HEADERS
#define SENSORES_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bdados.h"

typedef struct {
    int codSensor; //PRIMARY KEY
    char *designacao;
    char *latitude;
    char *longitude;
} Sensor, *ptSensor;

int inserirSensorLido(Bdados *bd, int codSensor, char *designacao, char *latitude, char *longitude);
int compararSensores(void *sensor1, void *sensor2);
int compIdSensor(void *sensor, void *idSensor);
void freeSensor(void *sensor);

#endif