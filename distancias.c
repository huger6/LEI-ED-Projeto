#include "distancias.h"
#include "bdados.h"

int inserirDistanciaLido(Bdados *bd, int codSensor1, int codSensor2, float distancia) {
    if (!bd || !bd->distancias || !bd->nColunasMatrizDistancias < 0) return 0;

    bd->distancias[codSensor1 * bd->nColunasMatrizDistancias + codSensor2] = distancia;
    return 1;
}

int compararDistancias(void *dist1, void *dist2) {
    if (dist1 == NULL && dist2 == NULL) return 0;
    if (dist1 == NULL) return -1; //NULL < qualquer coisa
    if (dist2 == NULL) return 1;

    Distancias *x = (Distancias *)dist1;
    Distancias *y = (Distancias *)dist2;

    if (x->distancia > y->distancia) return 1;
    if (x->distancia < y->distancia) return -1;
    return 0;
}

void freeDistancia(void *distancia) {
    Distancias *obj = (Distancias *)distancia;
    free(obj);
}

void mostrarDistancia(void *sensor1, void *sensor2, void *distancia){
    if (!sensor1 || !sensor2 || !distancia) return;
    Sensor *s1 = (Sensor*) sensor1;
    Sensor *s2 = (Sensor*) sensor2;
    Distancias *x = (Distancias*) distancia;
    
    printf("\nDistância do Sensor %d ao Sensor %d: %f", s1->codSensor, s2->codSensor, x->distancia);
}