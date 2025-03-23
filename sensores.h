#ifndef SENSORES_HEADERS
#define SENSORES_HEADERS

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int codSensor; //PRIMARY KEY
    char *designacao;
    char *latitude; //Pode ser necessário criar uma struct apenas para as coordenadas
    char *longitude;
} Sensor, *ptSensor;

#endif