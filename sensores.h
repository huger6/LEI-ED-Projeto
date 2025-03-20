#include <stdio.h>
#include <stdlib.h>

typedef struct Sensores
{
    short codSensor;
    char *desigSensor;
    char *latitude;
    char *longitude;

} Sensor, *ptSensor;