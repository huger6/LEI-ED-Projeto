#ifndef SENSORES_HEADERS
#define SENSORES_HEADERS

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int codSensor; //PRIMARY KEY
    char *designacao;
    char *latitude;
    char *longitude;
} Sensor, *ptSensor;

#endif