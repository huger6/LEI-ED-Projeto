#include "sensores.h"

int inserirSensorLido(Bdados *bd, int codSensor, char *designacao, char *latitude, char *longitude) {
    if (!bd || !designacao || !latitude || !longitude) return 0;

    Sensor * sen = (Sensor *)malloc(sizeof(sen));
    if (!sen) return 0;
    //Codigo Sensor
    sen->codSensor = codSensor;
    //Designação
    sen->designacao = (char *)malloc(strlen(designacao) * sizeof(char) + 1);
    if (!sen->designacao) {
        free(sen);
        return 0;
    }
    strcpy(sen->designacao, designacao);
    //Latitude
    sen->latitude = (char *)malloc(strlen(latitude) * sizeof(char) + 1);
    if (!sen->latitude) {
        free(sen->designacao);
        free(sen);
        return 0;
    }
    strcpy(sen->latitude, latitude);
    //Longitude
    sen->longitude = (char *)malloc(strlen(longitude) * sizeof(char) + 1);
    if (!sen->longitude) {
        free(sen->designacao);
        free(sen->latitude);
        free(sen);
        return 0;
    }
    strcpy(sen->longitude, longitude);

    if (!addInicioLista(bd->sensores, (void *)sen)) return 0;

    return 1;
}

int compararSensores(void *sensor1, void *sensor2) {
    if (sensor1 == NULL && sensor2 == NULL) return 0;
    if (sensor1 == NULL) return -1; //NULL < qualquer coisa
    if (sensor2 == NULL) return 1;

    Sensor *x = (Sensor *)sensor1;
    Sensor *y = (Sensor *)sensor1;

    if (x->codSensor > y->codSensor) return 1;
    if (y->codSensor < x->codSensor) return -1;
    return 0;
}

int compCodSensor(void *sensor, int codigo) {
    if (!sensor || codigo < 0) return 0;

    Sensor *x = (Sensor *)sensor;
    if (x->codSensor == codigo) return 1;
    return 0;
}

void freeSensor(void *sensor) {
    Sensor *obj = (Sensor *)sensor;
    if (obj->designacao) free(obj->designacao);
    if (obj->latitude) free(obj->latitude);
    if (obj->longitude) free(obj->longitude);
    free(obj);
}

void mostrarSensor(void *sensor){
    if (!sensor) return;
    Sensor *x = (Sensor *) sensor;
    printf ("\nCódigo do Sensor: %d", x->codSensor);
    printf ("\nDesignação do Sensor: %s", x->designacao);
    printf ("\nLocalização do Sensor: %s\t%s", x->latitude, x->longitude);
}
