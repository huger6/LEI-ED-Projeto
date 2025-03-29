#ifndef SENSORES_HEADERS
#define SENSORES_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int codSensor; //PRIMARY KEY
    char *designacao;
    char *latitude;
    char *longitude;
} Sensor, *ptSensor;

int inserirSensorLido(BaseDados *bd, int codSensor, char *designacao, char *latitude, char *longitude);
int compararSensores(void *sensor1, void *sensor2);

#endif